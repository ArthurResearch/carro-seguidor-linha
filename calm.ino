//Variáveis para sensores de linha e velocidade

int sensorDD, sensorDM, sensorMD, sensorME, sensorEM, sensorEE; // Sensores - D = Direita, M = Meio, E = Esquerda
int velMin = 70, velMed = 70, velMax = 185; // Velocidade dos motores - 0 a 255
int limiar = 600; // Limiar do sensor de luz - acima de 600 = preto, abaixo de 600 = branco

//Variáveis para sensores de início e fim de pista

int sensorFim; // Detecta o fim do percurso
int contaFim = 4; // Conta cada marca do percurso
boolean flagFim = false; // Conta uma vez só ao invés de contar a todo momento que estiver em cima da marca
long tempoTotal; // Detecta tempo desde que o arduino foi ligado
long tempoExtra = 2000; // Tempo extra para funcionar depois da última marca do percurso

// Definições dos pinos do Arduino ligados a entrada da Ponte H

// Motor Esquerdo
int ENA = 10;
int IN1 = 7;
int IN2 = 6;

// Motor Direito
int ENB = 9;
int IN3 = 5;
int IN4 = 4;

// Define os pinos como saida e como entrada

void setup(){
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);  
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);
  pinMode(ENA,  OUTPUT);  
  pinMode(ENB,  OUTPUT); 

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT); 

  Serial.begin(9600);
}

// Controle dos motores

/*
void controlaMotores(int velE, int velD) {
  analogWrite(ENA, velE);
  analogWrite(ENB, velD);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
*/

void frente(int vel) {
  //esquerdo
  analogWrite(ENA, vel);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  //direito
  analogWrite(ENB, vel);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void direita(int vel) {
  //esquerdo
  analogWrite(ENA, vel);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  //direito
  analogWrite(ENB, vel);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void esquerda(int vel) {
  //esquerdo
  analogWrite(ENA, vel);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  //direito
  analogWrite(ENB, vel);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void paraMotores() {
  // Motor Esquerdo para
  digitalWrite(IN1, HIGH); //E
  digitalWrite(IN2, HIGH); //E

  // Motor Direito para
  digitalWrite(IN3, HIGH); //D
  digitalWrite(IN4, HIGH); //D
}

void segueLinha(){
  sensorEE = analogRead(A0);
  sensorEM = analogRead(A1);
  sensorME = analogRead(A2);
  sensorMD = analogRead(A3);
  sensorDM = analogRead(A4);
  sensorDD = analogRead(A5);

  if (sensorME < limiar || sensorMD < limiar){
    Serial.println("para_frente");
    frente(velMed);    
  }
  if (sensorEM < limiar || sensorEE < limiar){
    Serial.println("para_esquerda");
    esquerda(velMed);    
  }
  if (sensorDM < limiar || sensorDD < limiar){
    Serial.println("para_direita");
    direita(velMed);
  }

  /*
  if (/* esquerda forte) {
    controlaMotores(velMin, velMax);
  }
  else if (/* direita forte) {
    controlaMotores(velMax, velMin);
  }
  else if (/* esquerda) {
    controlaMotores(velMed, velMax);
  }
  else if (/* direita) {
    controlaMotores(velMax, velMed);
  }
  else if (/* centro) {
    controlaMotores(velMax, velMax);
  }
  */
  // Teste dos sensores
  /*
  Serial.print(sensorEE);
  Serial.print(" ");
  Serial.print(sensorEM);
  Serial.print(" ");
  Serial.print(sensorME);
  Serial.print(" ");
  Serial.print(sensorMD);
  Serial.print(" ");
  Serial.print(sensorDM);
  Serial.print(" ");
  Serial.println(sensorDD);

  delay(100);
  */
}
 
void loop(){
  //Segue linha até encontrar ENArca de fim de pista
  while(contaFim > 0){
    segueLinha();
    
    /*
    if (sensorFim < limiar)
       flagFim = true; 
    if (sensorFim > limiar && flagFim == true){
       flagFim=false;
       contaFim--;
    }
    */
  }

 //Segue linha por ENAis 2 segundos antes de parar
 tempoTotal = millis();
 while((millis() - tempoTotal) < tempoExtra) {
   segueLinha();
}

 //Pára motores
 paraMotores();

 //Espera 10s
 delay(10000);
 }
