#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Stepper.h>

#define TMP 450 
#define DELTA -6
#define LTMP (TMP-DELTA)
#define HTMP (TMP+DELTA)

#define RELAY_PIN 3

LiquidCrystal_I2C LCD(0x27, 16, 2);

const int stepsPerRevolution = 90;   
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

const byte tempPin = A0;
int res = 25;
bool toggle = false;

void setup() {
  myStepper.setSpeed(3);
  LCD.init();
  LCD.backlight();
  Serial.begin( 9600 );
  pinMode(tempPin, INPUT);
  pinMode(RELAY_PIN, OUTPUT);  
}

void loop() {
    myStepper.step(stepsPerRevolution);
    delay(100);
    res = getResistance();

if(res < LTMP) {
  digitalWrite(RELAY_PIN, LOW);
  toggle = false;
} else if(res > TMP) {
  digitalWrite(RELAY_PIN, HIGH);
  toggle = true;
}

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("R: ");

  LCD.setCursor(11, 0);
  LCD.print("<");
  LCD.setCursor(12, 0);
  LCD.print(TMP);

  LCD.setCursor(3, 0);
  LCD.print(res);
  LCD.setCursor(0, 1);
  if(toggle) {
    LCD.print("Heat: true");
  } else {
    LCD.print("Heat: false");
  }
}

int getResistance() {
    int t = analogRead( tempPin );
  return t;       
}