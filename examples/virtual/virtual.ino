// пример работы как с виртуальной кнопкой
// кнопка подключена на D3 и GND
// будем передавать состояние кнопки вручную
#define BTN_PIN 3

#include <VirtualButton.h>
VButton btn;

void setup() {
  Serial.begin(9600);
  // подтянем внутренней подтяжкой
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
  // передаём значение пина в poll
  // 1 - кнопка нажата, 0 - не нажата
  // поэтому инверсия
  btn.poll(!digitalRead(BTN_PIN));

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
  
  // прошло 5 секунд с момента отпускания кнопки
  if (btn.timeout(5000)) Serial.println("timeout");

  // вывести количество кликов
  if (btn.hasClicks()) {
    Serial.print("has clicks ");
    Serial.println(btn.clicks);
  }
}
