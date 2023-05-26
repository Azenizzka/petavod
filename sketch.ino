#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Stepper.h>

#define DELTA -6

#define BUTTON_UP_PIN 4 // КНОПКА ДЛЯ ПОВЫШЕНИЯ ТЕМПЕРАТУРЫ
#define BUTTON_DOWN_PIN 5 // КНОПКА ДЛЯ ПОНИЖЕНИЯ ТЕМПЕРАТУРЫ
#define TEMP_STEP 10 // ШАГ ИЗМЕНЕНИЯ ТЕМПЕРАТУРЫ (+X / -X)

#define TEMP_PIN A0 // ТЕРМОРЕЗИСТОР
#define RELAY_PIN 3 // РЕЛЕ, УПРАВЛЯЮЩЕЕ НАГРЕВОМ
#define SPEAKER_PIN 7

//#define STEPPER_SPEED 3 // СКОРОСТЬ ШАГА
//#define STEPS 90 // КОЛИЧЕСТВО ШАГОВ

LiquidCrystal_I2C LCD(0x27, 16, 2);
//Stepper myStepper(STEPS, 8, 9, 10, 11);

int res = 25;
bool isHeatToggle = false; 
bool isPressedUp = false;
bool isPressedDown = false;

int temp = 350;

int getLTMP() {
  return (temp-DELTA);
}

int getHTMP() {
  return (temp+DELTA);
}


void setup() {
  Serial.begin(9600);

  LCD.init();
  LCD.backlight();

  //myStepper.setSpeed(STEPPER_SPEED);
  
  pinMode(TEMP_PIN, INPUT);
  pinMode(BUTTON_UP_PIN, INPUT); 
  pinMode(BUTTON_DOWN_PIN, INPUT); 

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

}

unsigned long lcdTimer;
unsigned int speakerTimer;

void loop() {
  doRelay();
  if(millis() - lcdTimer > 750) {
    lcdTimer = millis();
    printLCD();
  }
  checkButtonUp();
  checkButtonDown();
  if(millis() - speakerTimer > 750) {
     speakerTimer = millis();
     digitalWrite(SPEAKER_PIN, LOW);
  }
}

void checkButtonUp() {
  if(digitalRead(BUTTON_UP_PIN) && !isPressedUp) {
    isPressedUp = true;
    tempUp();
    printLCD();
  }

  if(!digitalRead(BUTTON_UP_PIN)) {
    isPressedUp = false;
  }
}

void checkButtonDown() {
  if(digitalRead(BUTTON_DOWN_PIN) && !isPressedDown) {
    isPressedDown = true;
    
    tempDown();
    printLCD();
  }

  if(!digitalRead(BUTTON_DOWN_PIN)) {
    isPressedDown = false;
  }
}

void tempUp() {
  if(temp <= 1000) {
    temp = temp + TEMP_STEP;
  } else {
    doSpeaker();
  }
}


void doSpeaker() {
  speakerTimer = millis();
  digitalWrite(SPEAKER_PIN, HIGH);
}

void tempDown() {
  if(temp >= 250) {
    temp = temp - TEMP_STEP;
  } else {
    doSpeaker();
  }
}

void doStepper() {
  //myStepper.step(STEPS);
}

void doRelay() {
  res = getResistance();
  if(res < getLTMP()) {
    digitalWrite(RELAY_PIN, LOW);
    isHeatToggle = false;
  } else if(res > temp) {
   digitalWrite(RELAY_PIN, HIGH);
   isHeatToggle = true;
  }
}

void printLCD() {
  LCD.clear();

  LCD.setCursor(0, 0);
  LCD.print("R: ");
  LCD.setCursor(3, 0);
  LCD.print(res);

  LCD.setCursor(11, 0);
  LCD.print("<");
  LCD.setCursor(12, 0);
  LCD.print(temp);

  LCD.setCursor(0, 1);
  if(isHeatToggle) {
    LCD.print("Heat: true");
  } else {
    LCD.print("Heat: false");
  }
}

int getResistance() {
    int t = analogRead(TEMP_PIN);
  return t;       
}
