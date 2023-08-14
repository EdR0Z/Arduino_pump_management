#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// Tableau broches
const int broches[10] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
/* Pour note tableau
pompe1Pin = 2;
pompe2Pin = 3;
boutonPoussoirPompe1 = 4;
boutonPoussoirPompe2 = 5;
interSelecteurMode = 6;
interSelecteurModeAdj = 7;
boutonPoussoirAdjPlus = 8;
boutonPoussoirAdjMoins = 9;
trigger = 10;
echo = 11;
*/

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
NewPing sonar(broches[8], broches[9], max_distance);

void setup() {
  pinMode(broches[0], OUTPUT);
  pinMode(broches[1], OUTPUT);
  pinMode(broches[2], INPUT_PULLUP);
  pinMode(broches[3], INPUT_PULLUP);
  pinMode(broches[4], INPUT_PULLUP);
  pinMode(broches[5], INPUT_PULLUP);
  pinMode(broches[6], INPUT_PULLUP);
  pinMode(broches[7], INPUT_PULLUP);
  pinMode(broches[8], OUTPUT);
  pinMode(broches[9], INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Les broches sont LOW au démarrage
  digitalWrite(broches[0], LOW);
  digitalWrite(broches[1], LOW);
  digitalWrite(broches[2], LOW);
  digitalWrite(broches[3], LOW);
  digitalWrite(broches[4], LOW);
  digitalWrite(broches[5], LOW);
  digitalWrite(broches[6], LOW);
  digitalWrite(broches[7], LOW);
  digitalWrite(broches[8], LOW);
  digitalWrite(LED_BUILTIN, LOW);
  // Initialisation des fonctions du lcd
  lcd.init();
  lcd.backlight();
}

void loop() {

  distance = sonar.ping_cm();



  if (digitalRead(broches[4]) == LOW) {  // Selecteur Mode
    modeManuel = false;
    delay(250);
  } else if (digitalRead(broches[4]) == HIGH) {
    modeManuel = true;  // Mode manuel
    delay(250);
  }

  if (digitalRead(broches[5]) == LOW) {  // Selecteur modeAdj
    modeAdj = false;
    delay(250);
  } else if (digitalRead(broches[5]) == HIGH) {
    modeAdj = true;
    delay(250);


    if (modeManuel) {
      if (broches[2] == LOW) {  // boutonPoussoirPompe1
        digitalWrite(broches[0], LOW);
        etatPompe1Pin = false;
      } else {
        digitalWrite(broches[0], HIGH);
        etatPompe1Pin = true;
      }

      if (broches[3] == LOW) {  // boutonPoussoirPompe2
        digitalWrite(broches[1], LOW);
        etatPompe2Pin = false;
      } else {
        digitalWrite(broches[1], HIGH);
        etatPompe2Pin = true;
      }
    }
  }
}