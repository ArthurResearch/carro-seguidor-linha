#include <QTRSensors.h>

QTRSensors qtr;

const uint8_t SensorCount = 6;

const uint8_t sensorPins[SensorCount] = {
  A0, A1, A2, A3, A4, A5
};

uint16_t sensorValues[SensorCount];


// ================================
// VELOCIDADES
// ================================

int velFrente = 80;
int velCurva = 80;


// ================================
// MOTORES
// ================================

// Motor esquerdo
const int ENA = 10;
const int IN1 = 7;
const int IN2 = 6;

// Motor direito
const int ENB = 9;
const int IN3 = 5;
const int IN4 = 4;


// ================================
// SETUP
// ================================

void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);


  qtr.setTypeRC();
  qtr.setSensorPins(sensorPins, SensorCount);


  // ==============================
  // CALIBRAÇÃO
  // ==============================

  Serial.println("Calibrando...");

  delay(1000);

  for (uint16_t i = 0; i < 250; i++) {

    qtr.calibrate();

    delay(20);
  }

  Serial.println("Calibracao concluida!");
}


// ================================
// FRENTE
// ================================

void frente(int velocidade) {

  analogWrite(ENA, velocidade);
  analogWrite(ENB, velocidade);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


// ================================
// ESQUERDA
// ================================

void esquerda(int velocidade) {

  // Motor esquerdo parado
  analogWrite(ENA, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);


  // Motor direito andando
  analogWrite(ENB, velocidade);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}


// ================================
// DIREITA
// ================================

void direita(int velocidade) {

  // Motor esquerdo andando
  analogWrite(ENA, velocidade);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);


  // Motor direito parado
  analogWrite(ENB, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// ================================
// PARAR
// ================================

void paraMotores() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


// ================================
// SEGUIR LINHA
// ================================

void segueLinha() {

  qtr.readCalibrated(sensorValues);


  // --------------------------------
  // BRANCO
  // --------------------------------

  bool esquerdaDetectou =
    sensorValues[0] > 1250 ||
    sensorValues[1] > 1250 ||
    sensorValues[2] > 1250;


  bool direitaDetectou =
    sensorValues[5] > 1250 ||
    sensorValues[6] > 1250 ||
    sensorValues[7] > 1250;


  // --------------------------------
  // DECISÃO
  // --------------------------------

  if (esquerdaDetectou && !direitaDetectou) {

    esquerda(velCurva);

  }

  else if (direitaDetectou && !esquerdaDetectou) {

    direita(velCurva);

  }

  else {

    // Nenhum branco
    // ou os dois lados detectaram branco

    frente(velFrente);
  }
}


// ================================
// LOOP
// ================================

void loop() {

  segueLinha();

}
