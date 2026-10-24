#include <Arduino.h>
#include <HX711.h>

// Pinout ======================================================================
// ATENÇÃO: Apenas utilize os pinos prórpios para GPIO. Por exemplo, usar os
// pinos 19 e 20 desliga o port JTAG completamente.
//
// PINOS SEGUROS: 4—18, 21, 38—42
// =============================================================================

// Load Cell ===================================================================

#define PIN_LOAD_CLK 4
#define PIN_LOAD_DATA 5

HX711 load_cell;

// =============================================================================

// Step Motor ==================================================================

#define PIN_STEP_STEP 6
#define PIN_STEP_DIR 7

// Para passo completo, no Nema 17, 200 passos configura revolução completa.
const int STEPS360 = 200;
const int STEPS90 = STEPS360 / 4;

void steps_rotate_angle(int steps, bool forward) {
  digitalWrite(PIN_STEP_STEP, LOW);
  digitalWrite(PIN_STEP_DIR, forward ? HIGH : LOW);
  delayMicroseconds(5);
  for (int i = 0; i < steps; i++) {
    digitalWrite(PIN_STEP_STEP, HIGH);
    delayMicroseconds(2000);
    digitalWrite(PIN_STEP_STEP, LOW);
    delayMicroseconds(2000);
  }
}

// =============================================================================

// Main ========================================================================

void setup() {
  Serial.begin(115200);

  // Load Cell Test ---------------------------------------------------------
  // Serial.println("HX711 Demo");
  // Serial.println("Initializing the load_cell");
  // // Initialize library with data output pin, clock input pin and gain factor.
  // // Channel selection is made by passing the appropriate gain:
  // // - With a gain factor of 64 or 128, channel A is selected
  // // - With a gain factor of 32, channel B is selected
  // // By omitting the gain factor parameter, the library
  // // default "128" (Channel A) is used here.
  // load_cell.begin(PIN_LOAD_DATA, PIN_LOAD_CLK);
  // Serial.println("Before setting up the load_cell:");
  // Serial.print("read: \t\t");
  // Serial.println(load_cell.read()); // print a raw reading from the ADC
  // Serial.print("read average: \t\t");
  // Serial.println(load_cell.read_average(
  //     20)); // print the average of 20 readings from the ADC
  // Serial.print("get value: \t\t");
  // Serial.println(
  //     load_cell.get_value(5)); // print the average of 5 readings from the ADC
  //                              // minus the tare weight (not set yet)
  // Serial.print("get units: \t\t");
  // Serial.println(
  //     load_cell.get_units(5),
  //     1); // print the average of 5 readings from the ADC minus tare weight (not
  //         // set) divided by the load_cell parameter (not set yet)
  // load_cell.set_scale(
  //     2280.f); // this value is obtained by calibrating the load_cell with known
  //              // weights; see the README for details
  // load_cell.tare(); // reset the load_cell to 0
  // Serial.println("After setting up the load_cell:");
  // Serial.print("read: \t\t");
  // Serial.println(load_cell.read()); // print a raw reading from the ADC
  // Serial.print("read average: \t\t");
  // Serial.println(load_cell.read_average(
  //     20)); // print the average of 20 readings from the ADC
  // Serial.print("get value: \t\t");
  // Serial.println(
  //     load_cell.get_value(5)); // print the average of 5 readings from the ADC
  //                              // minus the tare weight, set with tare()
  // Serial.print("get units: \t\t");
  // Serial.println(
  //     load_cell.get_units(5),
  //     1); // print the average of 5 readings from the ADC minus tare weight,
  //         // divided by the load_cell parameter set with set_scale
  // Serial.println("Readings:");
  // Load Cell Test ---------------------------------------------------------

  pinMode(PIN_STEP_STEP, OUTPUT);
  pinMode(PIN_STEP_DIR, OUTPUT);
  digitalWrite(PIN_STEP_STEP, LOW);
  digitalWrite(PIN_STEP_DIR, LOW);
}

void loop() {
  // Load Cell Test ---------------------------------------------------------
  // https://github.com/bogde/HX711/blob/master/examples/HX711_basic_example/HX711_basic_example.ino
  // Serial.print("one reading:\t");
  // Serial.print(load_cell.get_units(), 1);
  // Serial.print("\t| average(3):\t");
  // Serial.println(load_cell.get_units(3), 1);
  //
  // delay(500);
  // Load Cell Test ---------------------------------------------------------

  // Step Motor Test --------------------------------------------------------
  Serial.println("Girando 360 graus em um sentido.");
  steps_rotate_angle(STEPS360, true);
  delay(1000);
  Serial.println("Retornando para a posição inicial.");
  steps_rotate_angle(STEPS360, false);
  delay(1000);
  // Step Motor Test --------------------------------------------------------
}

// =============================================================================
