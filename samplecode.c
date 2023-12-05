#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <ArduinoHttpClient.h>

// Define the pins for the GSM module
#define GSM_TX 7
#define GSM_RX 8

// Define the accelerometer
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Define the GPS module
SoftwareSerial gpsSerial(2, 3); // RX, TX

// Define the server details
const char *server = "your-server.com";
const int port = 80;
const String endpoint = "/upload";
const String apiKey = "your-api-key";

void setup() {
  Serial.begin(9600);
  
  // Initialize the accelerometer
  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }
  
  // Initialize the GPS module
  gpsSerial.begin(9600);
  
  // Initialize the GSM module
  Serial.begin(9600);
  Serial.println("Initializing GSM module...");
  delay(1000);
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
}

void loop() {
  // Read accelerometer data
  sensors_event_t event; 
  accel.getEvent(&event);
  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;
  
  // Read GPS data
  String gpsData = readGPSData();
  
  // Get timestamp
  String timestamp = getTimestamp();
  
  // Prepare data in CSV format
  String csvData = timestamp + "," + String(x) + "," + String(y) + "," + String(z) + "," + gpsData;
  
  // Send data to the server
  sendToServer(csvData);
  
  delay(5000); // Delay between readings
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

String getTimestamp() {
  // Implement your timestamp retrieval logic here
  return "2023-12-01T12:00:00";
}

void sendToServer(String data) {
  Serial.println("Sending data to server...");
  
  // Start GSM connection
  Serial.println("AT+CIPSHUT");
  delay(1000);
  Serial.println("AT+CSTT=\"your-apn\",\"\",\"\""); // Set your APN details
  delay(1000);
  Serial.println("AT+CIICR");
  delay(1000);
  Serial.println("AT+CIFSR");
  delay(1000);
  
  // Make HTTP POST request
  HttpClient client = HttpClient(Serial, server, port);
  client.post(endpoint, "application/json", data);
  client.sendHeader("X-API-Key", apiKey);
  int statusCode = client.responseStatusCode();
  
  if (statusCode == 200) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Failed to send data. HTTP Status Code: " + String(statusCode));
  }
  
  // Close GSM connection
  Serial.println("AT+CIPCLOSE");
  delay(1000);
}
