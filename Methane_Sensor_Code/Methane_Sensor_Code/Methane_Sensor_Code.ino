#include <SoftwareSerial.h>

SoftwareSerial methane(12, 13);  // Arduino RX, TX

void setup() {
  Serial.begin(9600);
  methane.begin(9600);
  methane.setTimeout(1000);

  Serial.println("Waiting for sensor warm-up...");
  delay(10000);
}

void loop() {
  // Discard any leftover bytes before starting a new request.
  while (methane.available()) methane.read();

  const uint8_t request[] = {0x11, 0x01, 0x01, 0xED};
  methane.write(request, sizeof(request));  // No CR/LF

  uint8_t reply[8];
  size_t received = methane.readBytes(reply, sizeof(reply));

  Serial.print("RX: ");
  for (size_t j = 0; j < received; j++) {
    if (reply[j] < 0x10) Serial.print('0');
    Serial.print(reply[j], HEX);
    Serial.print(' ');
  }
  Serial.println();

  if (received != 8) {
    Serial.println("Missing or incomplete response");
  } else {
    uint8_t sum = 0;
    for (uint8_t j = 0; j < 8; j++) sum += reply[j];

    if (reply[0] != 0x16 || reply[1] != 0x05 ||
        reply[2] != 0x01 || sum != 0) {
      Serial.println("Invalid response or checksum");
    } else if (reply[5] != 0) {
      Serial.print("Sensor status flags: 0x");
      Serial.println(reply[5], HEX);
    } else {
      uint16_t count = ((uint16_t)reply[3] << 8) | reply[4];

      // SJH-100A: documented resolution is 0.01% volume.
      Serial.print("Methane: ");
      Serial.print(count / 100.0, 2);
      Serial.println(" % vol");
    }
  }

  delay(1000);
}