#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(9600);
  
  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  sensors_event_t event; 
  accel.getEvent(&event);

  float x = map(event.acceleration.x, -1000, 1000, -10, 10);  // Adjust these values based on calibration
  float y = map(event.acceleration.y, -1000, 1000, -10, 10);
  float z = map(event.acceleration.z, -1000, 1000, -10, 10);

  Serial.print("Calibrated Acceleration (g): ");
  Serial.print("X: "); Serial.print(x);
  Serial.print(", Y: "); Serial.print(y);
  Serial.print(", Z: "); Serial.println(z);

  delay(1000);
}
