#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

const int pompe1Pin = 2;
const int pompe2Pin = 3;
const int boutonPoussoirPompe1 = 4;
const int boutonPoussoirPompe2 = 5;
const int interSelecteurMode = 6;
const int interSelecteurModeAdj = 7;
const int boutonPoussoirAdjMoins = 8;
const int boutonPoussoirAdjPlus = 9;
const int TRIGGER_PIN = 10;
const int ECHO_PIN = 11;

const int shP1address = 0;
const int shP2address = 1;

int seuilHautP1 = 0;
const int seuilBasP1 = 25;
int seuilHautP2 = 0;
const int seuilBasP2 = 23;

const int MAX_DISTANCE = 400; // Updated the MAX_DISTANCE value to 400 cm
String etatPompe1Str = "";
String etatPompe2Str = "";
String modeStr = "";
long duration;
int distance;

int selecteurMode = 0;
int selecteurModeAdj = 0;
int etatPompe1Pin = 0;
int etatPompe2Pin = 0;
int bpPompe1 = 0;
int bpPompe2 = 0;
int bpAdjPlus = 0;
int bpAdjMoins = 0;
int modeManuel = 0;
int modeAuto = 1;
int modeAdj = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4); // Configuration for a 20x4 I2C LCD
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Constants for error handling
const int NO_ECHO_ERROR = -1;
const int MAX_DISTANCE_ERROR = -2;

// Variables for debouncing
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(pompe1Pin, OUTPUT);
  pinMode(pompe2Pin, OUTPUT);

  pinMode(boutonPoussoirPompe1, INPUT);
  pinMode(boutonPoussoirPompe2, INPUT);
  pinMode(interSelecteurMode, INPUT);
  pinMode(interSelecteurModeAdj, INPUT);
  pinMode(boutonPoussoirAdjPlus, INPUT);
  pinMode(boutonPoussoirAdjMoins, INPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  int shP1readValue = EEPROM.read(shP1address);
  seuilHautP1 = shP1readValue;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Read shP1: ");
  lcd.setCursor(11, 0);
  lcd.print(shP1readValue);

  delay(2000);

  int shP2readValue = EEPROM.read(shP2address);
  seuilHautP2 = shP2readValue;
  lcd.setCursor(0, 1);
  lcd.print("Read shP2: ");
  lcd.setCursor(11, 1);
  lcd.print(shP2readValue);

  delay(5000);

  lcd.clear();
}

void loop() {
  delay(2);
  lcd.setCursor(0, 0);
  lcd.print(modeStr);
  lcd.setCursor(0, 1);
  lcd.print(etatPompe1Str + " " + etatPompe2Str);
  lcd.setCursor(0, 3);
  lcd.print("Distance: ");

  // Measure distance with error handling
  int distanceError = mesurerDistance();
  if (distanceError == NO_ECHO_ERROR) {
    lcd.setCursor(11, 3);
    lcd.print("Err: No Echo");
  } else if (distanceError == MAX_DISTANCE_ERROR) {
    lcd.setCursor(11, 3);
    lcd.print("Err: Max Dist");
  } else {
    if (distance < 10) {
      lcd.setCursor(11, 3);
      lcd.print(0);
      lcd.print(distance);
    } else {
      lcd.setCursor(11, 3);
      lcd.print(distance);
    }
  }

  selecteurMode = digitalRead(interSelecteurMode);
  selecteurModeAdj = digitalRead(interSelecteurModeAdj);

  if (selecteurMode == LOW) {
    modeManuel = 0;
    modeAuto = 1;
  } else if (selecteurMode == HIGH) {
    modeManuel = 1;
    modeAuto = 0;
  }

  if (selecteurModeAdj == LOW) {
    modeAdj = 0;
  } else if (selecteurModeAdj == HIGH) {
    modeAdj = 1;
  }

  if ((modeManuel) && (!modeAdj)) {
    mettreAJourPompesManuel();
  }

  if ((modeManuel) && (modeAdj)) {
    ajusterSeuils();
  }

  if ((modeAuto) && (!modeManuel)) {
    // Logic for automatic mode here
  }
}

int mesurerDistance() {
  // Measure distance with error handling
  unsigned int uS = sonar.ping();
  if (uS == NO_ECHO || uS == MAX_DISTANCE) {
    return uS; // Return the error
  }
  return uS / US_ROUNDTRIP_CM;
}

void mettreAJourPompesManuel() {
  int bpPompe1State = digitalRead(boutonPoussoirPompe1);
  int bpPompe2State = digitalRead(boutonPoussoirPompe2);

  if (bpPompe1State != bpPompe1) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (bpPompe1State == LOW) {
      digitalWrite(pompe1Pin, HIGH);
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    } else {
      digitalWrite(pompe1Pin, LOW);
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    }
    bpPompe1 = bpPompe1State;
  }

  if (bpPompe2State != bpPompe2) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (bpPompe2State == LOW) {
      digitalWrite(pompe2Pin, HIGH);
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    } else {
      digitalWrite(pompe2Pin, LOW);
      etatPompe2Str = "P2: ON ";
      etatPompe2Pin = 1;
    }
    bpPompe2 = bpPompe2State;
  }
}

void ajusterSeuils() {
  // Logic for adjusting thresholds here
}
