/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

// Voltage devider
#define VOLTAGE_INPUT 10
#define VOLTAGE_DEVIDER_FACTOR 2.355f

// DHT22 Sensor
#define DHT_INPUT 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

#define LIGHT_SENSOR_INPUT 9

DHT dht(DHT_INPUT, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables
float dhtHum;  //Stores humidity value
float dhtTemp; //Stores temperature value

// Reference voltage variables
float voltage_read;
float voltage_reference;

// Light sensor variables
int lightSensorValue = 0, lightSensorMappedValue = 0;

String transmissionString;

void setup()
{
  pinMode(VOLTAGE_INPUT, INPUT);
  pinMode(LIGHT_SENSOR_INPUT, INPUT);
  dht.begin();

  Serial.begin(9600);
}

void readVoltage() {
  voltage_read = analogRead(VOLTAGE_INPUT);
  Serial.print("Voltage int:"); Serial.println(voltage_read);

  voltage_reference = (VOLTAGE_DEVIDER_FACTOR*1024)/voltage_read;
  Serial.print("Voltage Reference :"); Serial.println(voltage_reference);
}

void readDHT() {
  //Read data and store it to variables hum and temp
  dhtHum = dht.readHumidity();
  dhtTemp= dht.readTemperature();
  Serial.print("Humitidty:   "); Serial.println(dhtHum);
  Serial.print("Temperature: "); Serial.println(dhtTemp);
}

void readLightSensor() {
  lightSensorValue = analogRead(LIGHT_SENSOR_INPUT); // read the value from the sensor
  lightSensorMappedValue = map(lightSensorValue, 0, voltage_reference, 0, 5);
  Serial.print("Light intensity: "); Serial.println(lightSensorMappedValue);
}

void loop()
{

  readVoltage();
  readDHT();
  readLightSensor();

  // concat all strings
  // Pattern: temp|humidity|lightintensity|voltage
  transmissionString  = String(dhtTemp);transmissionString += String("|");
  transmissionString += String(dhtHum);transmissionString += String("|");
  transmissionString += String(lightSensorMappedValue);transmissionString += String("|");
  transmissionString += String(voltage_reference);transmissionString += String("|");

  Serial.println(transmissionString);
  
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED_BUILTIN, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN, LOW);
   // wait for a second
  Serial.println("Blink");
  delay(1000);
}
