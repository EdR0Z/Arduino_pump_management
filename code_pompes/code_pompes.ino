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
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pompe1Pin, OUTPUT);
  pinMode(pompe2Pin, OUTPUT);
  pinMode(boutonPoussoirPompe1, INPUT);
  pinMode(boutonPoussoirPompe2, INPUT);
  pinMode(interSelecteurMode, INPUT);
  pinMode(interSelecteurModeAdj, INPUT);
  pinMode(boutonPoussoirAdjPlus, INPUT);
  pinMode(boutonPoussoirAdjMoins, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  // Broches capteur ultrasoons HC-SR04
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  // Les broches sont LOW au démarrage
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TRIGGER_PIN, LOW);  // La broche TRIGGER doit être à LOW au repos

  //  Séquence de démarrage
  digitalWrite(LED_BUILTIN, HIGH);
  delay(30);
  digitalWrite(LED_BUILTIN, LOW);
  delay(30);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(30);
  digitalWrite(LED_BUILTIN, LOW);
}

void printAndDelay(const char* label, int value) {
  Serial.print(label);
  Serial.print(":");
  Serial.print(value);
  Serial.println("");
  delay(100);
}

void loop() {

  //  Debug
  //printAndDelay("modeManuel", modeManuel);
  //printAndDelay("modeAuto", modeAuto);
  //printAndDelay("modeAdj", modeAdj);
  //printAndDelay("selecteurMode", selecteurMode);
  //printAndDelay("selecteurModeAdj", selecteurModeAdj);
  //printAndDelay("etatPompe1Pin", etatPompe1Pin);
  //printAndDelay("etatPompe2Pin", etatPompe2Pin);
  //printAndDelay("bpPompe1", bpPompe1);
  //printAndDelay("bpPompe2", bpPompe2);
  //printAndDelay("bpAdjPlus", bpAdjPlus);
  //printAndDelay("bpAdjMoins", bpAdjMoins);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);

  distance = sonar.ping_cm();                       // Lire la distance en mode automatique
  selecteurMode = digitalRead(interSelecteurMode);  //Lecture sélecteur mode
  selecteurModeAdj = digitalRead(interSelecteurModeAdj);

  if ((selecteurMode) == LOW) {          // Selecteur de Mode
    modeManuel = 0;                      // Mode manuel off
    modeAuto = 1;                        // Mode auto on
  } else if ((selecteurMode) == HIGH) {  // Selecteur Mode
    modeManuel = 1;                      // Mode manuel on
    modeAuto = 0;                        // Mode auto off
  }

  if ((selecteurModeAdj) == LOW) {          // Selecteur modeAdj
    modeAdj = 0;                            // Mode Adj off
  } else if ((selecteurModeAdj) == HIGH) {  // Selecteur modeAdj
    modeAdj = 1;                            // Mode Adj on
  }

  if ((modeManuel) && (!modeAdj)) {                // mode manu et pas mode d'ajustement
    bpPompe1 = digitalRead(boutonPoussoirPompe1);  //Lecture poussoir pompe 1
    bpPompe2 = digitalRead(boutonPoussoirPompe2);  //Lecture poussoir pompe 2
    if ((bpPompe1) == 0) {                         // boutonPoussoirPompe1 pas actionné
      digitalWrite(pompe1Pin, LOW);                // Pompe1 sur OFF pompe1Pin
      etatPompe1Pin = 0;
    } else if ((bpPompe1) == 1) {     // boutonPoussoirPompe1 actionné
      digitalWrite(pompe1Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe1Pin = 1;
    }

    if ((bpPompe2) == LOW) {         // boutonPoussoirPompe2
      digitalWrite(pompe2Pin, LOW);  // pompe2Pin
      etatPompe2Pin = 0;
    } else if ((bpPompe2) == HIGH)  // boutonPoussoirPompe2
    {
      digitalWrite(pompe2Pin, HIGH);  // pompe2Pin
      etatPompe2Pin = 1;
    }
  }

  if ((modeManuel) && (modeAdj)) {                     // mode manu et mode d'ajustement
    bpAdjPlus = digitalRead(boutonPoussoirAdjPlus);    //Lecture poussoir adj plus
    bpAdjMoins = digitalRead(boutonPoussoirAdjMoins);  //Lecture poussoir adj moins

    bpPompe1 = digitalRead(boutonPoussoirPompe1);  //Lecture poussoir pompe 1
    bpPompe2 = digitalRead(boutonPoussoirPompe2);  //Lecture poussoir pompe 2

    digitalWrite(pompe1Pin, LOW);  // Pompe1 sur OFF pompe1Pin
    etatPompe1Pin = 0;
    digitalWrite(pompe2Pin, LOW);  // pompe2 sur OFF pompe2Pin
    etatPompe2Pin = 0;

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

  if ((modeAuto) && (!modeManuel) && (!modeAdj)) {  // mode auto et pas mode manu ni adj
    if ((distance) <= (seuilPompe1)) {
      digitalWrite(pompe1Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe1Pin = 1;
    } else if ((distance) > (seuilPompe1)) {
      digitalWrite(pompe1Pin, LOW);  // Pompe1 sur ON pompe1Pin
      etatPompe1Pin = 0;
    }
  }

  if ((distance) <= (seuilPompe2)) {
    digitalWrite(pompe2Pin, HIGH);  // Pompe1 sur ON pompe1Pin
    etatPompe2Pin = 1;
  } else if ((distance) > (seuilPompe2)) {
    digitalWrite(pompe2Pin, LOW);  // Pompe1 sur ON pompe1Pin
    etatPompe2Pin = 0;
  }
}