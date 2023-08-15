#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// Tableau broches
int pompe1Pin = 2;
int pompe2Pin = 3;
int boutonPoussoirPompe1 = 4;
int boutonPoussoirPompe2 = 5;
int interSelecteurMode = 6;
int interSelecteurModeAdj = 7;
int boutonPoussoirAdjPlus = 8;
int boutonPoussoirAdjMoins = 9;
int trigger = 10;
int echo = 11;

// Variables
const int seuilPompe1 = 50;
const int seuilPompe2 = 30;
const int max_distance = 400;
unsigned int distance = 0;

// États
boolean selecteurMode = false;
boolean selecteurModeAdj = false;
boolean etatPompe1Pin = false;
boolean etatPompe2Pin = false;
boolean bpPompe1 = false;
boolean bpPompe2 = false;
boolean bpAdjPlus = false;
boolean bpAdjMoins = false;
boolean modeManuel = false;
boolean modeAuto = true;
boolean modeAdj = false;

// Modules externes
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(trigger, echo, max_distance);

void setup() {
  pinMode(pompe1Pin, OUTPUT);
  pinMode(pompe2Pin, OUTPUT);
  pinMode(boutonPoussoirPompe1, INPUT);
  pinMode(boutonPoussoirPompe2, INPUT);
  pinMode(interSelecteurMode, INPUT);
  pinMode(interSelecteurModeAdj, INPUT);
  pinMode(boutonPoussoirAdjPlus, INPUT);
  pinMode(boutonPoussoirAdjMoins, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Les broches sont LOW au démarrage
  digitalWrite(LED_BUILTIN, LOW);

  // Initialisation des fonctions du lcd
  lcd.init();
  lcd.backlight();
}

void loop() {

  distance = sonar.ping_cm();

  //  Debug
  Serial.println("modeManuel");
  Serial.println(modeManuel);
  Serial.println("------");
  delay(1000);
  Serial.println("modeAuto");
  Serial.println(modeAuto);
  Serial.println("------");
  delay(1000);
  Serial.println("modeAdj");
  Serial.println(modeAdj);
  Serial.println("------");
  delay(1000);
  Serial.println("selecteurMode");
  Serial.println(selecteurMode);
  Serial.println("------");
  delay(1000);
  Serial.println("selecteurModeAdj");
  Serial.println(selecteurModeAdj);
  Serial.println("------");
  delay(1000);
  Serial.println("etatPompe1Pin");
  Serial.println(etatPompe1Pin);
  Serial.println("------");
  delay(1000);
  Serial.println("etatPompe2Pin");
  Serial.println(etatPompe2Pin);
  Serial.println("------");
  delay(1000);
  Serial.println("bpPompe1");
  Serial.println(bpPompe1);
  Serial.println("------");
  delay(1000);
  Serial.println("bpPompe2");
  Serial.println(bpPompe2);
  Serial.println("------");
  delay(1000);
  Serial.println("bpAdjPlus");
  Serial.println(bpAdjPlus);
  Serial.println("------");
  delay(1000);
  Serial.println("bpAdjMoins");
  Serial.println(bpAdjMoins);
  Serial.println("------");
  delay(1000);

  if (digitalRead(interSelecteurMode) == LOW) {  // Selecteur Mode
    modeManuel = false;                          // Mode manuel off
    modeAuto = true;                             // Mode auto on
    delay(250);
  } else {
    modeManuel = true;  // Mode manuel on
    modeAuto = false;   // Mode auto off
    delay(250);
  }

  if (digitalRead(interSelecteurModeAdj) == LOW) {  // Selecteur modeAdj
    modeAdj = false;
    delay(250);
  } else {
    modeAdj = true;
    delay(250);
  }


  if (modeManuel) {
    if (boutonPoussoirPompe1 == LOW) {  // boutonPoussoirPompe1
      digitalWrite(pompe1Pin, LOW);     // pompe1Pin
      etatPompe1Pin = false;
    } else {
      digitalWrite(pompe1Pin, HIGH);  // pompe1Pin
      etatPompe1Pin = true;
    }

    if (boutonPoussoirPompe2 == LOW) {  // boutonPoussoirPompe2
      digitalWrite(pompe2Pin, LOW);     // pompe2Pin
      etatPompe2Pin = false;
    } else {
      digitalWrite(pompe2Pin, HIGH);  // pompe2Pin
      etatPompe2Pin = true;
    }
  }
}