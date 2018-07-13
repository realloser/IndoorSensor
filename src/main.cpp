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
#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

float voltage_read;
float voltage_reference;

void setup()
{
  pinMode(VOLTAGE_INPUT, INPUT);
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
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  Serial.print("Humitidty:   "); Serial.println(hum);
  Serial.print("Temperature: "); Serial.println(temp);
}

void loop()
{

  readVoltage();
  readDHT();


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
