
#include <LiquidCrystal_I2C.h>

#define DELTA -6

#define BUTTON_UP_PIN 7 // КНОПКА ДЛЯ ПОВЫШЕНИЯ ТЕМПЕРАТУРЫ
#define BUTTON_DOWN_PIN 6 // КНОПКА ДЛЯ ПОНИЖЕНИЯ ТЕМПЕРАТУРЫ
#define BUTTON_TOGGLE_STEPPER 12
#define TEMP_STEP 5 // ШАГ ИЗМЕНЕНИЯ ТЕМПЕРАТУРЫ (+X / -X)

#define TEMP_PIN A0 // ТЕРМОРЕЗИСТОР
#define RELAY_PIN 2 // РЕЛЕ, УПРАВЛЯЮЩЕЕ НАГРЕВОМ
#define SPEAKER_PIN 5

#define ENA 8
#define DIR 9
#define PUL 10



LiquidCrystal_I2C LCD(0x27, A5, A4);

int res = 25;
bool isHeatToggle = false; 
bool isPressedUp = false;
bool isPressedDown = false;
bool isPressedToggleStepper = false;
bool isToggleStepper = false;
bool stepperBool = false;

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
  
  pinMode(TEMP_PIN, INPUT);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_TOGGLE_STEPPER, INPUT_PULLUP);

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
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
  checkButtonToggleStepper();
  if(millis() - speakerTimer > 750) {
    speakerTimer = millis();
    digitalWrite(SPEAKER_PIN, LOW);
  }

  if(isToggleStepper) {
    digitalWrite(ENA, 0);
    doStepper();
  } else {
    digitalWrite(ENA, 1);
  }
}

unsigned long stepperTimer;
int timeImpulse = 80;

void doStepper() {
  for(int i = 0; i <= 200; i++) {
    if(millis() - stepperTimer > timeImpulse) {
      stepperTimer = millis();
      stepperBool = !stepperBool;
      digitalWrite(PUL, stepperBool);
    }
  }
}

void checkButtonToggleStepper() {
  if(!digitalRead(BUTTON_TOGGLE_STEPPER) && !isPressedToggleStepper) {
    isPressedToggleStepper = true;
    isToggleStepper = !isToggleStepper;
    
  }

  if(digitalRead(BUTTON_TOGGLE_STEPPER)) {
    isPressedToggleStepper = false;
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
