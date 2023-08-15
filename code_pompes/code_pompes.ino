// Broches
const int pompe1Pin = 2;
const int pompe2Pin = 3;
const int boutonPoussoirPompe1 = 4;
const int boutonPoussoirPompe2 = 5;
const int interSelecteurMode = 6;
const int interSelecteurModeAdj = 7;
const int boutonPoussoirAdjPlus = 8;
const int boutonPoussoirAdjMoins = 9;

// Variables
const int seuilPompe1 = 50;
const int seuilPompe2 = 30;
const int max_distance = 400;

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

// Modules externes

void setup() {
  pinMode(pompe1Pin, OUTPUT);
  pinMode(pompe2Pin, OUTPUT);
  pinMode(boutonPoussoirPompe1, INPUT);
  pinMode(boutonPoussoirPompe2, INPUT);
  pinMode(interSelecteurMode, INPUT);
  pinMode(interSelecteurModeAdj, INPUT);
  pinMode(boutonPoussoirAdjPlus, INPUT);
  pinMode(boutonPoussoirAdjMoins, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // Les broches sont LOW au démarrage
  digitalWrite(LED_BUILTIN, LOW);

  // Initialisation des fonctions du lcd
}

void loop() {


  selecteurMode = digitalRead(interSelecteurMode);  //Lecture sélecteur mode
  selecteurModeAdj = digitalRead(interSelecteurModeAdj);


  if ((selecteurMode) == LOW)  // Selecteur de Mode
  {
    modeManuel = 0;                    // Mode manuel off
    modeAuto = 1;                      // Mode auto on
    //delay(250);
  } else if ((selecteurMode) == HIGH)  // Selecteur Mode
  {
    modeManuel = 1;  // Mode manuel on
    modeAuto = 0;    // Mode auto off
    //delay(250);
  }

  if ((interSelecteurModeAdj) == LOW)  // Selecteur modeAdj
  {
    modeAdj = 0;  // Mode Adj off
    //delay(250);
  } else if ((interSelecteurModeAdj) == HIGH)  // Selecteur modeAdj
  {
    modeAdj = 1;  // Mode Adj on
   //delay(250);
  }

  if (modeManuel) {

    bpPompe1 = digitalRead(boutonPoussoirPompe1);  //Lecture poussoir pompe 1
    bpPompe2 = digitalRead(boutonPoussoirPompe2);  //Lecture poussoir pompe 2
    if ((bpPompe1) == 0)                           // boutonPoussoirPompe1 pas actionné
    {
      digitalWrite(pompe1Pin, LOW);  // Pompe1 sur OFF pompe1Pin
      etatPompe1Pin = 0;
    } else if ((bpPompe1) == 1)  // boutonPoussoirPompe1 actionné
    {
      digitalWrite(pompe1Pin, HIGH);  // Pompe1 sur ON pompe1Pin
      etatPompe1Pin = 1;
    }

    if ((bpPompe2) == LOW)  // boutonPoussoirPompe2
    {
      digitalWrite(pompe2Pin, LOW);  // pompe2Pin
      etatPompe2Pin = 0;
    } else if ((bpPompe2) == HIGH)  // boutonPoussoirPompe2
    {
      digitalWrite(pompe2Pin, HIGH);  // pompe2Pin
      etatPompe2Pin = 1;
    }
  }
}