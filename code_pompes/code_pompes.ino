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

void printAndDelay(const char* label, int value) {
  Serial.println(label + value);
  Serial.println("------");
  delay(1000);
}

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
  printAndDelay("modeManuel", modeManuel);
  printAndDelay("modeAuto", modeAuto);
  printAndDelay("modeAdj", modeAdj);
  printAndDelay("selecteurMode", selecteurMode);
  printAndDelay("selecteurModeAdj", selecteurModeAdj);
  printAndDelay("etatPompe1Pin", etatPompe1Pin);
  printAndDelay("etatPompe2Pin", etatPompe2Pin);
  printAndDelay("bpPompe1", bpPompe1);
  printAndDelay("bpPompe2", bpPompe2);
  printAndDelay("bpAdjPlus", bpAdjPlus);
  printAndDelay("bpAdjMoins", bpAdjMoins);

  if ((interSelecteurMode) == LOW) {  // Selecteur Mode
    modeManuel = false;               // Mode manuel off
    modeAuto = true;                  // Mode auto on
    delay(250);
  } else if ((interSelecteurMode) == HIGH) {  // Selecteur Mode
    modeManuel = true;                        // Mode manuel on
    modeAuto = false;                         // Mode auto off
    delay(250);
  }

  if ((interSelecteurModeAdj) == LOW) {  // Selecteur modeAdj
    modeAdj = false;                     // Mode Adj off
    delay(250);
  } else if ((interSelecteurModeAdj) == HIGH) {  // Selecteur modeAdj
    modeAdj = true;                              // Mode Adj on
    delay(250);
  }


  if (modeManuel) {
    if ((boutonPoussoirPompe1) == LOW) {  // boutonPoussoirPompe1
      digitalWrite(pompe1Pin, LOW);       // pompe1Pin
      etatPompe1Pin = false;
    } else if ((boutonPoussoirPompe1) == HIGH) {  // boutonPoussoirPompe1 {
      digitalWrite(pompe1Pin, HIGH);              // pompe1Pin
      etatPompe1Pin = true;
    }

    if ((boutonPoussoirPompe2) == LOW) {  // boutonPoussoirPompe2
      digitalWrite(pompe2Pin, LOW);       // pompe2Pin
      etatPompe2Pin = false;
    } else if ((boutonPoussoirPompe2) == HIGH) {  // boutonPoussoirPompe2
      digitalWrite(pompe2Pin, HIGH);              // pompe2Pin
      etatPompe2Pin = true;
    }
  }
}