#define SERIAL_BAUD 115200

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(10);
  Serial.println("Serial is configured");
}

void loop() {
  delay(500);
  Serial.println("Marko");
}
