#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// Broches
const int pompe1Pin = 2;
const int pompe2Pin = 3;

const int boutonPoussoirPompe1 = 4;
const int boutonpPoussoirPompe2 = 5;

const int interSelecteurMode = 6;
const int interSelecteurModeAdj = 7;

const int boutonPoussoirAdjPlus = 8;
const int boutonPoussoirAdjMoins = 9;

const int trigger = 10;
const int echo = 11;

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
boolean modeAuto = false;
boolean modeAdj = false;

// Modules externes
LiquidCrystal_I2C lcd(0x27, 16, 2);
NewPing sonar(trigger, echo, max_distance);

void setup() {
  pinMode(pompe1Pin, OUTPUT);
  pinMode(pompe2Pin, OUTPUT);

  pinMode(bpPompe1, INPUT_PULLUP);
  pinMode(bpPompe2, INPUT_PULLUP);

  pinMode(selecteurMode, INPUT_PULLUP);
  pinMode(selecteurModeAdj, INPUT_PULLUP);

  pinMode(bpAdjPlus, INPUT_PULLUP);
  pinMode(bpAdjMoins, INPUT_PULLUP);

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  // Les broches sont LOW au démarrage
  digitalWrite(pompe1Pin, LOW);
  digitalWrite(pompe2Pin, LOW);

  digitalWrite(bpPompe1, LOW);
  digitalWrite(bpPompe2, LOW);

  digitalWrite(selecteurMode, LOW);
  digitalWrite(selecteurModeAdj, LOW);

  digitalWrite(bpAdjPlus, LOW);
  digitalWrite(bpAdjMoins, LOW);

  digitalWrite(LED_BUILTIN, LOW);

  digitalWrite(trigger, LOW);
  // Initialisation des fonctions du lcd
  lcd.init();
  lcd.backlight();
}

void loop() {

  distance = sonar.ping_cm();

  if (selecteurMode == LOW) {
    modeManuel = true;
    modeAuto = false;
    modeAdj = false;
  } else if (selecteurMode == HIGH) {
    modeManuel = false;
    modeAuto = true;
    modeAdj = false;
  } else if ((selecteurMode + selecteurModeAdj) == HIGH) {
      modeManuel = false;
      modeAuto = false;
      modeAdj = true;
    }

  if (modeAuto) {
    if (distance <= seuilPompe1) {
      pompe1Pin, true;
      etatPompe1Pin = true;
    } else {
      pompe1Pin, false;
      etatPompe1Pin = false;
    }

    if (distance <= seuilPompe2) {
      pompe2Pin, true;
      etatPompe2Pin = true;
    } else {
      pompe2Pin, false;
      etatPompe2Pin = false;
    }
  }
}
