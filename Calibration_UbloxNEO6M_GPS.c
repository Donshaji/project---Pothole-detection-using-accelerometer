#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  String gpsData = readGPSData();
  Serial.println("Raw GPS Data: " + gpsData);

  // Parse and process the GPS data as needed
  
  delay(1000);
}

String readGPSData() {
  String data = "";
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    data += c;
    delay(2);
  }
  return data;
}
