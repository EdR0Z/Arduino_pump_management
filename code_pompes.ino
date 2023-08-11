#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

int pompe1Pin = 2;
int pompe2Pin = 3;
int boutonManuelPompe1Pin = 4;
int boutonManuelPompe2Pin = 5;
int boutonModePin = 6;
int ledPompe1Pin = 7;
int ledPompe2Pin = 8;
int ledSystemePin = 9;
int triggerPin = 10;
int echoPin = 11;
int boutonAdjSeuils = 12;
int boutonPosSeuils = 13;
int boutonNegSeuils = A1;

LiquidCrystal_I2C lcd(0x27, 16, 2);

boolean modeManuel = false;
boolean modeAdjSeuils = false;
boolean etatManuelPompe1 = false;
boolean etatManuelPompe2 = false;
boolean etatLedPompe1 = false;
boolean etatLedPompe2 = false;

const unsigned long distanceReadInterval = 200;
unsigned long lastDistanceReadTime = 0;
unsigned int distance = 0;
unsigned long previousMillis = 0;
unsigned long previousMillisSystem = 0;
const long intervalSystem = 500;
const int seuilAutoPompe1 = 10;
const int seuilAutoPompe2 = 5;
const int maxDistance = 400;

NewPing sonar(triggerPin, echoPin, maxDistance);

void setup() {
  pinMode(pompe1Pin, OUTPUT);
  pinMode(pompe2Pin, OUTPUT);
  pinMode(ledPompe1Pin, OUTPUT);
  pinMode(ledPompe2Pin, OUTPUT);
  pinMode(ledSystemePin, OUTPUT);
  pinMode(boutonManuelPompe1Pin, INPUT_PULLUP);
  pinMode(boutonManuelPompe2Pin, INPUT_PULLUP);
  pinMode(boutonModePin, INPUT_PULLUP);
  pinMode(boutonAdjSeuils, INPUT_PULLUP);
  pinMode(boutonPosSeuils, INPUT_PULLUP);
  pinMode(boutonNegSeuils, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  digitalWrite(ledSystemePin, HIGH);
}

void loop() {
  unsigned long currentTime = millis();

  if (digitalRead(boutonModePin) == LOW) {
    modeManuel = false;
    digitalWrite(ledSystemePin, HIGH);
    delay(250);
  } else {
    modeManuel = true;
    delay(250);
  }

  etatManuelPompe1 = digitalRead(boutonManuelPompe1Pin) == LOW;
  etatManuelPompe2 = digitalRead(boutonManuelPompe2Pin) == LOW;

  if (currentTime - lastDistanceReadTime >= distanceReadInterval) {
    distance = sonar.ping_cm();
    if (!modeManuel) {
      if (distance <= seuilAutoPompe1) {
        controlerPompe(pompe1Pin, true, ledPompe1Pin);
        etatLedPompe1 = true;
      } else {
        controlerPompe(pompe1Pin, false, ledPompe1Pin);
        etatLedPompe1 = false;
      }
      if (distance <= seuilAutoPompe2) {
        controlerPompe(pompe2Pin, true, ledPompe2Pin);
        etatLedPompe2 = true;
      } else {
        controlerPompe(pompe2Pin, false, ledPompe2Pin);
        etatLedPompe2 = false;
      }
    } else {
      controlerPompe(pompe1Pin, etatManuelPompe1, ledPompe1Pin);
      etatLedPompe1 = etatManuelPompe1;
      controlerPompe(pompe2Pin, etatManuelPompe2, ledPompe2Pin);
      etatLedPompe2 = etatManuelPompe2;
    }
    lastDistanceReadTime = currentTime;
  }

  if (modeManuel) {
    digitalWrite(ledPompe1Pin, etatManuelPompe1);
    digitalWrite(ledPompe2Pin, etatManuelPompe2);
    if (currentTime - previousMillisSystem >= intervalSystem) {
      previousMillisSystem = currentTime;
      digitalWrite(ledSystemePin, !digitalRead(ledSystemePin));
    }
    lcd.setCursor(0, 2);
    if (etatManuelPompe1 && etatManuelPompe2) {
      lcd.print("Forcage pompe1+2 ");
    } else if (etatManuelPompe1) {
      lcd.print("Forcage pompe1   ");
    } else if (etatManuelPompe2) {
      lcd.print("Forcage pompe2   ");
    } else {
      lcd.print("Forcages inactifs");
    }
  } else {
    digitalWrite(ledPompe1Pin, etatLedPompe1);
    digitalWrite(ledPompe2Pin, etatLedPompe2);
    digitalWrite(ledSystemePin, HIGH);

    lcd.setCursor(0, 2);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm   ");
  }

  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(modeManuel ? "Manuel     " : "Automatique");

  lcd.setCursor(0, 1);
  lcd.print("P1:");
  lcd.print(etatLedPompe1 ? "ON " : "OFF");

  lcd.setCursor(8, 1);
  lcd.print("P2:");
  lcd.print(etatLedPompe2 ? "ON " : "OFF");
}

void controlerPompe(int pin, boolean etat, int ledPin) {
  digitalWrite(pin, etat);
  digitalWrite(ledPin, etat);
}
