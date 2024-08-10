const int turbidityPin = 34; // Analog pin to which the turbidity sensor is connected
int turbidityValue;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Read the analog value from the turbidity sensor
  turbidityValue = analogRead(turbidityPin);

  // Print the raw turbidity value to the serial monitor
  Serial.print("Raw Turbidity Value: ");
  Serial.println(turbidityValue);

  // Map the raw value to turbidity values (in NTU)
  float turbidityNTU = mapFloat(turbidityValue, 0, 4095, 0, 100); // Adjust the 0 and 100 to the appropriate range

  // Print the calibrated turbidity value (in NTU) to the serial monitor
  Serial.print("Turbidity: ");
  Serial.print(turbidityNTU);
  Serial.println(" NTU");

  delay(1000);
}

// Function to map a value from one range to another
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
