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
  pinMode(broches[2], INPUT);
  pinMode(broches[3], INPUT);
  pinMode(broches[4], INPUT);
  pinMode(broches[5], INPUT);
  pinMode(broches[6], INPUT);
  pinMode(broches[7], INPUT);
  pinMode(broches[8], OUTPUT);
  pinMode(broches[9], INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Les broches sont LOW au démarrage
  digitalWrite(LED_BUILTIN, LOW);

  // Initialisation des fonctions du lcd
  lcd.init();
  lcd.backlight();
}

void loop() {

  distance = sonar.ping_cm();

  Serial.println("modeManuel");
  Serial.println(modeManuel);
  Serial.println("------");
  delay(100);

  Serial.println("modeAuto");
  Serial.println(modeAuto);
  Serial.println("------");
  delay(100);

  Serial.println("modeAdj");
  Serial.println(modeAdj);
  Serial.println("------");
  delay(100);

  if (digitalRead(broches[4]) == LOW) {  // Selecteur Mode
    modeManuel = false;                  // Mode manuel off
    modeAuto = true;                     // Mode auto on
    delay(250);
  } else {
    modeManuel = true;  // Mode manuel on
    modeAuto = false;   // Mode auto off
    delay(250);
  }

  if (digitalRead(broches[5]) == LOW) {  // Selecteur modeAdj
    modeAdj = false;
    delay(250);
  } else {
    modeAdj = true;
    delay(250);
  }


  if (modeManuel) {
    if (broches[2] == LOW) {          // boutonPoussoirPompe1
      digitalWrite(broches[0], LOW);  // pompe1Pin
      etatPompe1Pin = false;
    } else {
      digitalWrite(broches[0], HIGH);  // pompe1Pin
      etatPompe1Pin = true;
    }

    if (broches[3] == LOW) {          // boutonPoussoirPompe2
      digitalWrite(broches[1], LOW);  // pompe2Pin
      etatPompe2Pin = false;
    } else {
      digitalWrite(broches[1], HIGH);  // pompe2Pin
      etatPompe2Pin = true;
    }
  }
}