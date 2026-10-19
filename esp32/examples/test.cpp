// Uart testing.
// Use `/dev/ttyUSB0` instead of `/dev/ttyACM0`.

#include <Arduino.h>

HardwareSerial Uart0(0);

// =============================================================================

void setup() {
  Uart0.begin(115200);
}

// =============================================================================

void loop() {
  Uart0.println(".");
  delay(1000);
}
