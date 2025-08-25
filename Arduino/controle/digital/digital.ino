//#include <Ultrasonic.h>

uint8_t IR_Pins[5] = {4, A8, A10, A5, A7}; //pinos dos sensores infravermelhos
uint8_t Motor[4] = {22, 23, 24, 25}; //pinos dos motores (ponte H)
uint8_t Posicao[4] = {0, 0, 0, 0}; 
//Uma matriz para guardar onde está cada área para colocar os mortos e vivos, sendo posicao[0] o vertice inferior direito, assim por diante
static bool D4_, A0_, A1_, A2_, A3_;
static bool state_motor;

//Ultrasonic Ultra_Drt(26, 27);
//Ultrasonic Ultra_Esq(28, 29);
//Ultrasonic Ultra_Mei(30, 31);
uint32_t time_Atual;
uint32_t time_Ant;

uint32_t timeZ;
float Z_Angulo;

#define PWMd 2
#define PWMe 3
#define RedPin A4
#define GreenPin A5
#define BluePin A6

int limiar_branco = 690;

int dist_Drt, dist_Esq, dist_Mei;


void setup() {
  Serial.begin(9600);

  for(uint8_t x = 0; x < 5; x++){
    pinMode(IR_Pins[x], INPUT);

    if(x < 4){
      pinMode(Motor[x], OUTPUT);
    }
  }

  analogWrite(PWMd, 75);
  analogWrite(PWMe, 75);

}

void loop() {
  time_Atual = millis();
  //imprimirLeituras();

  if((time_Atual - time_Ant) > 500){
    Parar_Motores(300);
    time_Ant = time_Atual;
  }

  D4_ = naLinha(0);
  A0_ = naLinha(1);
  A1_ = naLinha(2);
  A2_ = naLinha(3);
  A3_ = naLinha(4);

  /*if(D4_ && !A0_ && A1_ && !A2_ && !A3_){
    Serial.println("Girando direita");
    Parar_Motores(500);
    Mover_Direita();
    delay(200);
    Parar_Motores(500);
  }else if(D4_ && !A0_ && !A1_ && A2_ && !A3_){
    Serial.println("Girando");
    Parar_Motores(500);
    Mover_Esquerda();
    delay(200);
    Parar_Motores(500);
  }else if(D4_ && !A0_ && !A1_ && !A2_ && !A3_){
    Serial.println("Movendo para frente");
    Mover_Frente(1);
  }*/

  if(D4_ && A0_ && A1_ && A2_ && A3_){
    Mover_Frente(0);
  }
  
  if(D4_ && !A0_ && A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Frente(500);
    Parar_Motores(500);
    Mover_Direita();
    delay(200);
    Parar_Motores(500); 
  }
  
  if(D4_ && !A0_ && !A1_ && A2_ && !A3_){
    Parar_Motores(500);
    Mover_Frente(200);
    Parar_Motores(500);
    Mover_Esquerda();
    delay(200);
    Parar_Motores(500);
  }
  
  if(!D4_ && A0_ && A1_ && !A2_ && !A3_){
    Mover_90_Graus_Direita();
    Mover_Atras(500);
    Parar_Motores(500);
  }
  
  if(!D4_ && !A0_ && !A1_ && A2_ && A3_){
    Mover_90_Graus_Esquerda();
    Mover_Atras(500);
    Parar_Motores(500);
  }
  
  if(!D4_ && A0_ && A1_ && A2_ && A3_){
    Mover_90_Graus_Esquerda();
    Mover_Atras(500);
    Parar_Motores(500);
  }
  
  if(D4_ && A0_ && A1_ && !A2_ && !A3_){
    Mover_Frente(0);
  }
  
  if(D4_ && !A0_ && !A1_ && A2_ && A3_){
    Mover_Frente(0);
  }
  
  if(!D4_ && !A0_ && A1_ && A2_ && !A3_){
    Mover_90_Graus_Direita();
    Mover_Atras(500);
    Parar_Motores(500);
  }
  
  if(D4_ && !A0_ && !A1_ && !A2_ && !A3_){
    Mover_Frente(0);
  }

  if(!D4_ && !A0_ && !A1_ && !A2_ && !A3_){
    Mover_Frente(0);
  }

  if(!D4_ && !A0_ && A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Direita();
    delay(200);
    Parar_Motores(500);
  }

  if(!D4_ && !A0_ && !A1_ && A2_ && !A3_){
    Parar_Motores(500);
    Mover_Esquerda();
    delay(200);
    Parar_Motores(500);
  }

  if(!D4_ && A0_ && A1_ && A2_ && !A3_){
    Mover_90_Graus_Direita();
    Mover_Atras(500);
    Parar_Motores(500);
  }

  if(!D4_ && !A0_ && A1_ && A2_ && A3_){
    Mover_90_Graus_Esquerda();
    Mover_Atras(500);
    Parar_Motores(500);
  }

  if(D4_ && A0_ && A1_ && A2_ && !A3_){
    Mover_Frente(0);
  }

  if(D4_ && !A0_ && A1_ && A2_ && A3_){
    Mover_Frente(0);
  }

  delay(10);
  state_motor = !state_motor;

}

