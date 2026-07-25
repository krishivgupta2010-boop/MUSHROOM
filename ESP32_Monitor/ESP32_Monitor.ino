// ============================================================
//  Mushroom Cultivation Environment Monitor
//  Hardware : ESP32 + DHT22 (or DHT11) + Soil Moisture Sensor
// ============================================================

#include <Arduino.h>
#include <DHT.h>

// Pin Definitions
#define DHT_PIN 4
#define MOISTURE_PIN 34

// Sensor Type
#define DHT_TYPE DHT22

// DHT Object
DHT dht(DHT_PIN, DHT_TYPE);

// Moisture Calibration
const int MOISTURE_DRY = 3200;
const int MOISTURE_WET = 800;

// Timing
const unsigned long READ_INTERVAL_MS = 2000;
unsigned long lastReadTime = 0;

void readAndDisplaySensors();

void setup() {

  Serial.begin(115200);
  delay(500);

  Serial.println("==============================================");
  Serial.println(" Mushroom Cultivation Environment Monitor ");
  Serial.println("==============================================");

  dht.begin();

  pinMode(MOISTURE_PIN, INPUT);

  Serial.println("[INFO] System Started");
}

void loop() {

  unsigned long now = millis();

  if (now - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = now;
    readAndDisplaySensors();
  }

}

void readAndDisplaySensors() {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[ERROR] Failed to read DHT sensor.");
    return;
  }

  int rawMoisture = analogRead(MOISTURE_PIN);

  int moisturePercent =
      map(rawMoisture, MOISTURE_DRY, MOISTURE_WET, 0, 100);

  moisturePercent = constrain(moisturePercent, 0, 100);

  String moistureStatus;

  if (moisturePercent < 20)
    moistureStatus = "Very Dry";
  else if (moisturePercent < 40)
    moistureStatus = "Dry";
  else if (moisturePercent < 65)
    moistureStatus = "Optimal";
  else if (moisturePercent < 80)
    moistureStatus = "Moist";
  else
    moistureStatus = "Very Wet";

  String tempStatus;

  if (temperature < 15)
    tempStatus = "Too Cold";
  else if (temperature < 18)
    tempStatus = "Cool";
  else if (temperature <= 24)
    tempStatus = "Optimal";
  else if (temperature <= 28)
    tempStatus = "Warm";
  else
    tempStatus = "Too Hot";

  String humidityStatus;

  if (humidity < 70)
    humidityStatus = "Too Low";
  else if (humidity < 80)
    humidityStatus = "Acceptable";
  else if (humidity <= 95)
    humidityStatus = "Optimal";
  else
    humidityStatus = "Too High";

  Serial.println("----------------------------------------------");
  Serial.printf("Temperature : %.1f C [%s]\n",
                temperature, tempStatus.c_str());

  Serial.printf("Humidity    : %.1f %% [%s]\n",
                humidity, humidityStatus.c_str());

  Serial.printf("Moisture    : %d %% (Raw:%d) [%s]\n",
                moisturePercent,
                rawMoisture,
                moistureStatus.c_str());

  Serial.println("----------------------------------------------");
}