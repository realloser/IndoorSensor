/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <RFTransmitter.h>

// pins for Attiny85
#if defined(__AVR_ATtiny84__) || (__AVR_ATtiny2313__) || defined (__AVR_ATtiny85__)
#define RF_OUTPUT 0
#define DHT_INPUT 1
#define LIGHT_SENSOR_INPUT 2
#define VOLTAGE_INPUT 3

#else
#define VOLTAGE_INPUT 10
#define DHT_INPUT 8     // what pin we're connected to
#define LIGHT_SENSOR_INPUT 9
#define RF_OUTPUT 6

#endif

// 433 rf
#define NODE_ID 1
#define NODE_HASH "B9FC4586"

// Voltage devider
#define VOLTAGE_DEVIDER_FACTOR 2.355f

// DHT22 Sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302)


DHT dht(DHT_INPUT, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables
float dhtHum;  //Stores humidity value
float dhtTemp; //Stores temperature value

// Reference voltage variables
float voltage_read;
float voltage_reference;

// Light sensor variables
int lightSensorValue = 0, lightSensorMappedValue = 0;

RFTransmitter transmitter(RF_OUTPUT, NODE_ID);
String transmissionString;
char transmissionMessage[30];
bool pingSend;

void setup()
{
  pinMode(VOLTAGE_INPUT, INPUT);
  pinMode(LIGHT_SENSOR_INPUT, INPUT);
  dht.begin();

  // Serial.begin(9600);
}

void readVoltage() {
  voltage_read = analogRead(VOLTAGE_INPUT);
  // Serial.print("Voltage int:"); Serial.println(voltage_read);

  voltage_reference = (VOLTAGE_DEVIDER_FACTOR*1024)/voltage_read;
  // Serial.print("Voltage Reference :"); Serial.println(voltage_reference);
}

void readDHT() {
  //Read data and store it to variables hum and temp
  dhtHum = dht.readHumidity();
  dhtTemp= dht.readTemperature();
  // Serial.print("Humitidty:   "); Serial.println(dhtHum);
  // Serial.print("Temperature: "); Serial.println(dhtTemp);
}

void readLightSensor() {
  lightSensorValue = analogRead(LIGHT_SENSOR_INPUT); // read the value from the sensor
  lightSensorMappedValue = map(lightSensorValue, 0, voltage_reference, 0, 5);
  // Serial.print("Light intensity: "); Serial.println(lightSensorMappedValue);
}

void sendData(bool resend) {
  if(resend) {
    transmitter.resend((byte *)transmissionMessage, strlen(transmissionMessage) + 1);
    // Serial.println(transmissionMessage);
    // Serial.println("resend message");
  }
  else {
    transmitter.send((byte *)transmissionMessage, strlen(transmissionMessage) + 1);
    // Serial.println(transmissionMessage);
    // Serial.println("send message");
  }
}

void sendPing() {
  if (pingSend) {
    return;
  }

  transmissionString = String(NODE_HASH);
  transmissionString += String("-");
  transmissionString += String("ping");
  transmissionString.toCharArray(transmissionMessage, transmissionString.length() + 1);

  sendData(false);
  delay(500);
  sendData(true);
  delay(500);
  sendData(true);
  pingSend = true;
}

void loop()
{
  sendPing();

  readVoltage();
  readDHT();
  readLightSensor();

  // concat all strings
  // Pattern: nodeHash|temp|humidity|lightintensity|voltage
  transmissionString  = String(NODE_HASH);transmissionString += String("|");
  transmissionString += String(dhtTemp);transmissionString += String("|");
  transmissionString += String(dhtHum);transmissionString += String("|");
  transmissionString += String(lightSensorMappedValue);transmissionString += String("|");
  transmissionString += String(voltage_reference);transmissionString += String("|");
  transmissionString.toCharArray(transmissionMessage, transmissionString.length() + 1);

  sendData(false);
  delay(1000);
  sendData(true);
  delay(3000);
  sendData(true);
}