void Mover_Frente(int del){
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);
  delay(del);
}

void Mover_Esquerda(){
  analogWrite(PWMd, 70);
  analogWrite(PWMe, 70);
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);
}

void Mover_Direita(){
  analogWrite(PWMd, 70);
  analogWrite(PWMe, 70);
  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);
}

void Parar_Motores(int del) {
  for(int i=0;i<4;i++){
    digitalWrite(Motor[i], LOW);
  }
  delay(del);
}

void Mover_Atras(int del){
  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);
  delay(del);
}

void Mover_90_Graus_Esquerda(){
  Parar_Motores(500);
  Mover_Frente(200);
  Mover_Esquerda();
  delay(400);
}

void Mover_90_Graus_Direita(){
  Parar_Motores(500);
  Mover_Frente(200);
  Mover_Direita();
  delay(400);
}

void Virar_Ao_Contrario(){
}

void Mover_45_Graus_Esquerda(){
}

void Mover_45_Graus_Direita(){;
}


/*Mapear_Area(){
  int intensidade;
  Mover_90_Graus_Direita(); //Mover o robô de frente à parede para calcular as matrizes
  if(Dist_Mei > 5){
    while(Dist_Mei > 5){
      Mover_Frente(0);
    }
    Parar_Motores();
  }
  
  intensidade = analogRead(RedPin) + analogRead(GreenPin) + analogRead(BluePin);

  if(intensidade > limiar_branco){
    Posicao[0] = 0;
  }else if(analogRead(RedPin) > analogRead(GreenPin)){
    Posicao[0] = 1;
  }else{
    Posicao[0] = 2;
  }

  Mover_90_Graus_Esquerda();
  Mover_Frete(0);

  if(Dist_Drt > 10){
    Mover_90_Graus_Direita();
    Mover_Frente(0);
    Mover_90_Graus_Esquerda();
  }

  while(Dist_Mei > 5){
    Mover_Frente(0);
    bool A0_ = naLinha(IR_Pins[1]);
    bool A1_ = naLinha(IR_Pins[2]);
    bool A2_ = naLinha(IR_Pins[3]);
    bool A3_ = naLinha(IR_Pins[4]);

    if(A0_ && A1_ && A2_ && A3_){
      Mover_Atras(0);
    }
  }

  intensidade = analogRead(RedPin) + analogRead(GreenPin) + analogRead(BluePin);
  
  if(intensidade > limiar_branco){
    Posicao[1] = 0;
  }else if(analogRead(RedPin) > analogRead(GreenPin)){
    Posicao[1] = 1;
  }else{
    Posicao[1] = 2;
  }

  Mover_90_Graus_Esquerda();
  Mover_Frete(0);

  if(Dist_Drt > 10){
    Mover_90_Graus_Direita();
    Mover_Frente(0);
    Mover_90_Graus_Esquerda();
  }

  while(Dist_Mei > 5){
    Mover_Frente(0);
    if(
  }

  intensidade = analogRead(RedPin) + analogRead(GreenPin) + analogRead(BluePin);
  
  if(intensidade > limiar_branco){
    Posicao[2] = 0;
  }else if(analogRead(RedPin) > analogRead(GreenPin)){
    Posicao[2] = 1;
  }else{
    Posicao[2] = 2;
  }
  
}*/

bool naLinha(uint8_t index) {
  if (index == 0) {
    return digitalRead(IR_Pins[0]) == LOW;
  } else {
    return analogRead(IR_Pins[index]) < 300;
  }
}


void imprimirLeituras() {
  Serial.print("D4_=");
  Serial.print(naLinha(0) ? " LINHA" : " BRANCO");
  Serial.print(" | A0_= ");
  Serial.print(analogRead(A8));
  Serial.print(naLinha(1) ? " LINHA" : " BRANCO");
  Serial.print(" | A1_=");
  Serial.print(analogRead(A10));
  Serial.print(naLinha(2) ? " LINHA" : " BRANCO");
  Serial.print(" | A2_=");
  Serial.print(analogRead(A5));
  Serial.print(naLinha(3) ? " LINHA" : " BRANCO");
  Serial.print(" | A3=");
  Serial.print(analogRead(A7));
  Serial.println(naLinha(4) ? " LINHA" : " BRANCO");
}
