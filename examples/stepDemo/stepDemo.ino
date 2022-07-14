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

int val_a, val_b, val_c;
int8_t step_a = 1;
int8_t step_b = 5;
int8_t step_c = 10;

void loop() {
  // передаём значение пина в poll
  // 1 - кнопка нажата, 0 - не нажата
  // поэтому инверсия
  btn.poll(!digitalRead(BTN_PIN));

  // передаём количество предварительных кликов
  if (btn.step(1)) {
    val_a += step_a;
    Serial.print("val_a: ");
    Serial.println(val_a);
  }
  if (btn.step(2)) {
    val_b += step_b;
    Serial.print("val_b: ");
    Serial.println(val_b);
  }
  if (btn.step(3)) {
    val_c += step_c;
    Serial.print("val_c: ");
    Serial.println(val_c);
  }

  // разворачиваем шаг для изменения в обратную сторону
  // передаём количество предварительных кликов
  if (btn.releaseStep(1)) step_a = -step_a;
  if (btn.releaseStep(2)) step_b = -step_b;
  if (btn.releaseStep(3)) step_c = -step_c;
}
