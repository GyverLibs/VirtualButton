// пример работы библиотеки в другом классе
// для примера пусть класс опрашивает пин
#include <VirtualButton.h>

struct MyBtn : public VButton {
  MyBtn(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, INPUT_PULLUP);
  }
  bool tick() {
    return poll(!digitalRead(_pin));
  }
  uint8_t _pin;
};

MyBtn btn(3);

void setup() {
  Serial.begin(9600);
}

void loop() {
  btn.tick();

  if (btn.press()) Serial.println("press");
  if (btn.click()) Serial.println("click");
  if (btn.release()) Serial.println("release");

  if (btn.held()) Serial.println("held");     // однократно вернёт true при удержании
  //if (btn.hold()) Serial.println("hold");   // будет постоянно возвращать true после удержания
  if (btn.step()) Serial.println("step");     // импульсное удержание

  // проверка на количество кликов
  if (btn.hasClicks(1)) Serial.println("action 1 clicks");
  if (btn.hasClicks(2)) Serial.println("action 2 clicks");
  if (btn.hasClicks(3)) Serial.println("action 3 clicks");
  if (btn.hasClicks(5)) Serial.println("action 5 clicks");

  // вывести количество кликов
  if (btn.hasClicks()) {
    Serial.print("has clicks ");
    Serial.println(btn.clicks);
  }
}
