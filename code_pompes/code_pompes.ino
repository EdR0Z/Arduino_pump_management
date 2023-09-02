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

const int MAX_DISTANCE = 40;
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

LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd1(0x26, 16, 2);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd1.init();
  lcd1.backlight();

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

  digitalWrite(pompe1Pin, LOW);
  digitalWrite(pompe2Pin, LOW);

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
  if (distance < 10) {
    lcd.setCursor(10, 3);
    lcd.print(0);
    lcd.print(distance);
  } else {
    lcd.setCursor(10, 3);
    lcd.print(distance);
  }


  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(5);


  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);


  duration = pulseIn(ECHO_PIN, HIGH);
  
  distance = duration * 0.034 / 2;

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
    bpPompe1 = digitalRead(boutonPoussoirPompe1);
    bpPompe2 = digitalRead(boutonPoussoirPompe2);

    lcd.setCursor(0, 2);
    lcd.print("            ");

    modeStr = "Mode: Manuel";

    if ((bpPompe1) == 0) {
      digitalWrite(pompe1Pin, HIGH);
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    } else if ((bpPompe1) == 1) {
      digitalWrite(pompe1Pin, LOW);
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    }

    if ((bpPompe2) == 0) {
      digitalWrite(pompe2Pin, HIGH);
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    } else if ((bpPompe2) == 1) {
      digitalWrite(pompe2Pin, LOW);
      etatPompe2Str = "P2: ON ";
      etatPompe2Pin = 1;
    }
  }

  if ((modeManuel) && (modeAdj)) {
    bpAdjPlus = digitalRead(boutonPoussoirAdjPlus);
    bpAdjMoins = digitalRead(boutonPoussoirAdjMoins);
    bpPompe1 = digitalRead(boutonPoussoirPompe1);
    bpPompe2 = digitalRead(boutonPoussoirPompe2);
    digitalWrite(pompe1Pin, LOW);
    etatPompe1Pin = 0;
    digitalWrite(pompe2Pin, LOW);
    etatPompe2Pin = 0;

    modeStr = "Mode: Adjust";

    if ((bpAdjPlus) && (bpPompe1)) {
      lcd.setCursor(0, 2);
      lcd.print("Seuil P1: ");
      lcd.setCursor(10, 2);
      if (seuilHautP1 < 10) {
        lcd.print(0);
        lcd.print(seuilHautP1);
      } else {
        lcd.print(seuilHautP1);
      }
      if (seuilHautP1 > 0) {
        seuilHautP1 = seuilHautP1 + 1;
      }
    } else if ((bpAdjMoins) && (bpPompe1)) {
      lcd.setCursor(0, 2);
      lcd.print("Seuil P1: ");
      lcd.setCursor(10, 2);
      if (seuilHautP1 < 10) {
        lcd.print(0);
        lcd.print(seuilHautP1);
      } else {
        lcd.print(seuilHautP1);
      }
      if (seuilHautP1 > 0) {
        seuilHautP1 = seuilHautP1 - 1;
      }
    }
    EEPROM.write(shP1address, seuilHautP1);
    Serial.println("Write shP1:");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Write shP1...");
  }

  if ((bpAdjPlus) && (bpPompe2)) {
    lcd.setCursor(0, 2);
    lcd.print("Seuil P2: ");
    lcd.setCursor(10, 2);
    if (seuilHautP2 < 10) {
      lcd.print(0);
      lcd.print(seuilHautP2);
    } else {
      lcd.print(seuilHautP2);
    }
    if (seuilHautP2 > 0) {
      seuilHautP2 = seuilHautP2 + 1;
    }
  } else if ((bpAdjMoins) && (bpPompe2)) {
    lcd.setCursor(0, 2);
    lcd.print("Seuil P2: ");
    lcd.setCursor(10, 2);
    if (seuilHautP2 < 10) {
      lcd.print(0);
      lcd.print(seuilHautP2);
    } else {
      lcd.print(seuilHautP2);
    }
    if (seuilHautP2 > 0) {
      seuilHautP2 = seuilHautP2 - 1;
    }
    EEPROM.write(shP2address, seuilHautP2);
    Serial.println("Write shP2");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Write shP2...");
  }

  if ((modeAuto) && (!modeManuel)) {

    lcd.setCursor(0, 2);
    lcd.print("            ");

    modeStr = "Mode: Auto  ";

    if (distance <= seuilHautP1) {
      digitalWrite(pompe1Pin, HIGH);
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    } else if (distance >= seuilBasP1) {
      digitalWrite(pompe1Pin, LOW);
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    }

    if (distance <= seuilHautP2) {
      digitalWrite(pompe2Pin, HIGH);
      etatPompe2Str = "P2: ON ";
      etatPompe2Pin = 1;
    } else if (distance >= seuilBasP2) {
      digitalWrite(pompe2Pin, LOW);
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    }
  }
}