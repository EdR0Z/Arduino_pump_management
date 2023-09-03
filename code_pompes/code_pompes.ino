#include <EEPROM.h>             // Bibliothèque pour l'accès à l'EEPROM
#include <Wire.h>               // Bibliothèque pour la communication I2C
#include <LiquidCrystal_I2C.h>  // Bibliothèque pour les écrans LCD I2C
#include <NewPing.h>            // Bibliothèque pour le capteur ultrasonique

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
const int seuilBasP1 = 17;
const int shP1address = 0;
const int shP2address = 1;
const int seuilBasP2 = 14;
const int MAX_DISTANCE = 40;
int seuilHautP1 = 0;
int seuilHautP2 = 0;
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
int distance;
long duration;
String etatPompe1Str = "";
String etatPompe2Str = "";
String modeStr = "";
LiquidCrystal_I2C lcd(0x27, 20, 4);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  Wire.begin();
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
  digitalWrite(pompe1Pin, HIGH);
  etatPompe1Pin = 0;
  digitalWrite(pompe2Pin, HIGH);
  etatPompe2Pin = 0;
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
  delay(2000);
  lcd.clear();
}

void loop() {
  // Attente courte pour éviter de saturer la boucle
  delay(delaimesure * 1000);

  // Mesure de la distance avec le capteur ultrasonique
  duration = sonar.ping_median(5);  // Mesure de la distance médiane à partir de 5 échantillons
  if (duration == 0) {
    // Gestion de l'erreur si la mesure est invalide (distance de 0)
    distance = -1;  // Définit la distance à -1 pour indiquer une erreur
  } else {
    distance = duration * 0.034 / 2;
  }
  if (distance == -1) {
    lcd.setCursor(0, 2);
    lcd.print("Erreur lecture capteur");
  }

  // Affichage des informations sur l'écran LCD
  lcd.setCursor(0, 0);
  lcd.print(modeStr);
  lcd.setCursor(0, 1);
  lcd.print(etatPompe1Str + " " + etatPompe2Str);
  lcd.setCursor(0, 3);
  lcd.print("Distance: ");
  if (distance < 0) {
    lcd.setCursor(10, 3);
    lcd.print("Err");
  } else if (distance < 10) {
    lcd.setCursor(10, 3);
    lcd.print(0);
    lcd.print(distance);
  } else {
    lcd.setCursor(10, 3);
    lcd.print(distance);
  }

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
  }

  // Logique pour ajuster le seuil de la pompe 2
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
  }

  if ((bpAdjPlus) && (bpAdjMoins)) {
    // Écriture du seuil ajusté dans l'EEPROM
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Write EEPROM...");
    EEPROM.write(shP1address, seuilHautP1);
    int shP1readValue = EEPROM.read(shP1address);
    if (shP1readValue != seuilHautP1) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Write error shP1");
    }
    EEPROM.write(shP2address, seuilHautP2);
    int shP2readValue = EEPROM.read(shP2address);
    if (shP2readValue != seuilHautP2) {
      lcd.clear();
      lcd.setCursor(0, 2);
      lcd.print("Write error shP2");
    }
    delay(1000);
    lcd.setCursor(0, 3);
    lcd.print("Write successfull!");
    delay(1500);
    lcd.clear();
  }

  // Logique pour le mode automatique
  if ((modeAuto) && (!modeManuel)) {
    lcd.setCursor(0, 2);
    lcd.print("            ");
    modeStr = "Mode: Auto  ";

    // Logique pour activer/désactiver la pompe 1 en fonction de la distance
    if (distance <= seuilHautP1) {
      digitalWrite(pompe1Pin, LOW);
      etatPompe1Str = "P1: ON ";
      etatPompe1Pin = 1;
    } else if (distance >= seuilBasP1) {
      digitalWrite(pompe1Pin, HIGH);
      etatPompe1Str = "P1: OFF";
      etatPompe1Pin = 0;
    }

    // Logique pour activer/désactiver la pompe 2 en fonction de la distance
    if (distance <= seuilHautP2) {
      digitalWrite(pompe2Pin, LOW);
      etatPompe2Str = "P2: ON ";
      etatPompe2Pin = 1;
    } else if (distance >= seuilBasP2) {
      digitalWrite(pompe2Pin, HIGH);
      etatPompe2Str = "P2: OFF";
      etatPompe2Pin = 0;
    }
  }
}
