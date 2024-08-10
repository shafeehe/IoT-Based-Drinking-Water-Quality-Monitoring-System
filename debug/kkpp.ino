#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 2 on ESP32
#define ONE_WIRE_BUS 34

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  pinMode(18, INPUT_PULLUP);
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

  delay(2000); // Wait 2 seconds before next reading
}
