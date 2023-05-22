#include <GyverNTC.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define RELAY_PIN 2
#define SCANNER_PIN 3
float const LOW_TEMP = 125.0f;

int temp = 0;

GyverNTC therm(0, 10000, 3950);
//OneWire oneWire(SCANNER_PIN);
//DallasTemperature ds(&oneWire);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(9600);
  //ds.begin();
}

void loop() {
  //Serial.print("getTemp: ");
  Serial.println(therm.getTempAverage());
  //ds.requestTemperatures();
  //temp = ds.getTempCByIndex(0);
  //Serial.println(ds.getTempCByIndex(0));

  if (temp > LOW_TEMP) {
    digitalWrite(RELAY_PIN, LOW);
    //Serial.println("Write low");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    //Serial.println("Write high");
  }
}
