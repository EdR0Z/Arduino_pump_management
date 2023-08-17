#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// Broches
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

// Variables
int seuilPompe1 = 5;
int seuilPompe2 = 15;
const int MAX_DISTANCE = 400;
String etatPompe1Str = "";
String etatPompe2Str = "";
String modeStr = "";

// États
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
int distance = 0;

// Modules externes
LiquidCrystal_I2C lcd(0x27, 20, 4);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(LED_BUILTIN, OUTPUT);
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
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TRIGGER_PIN, LOW);


  digitalWrite(LED_BUILTIN, HIGH);
  delay(30);
  digitalWrite(LED_BUILTIN, LOW);
  delay(30);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(30);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(modeStr);

  lcd.setCursor(0, 1);
  lcd.print(etatPompe1Str + " " + etatPompe2Str);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);

  distance = sonar.ping_cm();
  selecteurMode = digitalRead(interSelecteurMode);
  selecteurModeAdj = digitalRead(interSelecteurModeAdj);

  if ((selecteurMode) == 0) {
    modeManuel = 0;
    modeAuto = 1;
  } else if ((selecteurMode) == 1) {
    modeManuel = 1;
    modeAuto = 0;
  }

  if ((selecteurModeAdj) == 0) {
    modeAdj = 0;
  } else if ((selecteurModeAdj) == 1) {
    modeAdj = 1;
  }

  if ((modeManuel) && (!modeAdj)) {                // mode manu et pas mode d'ajustement
    bpPompe1 = digitalRead(boutonPoussoirPompe1);  //Lecture poussoir pompe 1
    bpPompe2 = digitalRead(boutonPoussoirPompe2);  //Lecture poussoir pompe 2
    modeStr = "Mode: Manuel";

    if ((bpPompe1) == 0) {           // boutonPoussoirPompe1 pas actionné
      digitalWrite(pompe1Pin, LOW);  // Pompe1 sur OFF pompe1Pin
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    } else if ((bpPompe1) == 1) {     // boutonPoussoirPompe1 actionné
      digitalWrite(pompe1Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    }

    if ((bpPompe2) == 0) {           // boutonPoussoirPompe1 pas actionné
      digitalWrite(pompe2Pin, LOW);  // Pompe1 sur OFF pompe1Pin
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    } else if ((bpPompe2) == 1) {     // boutonPoussoirPompe1 actionné
      digitalWrite(pompe2Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe2Str = "P2: ON";
      etatPompe2Pin = 1;
    }
  }

  if ((modeManuel) && (modeAdj)) {                     // mode manu et mode d'ajustement
    bpAdjPlus = digitalRead(boutonPoussoirAdjPlus);    //Lecture poussoir adj plus
    bpAdjMoins = digitalRead(boutonPoussoirAdjMoins);  //Lecture poussoir adj moins
    bpPompe1 = digitalRead(boutonPoussoirPompe1);      //Lecture poussoir pompe 1
    bpPompe2 = digitalRead(boutonPoussoirPompe2);      //Lecture poussoir pompe 2
    digitalWrite(pompe1Pin, LOW);                      // Pompe1 sur OFF pompe1Pin
    etatPompe1Pin = 0;
    digitalWrite(pompe2Pin, LOW);  // pompe2 sur OFF pompe2Pin
    etatPompe2Pin = 0;

    modeStr = "Mode: Adjust";

    if ((bpAdjPlus) && (bpPompe1)) {
      bpAdjPlus = 1;
      seuilPompe1 = seuilPompe1 + 1;
    } else if ((bpAdjMoins) && (bpPompe1)) {
      seuilPompe1 = seuilPompe1 - 1;
      bpAdjPlus = 0;
    }

    if ((bpAdjPlus) && (bpPompe2)) {
      bpAdjPlus = 1;
      seuilPompe2 = seuilPompe2 + 1;
    } else if ((bpAdjMoins) && (bpPompe2)) {
      seuilPompe2 = seuilPompe2 - 1;
      bpAdjPlus = 0;
    }
  }

  if ((modeAuto) && (!modeManuel)) {  // mode auto et pas mode manu
    modeStr = "Mode: Auto";

    if ((distance) <= (seuilPompe1)) {
      digitalWrite(pompe1Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe1Str = "P1: ON";
      etatPompe1Pin = 0;
    } else if ((distance) > (seuilPompe1)) {
      digitalWrite(pompe1Pin, LOW);  // Pompe1 sur ON pompe1Pin
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    }

    if ((distance) <= (seuilPompe2)) {
      digitalWrite(pompe2Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe2Str = "P2: ON";
      etatPompe2Pin = 1;
    } else if ((distance) > (seuilPompe2)) {
      digitalWrite(pompe2Pin, LOW);  // Pompe1 sur ON pompe1Pin
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    }
  }
}