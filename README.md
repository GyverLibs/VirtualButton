[![latest](https://img.shields.io/github/v/release/GyverLibs/VirtualButton.svg?color=brightgreen)](https://github.com/GyverLibs/VirtualButton/releases/latest/download/VirtualButton.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/VirtualButton?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# VirtualButton
Библиотека с логикой обработки кнопки (виртуальная кнопка)
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

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **VirtualButton** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/VirtualButton/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="init"></a>
## Инициализация
```cpp
VButton btn;
```

<a id="usage"></a>
## Использование
### Описание методов
```cpp
// =================== ОПРОС ===================
// опрос, вернёт true если статус кнопки изменился. Принимает состояние кнопки (1 - нажата)
bool poll(bool s);

bool press();                   // кнопка нажата
bool release();                 // кнопка отпущена
bool click();                   // клик по кнопке

bool held();                    // кнопка удержана
bool held(uint8_t clk);         // кнопка удержана с предварительным накликиванием

bool hold();                    // кнопка удерживается
bool hold(uint8_t clk);         // кнопка удерживается с предварительным накликиванием

bool step();                    // режим импульсного удержания
bool step(uint8_t clk);         // режим импульсного удержания с предварительным накликиванием

bool releaseStep();             // кнопка отпущена после импульсного удержания
bool releaseStep(uint8_t clk);  // кнопка отпущена после импульсного удержания с предварительным накликиванием

bool hasClicks(uint8_t num);    // имеются клики
uint8_t hasClicks();            // имеются клики

bool timeout(uint16_t tout);    // с момента отпускания кнопки прошло указанное время, миллисекунд

uint8_t clicks;                 // счётчик кликов

// ================= НАСТРОЙКИ =================
// таймаут удержания кнопки для hold(), 32.. 8100 мс (по умолч. 1000 мс)
void setHoldTimeout(uint16_t tout);

// период импульсов step(), 32.. 8100 мс (по умолч. 500 мс)
void setStepTimeout(uint16_t tout);

// ================= СИСТЕМНОЕ =================
bool busy();            // вернёт true, если всё ещё нужно вызывать poll для опроса таймаутов
void reset();           // сбросить все флаги

// ============= ДЕФАЙНЫ НАСТРОЕК =============
// дефайнить ПЕРЕД ПОДКЛЮЧЕНИЕМ БИБЛИОТЕКИ, показаны значения по умолчанию
#define VB_DEB 50       // дебаунс кнопки, мс
#define VB_CLICK 400    // таймаут накликивания кнопки, мс
```

### Логика работы
*Для использования кнопки на пинах МК без написания лишнего кода используй библиотеку [EncButton](https://github.com/GyverLibs/EncButton)*

В метод `poll(state)` нужно как можно чаще передавать текущее состояние кнопки: `1` - нажата, `0` - отпущена. На основе этого 
библиотека будет обрабатывать таймауты и режимы нажатий, а затем "сигналить" при помощи набора функций (см. ниже). Большинство 
функций имеют механизм однократного срабатывания, т.е. один раз возвращают `true` при наступлении события, а затем возвращают `false` 
до повторного наступления события.

- `press()` - кнопка была нажата. *[однократно вернёт true]*
- `release()` - кнопка была отпущена. *[однократно вернёт true]*
- `timeout(tout)` - с момента отпускания кнопки прошло указанное время, миллисекунд. *[однократно вернёт true]*
- `click()` - кнопка была кликнута, т.е. нажата и отпущена до таймаута удержания. *[однократно вернёт true]*
- `held()` - кнопка была удержана дольше таймаута удержания. *[однократно вернёт true]*
- `held(clicks)` - то же самое, но функция принимает количество кликов, сделанных до удержания. Примечание: held() без аргумента перехватит вызов! См. пример *preClicks*. *[однократно вернёт true]*
- `hold()` - кнопка была удержана дольше таймаута удержания. *[возвращает true, пока удерживается]*
- `hold(clicks)` - то же самое, но функция принимает количество кликов, сделанных до удержания. Примечание: hold() без аргумента перехватит вызов! См. пример *preClicks*. *[возвращает true, пока удерживается]*
- `step()` - режим "импульсного удержания": после удержания кнопки дольше таймаута данная функция *[возвращает true с периодом VB_STEP]*. Удобно использовать для пошагового изменения переменных: `if (btn.step()) val++;`.
- `step(clicks)` - то же самое, но функция принимает количество кликов, сделанных до удержания. Примечание: step() без аргумента перехватит вызов! См. пример *StepMode* и *preClicks*.
- `releaseStep()` - кнопка была отпущена после импульсного удержания. Может использоваться для изменения знака инкремента переменной. См. пример *StepMode*. *[однократно вернёт true]*
- `releaseStep(clicks)` - то же самое, но функция принимает количество кликов, сделанных до удержания. Примечание: releaseStep() без аргумента перехватит вызов! См. пример *StepMode* и *preClicks*. *[однократно вернёт true]*
- `hasClicks(clicks)` - было сделано указанное количество кликов с периодом менее *VB_CLICK*. *[однократно вернёт true]*
- `hasClicks()` - вернёт количество кликов, сделанных с периодом менее *VB_CLICK*. В противном случае вернёт 0.
- `uint8_t clicks` - публичная переменная, хранит количество сделанных кликов с периодом менее *VB_CLICK*. Сбрасывается в 0 после нового клика.

![diagram](/doc/diagram.png)

Пример:
```cpp
void loop() {
  btn.poll(digitalRead(PIN));
  if (btn.click()) Serial.println("Click!");
  if (btn.held()) Serial.println("Held!");
}
```

### Оптимизация
Чтобы лишний раз не опрашивать "сигнальные" функции, можно поместить их в условие: метод `poll()` возвращает `true`, когда статус кнопки изменился:
```cpp
void loop() {
  if (btn.poll(digitalRead(PIN))) {
    if (btn.click()) Serial.println("Click!");
    if (btn.held()) Serial.println("Held!");
  }
}
```

### Наследование
Библиотека задумана как инструмент для разработчика. С её помощью можно расширить функциональность кнопки, опрашиваемой любым способом:
- Напрямую с пина МК
- Сдвиговый регистр
- Матричная клавиатура
- Резистивная клавиатура
- Резистивно-матричная клавиатура
- Ёмкостная кнопка
- И так далее

Пример реализации класса кнопки, который опрашивает пин стандартными средствами, но использует все возможности VirtualButton:
```cpp
class MyBtn : public VButton {
  public:
  MyBtn(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
  }
  bool tick() {
    return poll(!digitalRead(_pin));
  }
  uint8_t _pin;
};
```

Тут мы реализовали метод `tick()`, который будет опрашивать пин и передавать в `VButton`. Классу доступны все возможности VirtualButton:

```cpp
MyBtn btn(3);

void loop() {
  btn.tick();

  if (btn.press()) Serial.println("press");
  if (btn.click()) Serial.println("click");
  if (btn.release()) Serial.println("release");
  // .......
}
```

<a id="example"></a>
## Примеры
Остальные примеры смотри в **examples**!

<a id="versions"></a>
## Версии
- v1.0 - релиз
- v1.1 - добавлен механизм "таймаута"

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
