/*
    Библиотека с логикой обработки кнопки (виртуальная кнопка)
    Документация:
    GitHub: https://github.com/GyverLibs/VirtualButton
    Возможности:
    - Очень лёгкий оптимизированный код
    - Множество сценариев использования
    - Позволяет расширить функционал других библиотек
    - Обработка:
        - Антидребезг
        - Нажатие
        - Отпускание
        - Клик
        - Несколько кликов
        - Счётчик кликов
        - Удержание
        - Импульсное удержание
        - Действия с предварительными кликами
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
    
    v1.1 - добавлен механизм "таймаута"
*/

#ifndef _VirtualButton_h
#define _VirtualButton_h

// ========= НАСТРОЙКИ (можно передефайнить из скетча) ==========
#define _VB_DEB 50          // дебаунс кнопки, мс
#define _VB_CLICK 400	    // таймаут накликивания, мс

// =========== НЕ ТРОГАЙ ============
#include <Arduino.h>

#ifndef VB_DEB
#define VB_DEB _VB_DEB
#endif
#ifndef VB_CLICK
#define VB_CLICK _VB_CLICK
#endif

// ======================================= CLASS =======================================
class VButton {
public:
    // таймаут удержания кнопки для hold(), 32.. 8100 мс (по умолч. 1000 мс)
    void setHoldTimeout(uint16_t tout) {
        _holdT = tout >> 5;
    }
    
    // период импульсов step(), 32.. 8100 мс (по умолч. 500 мс)
    void setStepTimeout(uint16_t tout) {
        _stepT = tout >> 5;
    }
    
    // опрос, вернёт true если статус кнопки изменился. Принимает состояние кнопки (1 - нажата)
    bool poll(bool s) {
        uint16_t prev = _flags;
        if (s || readF(9)) pollBtn(s);  // опрос если кнопка нажата или не вышли таймауты
        return (prev != _flags);
    }
    
    // сбросить все флаги
    void reset() {
        _flags = 0;
    }

    // ======================================= BTN =======================================
    bool busy() { return readF(9); }             // вернёт true, если всё ещё нужно вызывать tick для опроса таймаутов
    bool press() { return checkF(3); }           // кнопка нажата
    bool release() { return checkF(10); }        // кнопка отпущена
    bool click() { return checkF(0); }           // клик по кнопке
    
    bool held() { return checkF(1); }            // кнопка удержана
    bool hold() { return readF(4); }             // кнопка удерживается
    bool step() { return checkF(2); }            // режим импульсного удержания
    bool releaseStep() { return checkF(12); }    // кнопка отпущена после импульсного удержания
    
    bool held(uint8_t clk) { return (clicks == clk) ? checkF(1) : 0; }              // кнопка удержана с предварительным накликиванием
    bool hold(uint8_t clk) { return (clicks == clk) ? readF(4) : 0; }               // кнопка удерживается с предварительным накликиванием
    bool step(uint8_t clk) { return (clicks == clk) ? checkF(2) : 0; }              // режим импульсного удержания с предварительным накликиванием
    bool releaseStep(uint8_t clk) { return (clicks == clk) ? checkF(12) : 0; }      // кнопка отпущена после импульсного удержания с предварительным накликиванием
    
    bool hasClicks(uint8_t num) { return (clicks == num && checkF(7)) ? 1 : 0; }    // имеются клики
    uint8_t hasClicks() { return checkF(6) ? clicks : 0; }                          // имеются клики
    
    // с момента отпускания кнопки прошло указанное время, миллисекунд
    bool timeout(uint16_t tout) { return ((uint16_t)(millis() & 0xFFFF) - _debTmr > tout && checkF(15)); }
    
    uint8_t clicks = 0;                                                             // счётчик кликов
    
private:
    // ===================================== POOL BTN =====================================
    void pollBtn(bool state) {
        uint16_t ms = millis() & 0xFFFF;
        uint16_t debounce = ms - _debTmr;
        if (state) {                                                // кнопка нажата
            setF(9);                                                // busy флаг
            if (!readF(8)) {                                        // и не была нажата ранее
                if (readF(14)) {                                    // ждём дебаунс
                    if (debounce > VB_DEB) {                        // прошел дебаунс
                        _flags |= 0b100001000;                      // set 8 3 кнопка нажата
                        _debTmr = ms;                               // сброс таймаутов
                    }
                } else {                                            // первое нажатие
                    setF(14);                                       // запомнили что хотим нажать                    
                    if (debounce > VB_CLICK || readF(5)) {          // кнопка нажата после VB_CLICK
                        clicks = 0;                                 // сбросить счётчик и флаг кликов
                        _flags &= ~0b0011000011101111;              // clear 0 1 2 3 5 6 7 12 13
                    }
                    _debTmr = ms;
                }
            } else {                                                // кнопка уже была нажата
                if (!readF(4)) {                                    // и удержание ещё не зафиксировано
                    if (debounce >= (uint16_t)(_holdT << 5)) {      // прошло больше удержания
                        _flags |= 0b00110010;                       // set 1 4 5 запомнили что удерживается и отключаем сигнал о кликах
                        _debTmr = ms;                               // сброс таймаута
                    }
                } else {                                            // удержание зафиксировано
                    if (debounce > (uint16_t)(_stepT << 5)) {       // таймер степа
                        _flags |= 0b0010000000000100;               // set 2 13 step
                        _debTmr = ms;                               // сброс таймаута
                    }
                }
            }
        } else {                                                    // кнопка не нажата
            if (readF(8)) {                                         // но была нажата
                if (debounce > VB_DEB) {
                    if (!readF(4)) {                                // не удерживали - это клик
                        setF(0);                                    // click
                        clicks++;
                    }
                    _flags &= ~0b100010000;                         // clear 8 4                    
                    _debTmr = ms;                                   // сброс таймаута
                    _flags |= (1 << 10) | (1 << 15);                // set 10 15
                    if (checkF(13)) setF(12);                       // кнопка отпущена после step
                }
            } else if (clicks && !readF(5)) {                       // есть клики
                if (debounce > VB_CLICK) _flags |= 0b11100000;      // set 5 6 7 (клики)
            } else clrF(9);                                         // снимаем busy флаг
            checkF(14);                                             // сброс ожидания нажатия
        }
    }
    
    // ===================================== MISC =====================================
    bool checkF(const uint8_t val) { return readF(val) ? clrF(val), 1 : 0; }
    inline void setF(const uint8_t x) __attribute__((always_inline)) {_flags |= 1 << x;}
    inline void clrF(const uint8_t x) __attribute__((always_inline)) {_flags &= ~(1 << x);}
    inline bool readF(const uint8_t x) __attribute__((always_inline)) {return _flags & (1 << x);}

    uint16_t _flags = 0;
    uint8_t _holdT = 1000 >> 5;
    uint8_t _stepT = 500 >> 5;
    uint16_t _debTmr = 0;
    
    // flags
    // 0 - click
    // 1 - held
    // 2 - step
    // 3 - press
    // 4 - hold
    // 5 - clicks flag
    // 6 - clicks get
    // 7 - clicks get num
    // 8 - флаг кнопки
    // 9 - busy flag
    // 10 - btn released
    // 11 - btn level
    // 12 - btn released after step
    // 13 - step flag
    // 14 - deb flag
    // 15 - timeout
};
#endif