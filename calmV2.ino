#include <QTRSensors.h>

// =====================================================
// QTR-8RC / SENSOR DE LINHA
// =====================================================

QTRSensors qtr;

const uint8_t SensorCount = 6;

// Ordem: esquerda -> direita
const uint8_t sensorPins[SensorCount] = {
  A0, A1, A2, A3, A4, A5
};

uint16_t sensorValues[SensorCount];


// =====================================================
// VELOCIDADES
// =====================================================

int velMin = 130;
int velMed = 100;
int velMax = 130;


// =====================================================
// MARCAS DE FIM DE PISTA
// =====================================================

int sensorFim;
int contaFim = 4;

bool flagFim = false;

unsigned long tempoTotal;
unsigned long tempoExtra = 2000;


// =====================================================
// MOTORES
// =====================================================

// Motor esquerdo
const int ENA = 10;
const int IN1 = 7;
const int IN2 = 6;

// Motor direito
const int ENB = 9;
const int IN3 = 5;
const int IN4 = 4;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  // -------------------------------
  // Motores
  // -------------------------------

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);


  // -------------------------------
  // QTR
  // -------------------------------

  qtr.setTypeRC();
  qtr.setSensorPins(sensorPins, SensorCount);

  // =================================================
  // CALIBRAÇÃO
  // =================================================

  Serial.println();
  Serial.println("================================");
  Serial.println("CALIBRACAO DOS SENSORES");
  Serial.println("================================");
  Serial.println();

  Serial.println("Movimente os sensores sobre");
  Serial.println("a linha BRANCA e o fundo PRETO.");
  Serial.println();

  delay(2000);


  // Aproximadamente 5 segundos
  for (uint16_t i = 0; i < 250; i++) {

    qtr.calibrate();

    delay(20);
  }


  Serial.println();
  Serial.println("Calibracao concluida!");
  Serial.println();

  // Mostra os valores mínimos e máximos
  mostrarCalibracao();

  delay(2000);
}


// =====================================================
// MOSTRA DADOS DA CALIBRAÇÃO
// =====================================================

void mostrarCalibracao() {

  Serial.println("Sensor | Min | Max");
  Serial.println("-------------------");

  for (uint8_t i = 0; i < SensorCount; i++) {

    Serial.print(i);
    Serial.print("      | ");

    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(" | ");

    Serial.println(qtr.calibrationOn.maximum[i]);
  }

  Serial.println();
}


// =====================================================
// MOTOR PARA FRENTE
// =====================================================

void frente(int vel) {

  // Motor esquerdo
  analogWrite(ENA, vel);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);


  // Motor direito
  analogWrite(ENB, vel);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


// =====================================================
// CURVA PARA DIREITA
// =====================================================

void direita(int vel) {

  // Motor esquerdo
  analogWrite(ENA, vel);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);


  // Motor direito parado
  analogWrite(ENB, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// =====================================================
// CURVA PARA ESQUERDA
// =====================================================

void esquerda(int vel) {

  // Motor esquerdo parado
  analogWrite(ENA, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);


  // Motor direito
  analogWrite(ENB, vel);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


// =====================================================
// PARA OS MOTORES
// =====================================================

void paraMotores() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// =====================================================
// LEITURA DOS SENSORES
// =====================================================

void lerSensores() {

  qtr.readCalibrated(sensorValues);
}


// =====================================================
// MOSTRA OS SENSORES NO SERIAL
// =====================================================

void mostrarSensores() {

  lerSensores();

  for (uint8_t i = 0; i < SensorCount; i++) {

    Serial.print(sensorValues[i]);

    if (i < SensorCount - 1) {
      Serial.print(" | ");
    }
  }

  Serial.println();
}


// =====================================================
// SEGUIR LINHA
// =====================================================

void segueLinha() {

  lerSensores();


  // -------------------------------------------------
  // Sensores:
  //
  // 0  1  2  3  4  5
  // E  E  M  M  D  D
  //
  // Linha BRANCA
  // -------------------------------------------------


  // Centro
  if (sensorValues[2] < 500 ||
      sensorValues[3] < 500) {

    frente(velMed);

    return;
  }


  // Esquerda
  if (sensorValues[0] < 500 ||
      sensorValues[1] < 500) {

    esquerda(velMed);

    return;
  }


  // Direita
  if (sensorValues[4] < 500 ||
      sensorValues[5] < 500) {

    direita(velMed);

    return;
  }


  // -------------------------------------------------
  // Nenhum sensor detectou a linha
  // -------------------------------------------------

  paraMotores();
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // =================================================
  // SEGUE A LINHA ATÉ AS MARCAS
  // =================================================

  while (contaFim > 0) {

    segueLinha();

    // ------------------------------------------------
    // Aqui entrará posteriormente a leitura do sensor
    // responsável pelas marcas de fim.
    // ------------------------------------------------
  }


  // =================================================
  // SEGUE POR MAIS 2 SEGUNDOS
  // =================================================

  tempoTotal = millis();

  while ((millis() - tempoTotal) < tempoExtra) {

    segueLinha();
  }


  // =================================================
  // PARA
  // =================================================

  paraMotores();


  // =================================================
  // ESPERA 10 SEGUNDOS
  // =================================================

  delay(10000);
}
