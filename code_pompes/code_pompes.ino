#include <EEPROM.h>             // Bibliothèque pour l'accès à l'EEPROM
#include <Wire.h>               // Bibliothèque pour la communication I2C
#include <LiquidCrystal_I2C.h>  // Bibliothèque pour les écrans LCD I2C
#include <NewPing.h>            // Bibliothèque pour le capteur ultrasonique

// Déclaration des broches matérielles
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
const int delaimesure = 1;

// Adresses EEPROM pour le stockage de données
const int shP1address = 0;
const int shP2address = 1;

// Variables pour les seuils de déclenchement des pompes
int seuilHautP1 = 1;
const int seuilBasP1 = 25;
int seuilHautP2 = 1;
const int seuilBasP2 = 23;

// Constante pour la distance maximale du capteur ultrasonique
const int MAX_DISTANCE = 40;
String etatPompe1Str = "";
String etatPompe2Str = "";
String modeStr = "";
long duration;
int distance;

// Variables de contrôle et d'état
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

// Initialisation des modules externes
LiquidCrystal_I2C lcd(0x27, 20, 4);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  // Initialisation de la communication série
  Serial.begin(9600);

  // Initialisation des écrans LCD
  lcd.init();
  lcd.backlight();

  // Configuration des broches en entrée ou en sortie
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

  // Lecture des seuils à partir de l'EEPROM et affichage
  int shP1readValue = EEPROM.read(shP1address);
  seuilHautP1 = shP1readValue;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Read shP1: ");
  lcd.setCursor(11, 0);
  lcd.print(shP1readValue);

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
  // Attente courte pour éviter de saturer la boucle
  delay(delaimesure * 1000);

  // Affichage des informations sur l'écran LCD
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

  // Mesure de la distance avec le capteur ultrasonique
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Lecture des états des sélecteurs de mode
  selecteurMode = digitalRead(interSelecteurMode);
  selecteurModeAdj = digitalRead(interSelecteurModeAdj);

  // Logique de sélection du mode (manuel ou automatique)
  if (selecteurMode == LOW) {
    modeManuel = 0;
    modeAuto = 1;
  } else if (selecteurMode == HIGH) {
    modeManuel = 1;
    modeAuto = 0;
  }

  // Logique de sélection du mode d'ajustement
  if (selecteurModeAdj == LOW) {
    modeAdj = 0;
  } else if (selecteurModeAdj == HIGH) {
    modeAdj = 1;
  }

  // Logique pour le mode manuel sans ajustement
  if ((modeManuel) && (!modeAdj)) {
    bpPompe1 = digitalRead(boutonPoussoirPompe1);
    bpPompe2 = digitalRead(boutonPoussoirPompe2);

    lcd.setCursor(0, 2);
    lcd.print("            ");

    modeStr = "Mode: Manuel";

    if (bpPompe1 == 0) {
      digitalWrite(pompe1Pin, HIGH);
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    } else if (bpPompe1 == 1) {
      digitalWrite(pompe1Pin, LOW);
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    }

    if (bpPompe2 == 0) {
      digitalWrite(pompe2Pin, HIGH);
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    } else if (bpPompe2 == 1) {
      digitalWrite(pompe2Pin, LOW);
      etatPompe2Str = "P2: ON ";
      etatPompe2Pin = 1;
    }
  }

  // Logique pour le mode manuel avec ajustement
  if ((modeManuel) && (modeAdj)) {
    bpAdjPlus = digitalRead(boutonPoussoirAdjPlus);
    bpAdjMoins = digitalRead(boutonPoussoirAdjMoins);
    bpPompe1 = digitalRead(boutonPoussoirPompe1);
    bpPompe2 = digitalRead(boutonPoussoirPompe2);
    digitalWrite(pompe1Pin, HIGH);
    etatPompe1Pin = 0;
    digitalWrite(pompe2Pin, HIGH);
    etatPompe2Pin = 0;

    modeStr = "Mode: Adjust";

    // Logique pour ajuster le seuil de la pompe 1
    if ((bpAdjPlus) && (bpPompe1)) {
      lcd.setCursor(0, 2);
      lcd.print("Seuil P1: ");
      lcd.setCursor(10, 2);
      if (seuilHautP1 < 10) {
        lcd.print(0);
      }
      lcd.print(seuilHautP1);
      seuilHautP1 = max(0, seuilHautP1 + 1);  // Augmenter le seuil en s'assurant qu'il reste positif
    } else if ((bpAdjMoins) && (bpPompe1)) {
      lcd.setCursor(0, 2);
      lcd.print("Seuil P1: ");
      lcd.setCursor(10, 2);
      if (seuilHautP1 < 10) {
        lcd.print(0);
      }
      lcd.print(seuilHautP1);
      seuilHautP1 = max(0, seuilHautP1 - 1);  // Diminuer le seuil en s'assurant qu'il reste positif
    }

    else if ((bpAdjPlus == HIGH) && (bpAdjMoins == HIGH)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Writing EEPROM...");
      lcd.setCursor(0, 1);
      lcd.print("Write shP1, shP2");
      EEPROM.write(shP1address, seuilHautP1 + 1);  // Écriture d'un octet à l'adresse spécifiée
      EEPROM.write(shP2address, seuilHautP2 + 1);  // Écriture d'un octet à l'adresse spécifiée
      lcd.setCursor(0, 2);
      lcd.print("Write succefull!");
      delay(5000);
      lcd.clear();
    }
  }

  // Logique pour ajuster le seuil de la pompe 2
  if ((bpAdjPlus) && (bpPompe2)) {
    lcd.setCursor(0, 2);
    lcd.print("Seuil P2: ");
    lcd.setCursor(10, 2);
    if (seuilHautP2 < 10) {
      lcd.print(0);
    }
    lcd.print(seuilHautP2);
    seuilHautP2 = max(0, seuilHautP2 + 1);  // Augmenter le seuil en s'assurant qu'il reste positif
  } else if ((bpAdjMoins) && (bpPompe2)) {
    lcd.setCursor(0, 2);
    lcd.print("Seuil P2: ");
    lcd.setCursor(10, 2);
    if (seuilHautP2 < 10) {
      lcd.print(0);
    }
    lcd.print(seuilHautP2);
    seuilHautP2 = max(0, seuilHautP2 - 1);  // Diminuer le seuil en s'assurant qu'il reste positif
  }

  // Logique pour le mode automatique
  if ((modeAuto) && (!modeManuel)) {
    lcd.setCursor(0, 2);
    lcd.print("            ");
    modeStr = "Mode: Auto  ";

    // Logique pour activer/désactiver la pompe 1 en fonction de la distance
    if (distance <= seuilHautP1) {
      digitalWrite(pompe1Pin, HIGH);
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    } else if (distance >= seuilBasP1) {
      digitalWrite(pompe1Pin, LOW);
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    }

    // Logique pour activer/désactiver la pompe 2 en fonction de la distance
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