#include <Arduino.h>

const int PIN_STEP = 10;
const int PIN_DIR = 11;
const int PIN_EN = 12;

// Para passo completo, no Nema 17, 200 passos configura revolução completa.
const int STEPS360 = 200;
const int STEPS90 = STEPS360 / 4;

// =============================================================================

void steps_rotate_angle(int steps, bool forward) {
  digitalWrite(PIN_STEP, LOW);
  digitalWrite(PIN_DIR, forward ? HIGH : LOW);
  delayMicroseconds(5);
  //
  for (int i = 0; i < steps; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(2000);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(2000);
  }
}

// =============================================================================

void setup() {
  Serial.begin(115200);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_STEP, LOW);
  digitalWrite(PIN_EN, HIGH);
  digitalWrite(PIN_DIR, LOW);
}

// =============================================================================

void loop() {
  Serial.println("Girando 90 graus em um sentido.");
  steps_rotate_angle(STEPS90, true);
  delay(1000);
  Serial.println("Retornando para a posição inicial.");
  steps_rotate_angle(STEPS90, false);
  delay(1000);
}
