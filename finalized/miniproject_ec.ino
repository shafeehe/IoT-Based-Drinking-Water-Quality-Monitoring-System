#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <Adafruit_ADS1X15.h>
#include <DFRobot_ESP_EC.h>
#include <Adafruit_GFX.h>

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 18
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_ROWS    2
const int tdsPin = 34;
const int turbidityPin = 35; // Analog pin to which the turbidity sensor is connected
int turbidityValue;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

 float  temperature = 25;
 
String apiKey = "FG3VYTORRQK79XXL";     //  Enter your Write API key from ThingSpeak
 
const char* ssid =  "Pro";     // replace with your wifi ssid and wpa2 key
const char* passwrd =  "11111111";
const char* server = "api.thingspeak.com";
 
WiFiClient client;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
  ec.begin();

  // Initialize DS18B20 sensor
  sensors.begin();

   Wire.begin();

  // Initialize LCD
  lcd.init();
  
  // Turn on the backlight
  lcd.backlight();


  delay(2000);
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
 
 
  WiFi.begin(ssid, passwrd);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Request temperature readings
  sensors.requestTemperatures();

  // Get temperature in Celsius
  float temperatureC = sensors.getTempCByIndex(0);

  int tdsValue = analogRead(tdsPin);
  turbidityValue = analogRead(turbidityPin);
  float turbidityNTU = mapFloat(turbidityValue,0 ,10000, 6.7, 0);
  
  //ecValue = ec.readEC(tdsValue, temperatureC);

  // Check if temperature reading is valid
    lcd.print("Temperature:");
    lcd.print(temperatureC,'\n');
    Serial.println(temperature,'\n');
    delay(2000);
    lcd.clear();
    lcd.print("TDS Value: ");
    Serial.print(tdsValue);
    lcd.print(tdsValue);
    delay(2000);
    lcd.clear();
    lcd.print("Turbidity:");
    lcd.print(turbidityNTU);
    delay(2000);
    lcd.clear();

   // ec.calibration(tdsValue, temperatureC); // calibration process by Serail CMD
 
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
 
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperatureC);
    postStr += "&field2=";
    postStr += String(tdsValue);
    postStr += "&field3=";
    postStr += String(turbidityNTU);
    postStr += "\r\n\r\n\r\n";
    delay(500);
 
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n\n");
    client.print(postStr);
    delay(500);
  }
  client.stop();
}
