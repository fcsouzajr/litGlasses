#include <Ultrasonic.h>
#include <Stepper.h>
#include <Servo.h>

// Stepper 28BYJ-48 (ULN2003)
const int stepsPerRevolution = 4096;  
Stepper myStepper(stepsPerRevolution, 50, 52, 51, 53); 
// OBS: a ordem é (IN1, IN3, IN2, IN4), por isso ficou assim

// Servos
Servo servo1;
Servo servo2;

uint8_t IR_Pins[5] = {4, A8, A10, A5, A7}; //pinos dos sensores infravermelhos
uint8_t Motor[4] = {22, 23, 24, 25}; //pinos dos motores (ponte H)
uint8_t Posicao[4] = {0, 0, 0, 0}; 
//Uma matriz para guardar onde está cada área para colocar os mortos e vivos, sendo posicao[0] o vertice inferior direito, assim por diante
static bool D4_, A0_, A1_, A2_, A3_;
static bool state_motor = false;
static bool leitura_cor = false;


Ultrasonic Ultra_Drt(26, 27);
Ultrasonic Ultra_Esq(28, 29);
Ultrasonic Ultra_Mei(30, 31);
uint32_t time_Ant;

#define SenC_D A0
#define SenC_E A2
#define PWMd 2
#define PWMe 3
#define RedPin A4
#define GreenPin A5
#define BluePin A6

#define Limiar_Min_D_Verde 890
#define Limiar_Min_E_Verde 820
#define Limiar_Max_D_Verde 905
#define Limiar_Max_E_Verde 840

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

  myStepper.setSpeed(10);
  servo1.attach(8);   
  servo2.attach(9);

  servo1.write(150);
  servo2.write(0)

}

unsigned long tempo_bloqueio = 0;
bool ignorarVerde = false;

void loop() {

  int dist = Ultra_Drt.read();

  if((millis() - time_Ant) > 50){
    Parar_Motores(200);
    time_Ant = millis();
  }

  if(dist < 10){
    Parar_Motores();
    Mover_Esquerda();
    delay(200);
    Mover_Frente(150);
    Parar_Motores(500);
    while(true){
      Mover_Direita(200);
      Parar_Motores(500):
      Mover_Frente(150);
      Parar_Motores(500):

      A0_ = naLinha(1);
      A1_ = naLinha(2);
      A2_ = naLinha(3);
      A3_ = naLinha(4);

      if(A0 || A1 || A2 || A3){
        break;
      }

    }
  }

  D4_ = naLinha(0);
  A0_ = naLinha(1);
  A1_ = naLinha(2);
  A2_ = naLinha(3);
  A3_ = naLinha(4);

  bool intersecao = (D4_ && A0_ && A1_ && A2_ && A3_);
  if(intersecao) {
    ignorarVerde = true;
    tempo_bloqueio = millis();
  }
  if(ignorarVerde && (millis() - tempo_bloqueio > 200)) {
    ignorarVerde = false;
  }

  int cor_D = analogRead(SenC_D);
  int cor_E = analogRead(SenC_E);

  if(!ignorarVerde) {
    if(cor_E > Limiar_Min_E_Verde && cor_D < Limiar_Max_E_Verde && cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Mover_Atras(200);
      Parar_Motores(500);
      Virar_Ao_Contrario();
    }else if(cor_E > Limiar_Min_E_Verde && cor_D < Limiar_Max_E_Verde){
      Parar_Motores(500);
      Mover_Frente(300);
      Mover_90_Graus_Esquerda();
      Mover_Atras(200);
      Parar_Motores(500);
    }else if(cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Parar_Motores(500);
      Mover_Frente(300);
      Mover_90_Graus_Direita();
      Mover_Atras(200);
      Parar_Motores(500);
    }
  }

  if(D4_ && A0_ && A1_ && A2_ && A3_){
    Parar_Motores(500);
    Mover_Frente(100);
    Parar_Motores(500);
    if(cor_E > Limiar_Min_E_Verde && cor_D < Limiar_Max_E_Verde && cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Mover_Atras(200);
      Parar_Motores(500);
      Virar_Ao_Contrario();
    }else if(cor_E > Limiar_Min_E_Verde && cor_D < Limiar_Max_E_Verde){
      Parar_Motores(500);
      Mover_Frente(300);
      Mover_90_Graus_Esquerda();
      Mover_Atras(300);
      Parar_Motores(500);
    }else if(cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Parar_Motores(500);
      Mover_Frente(300);
      Mover_90_Graus_Direita();
      Mover_Atras(300);
      Parar_Motores(500);
    }else{
      Mover_Frente(0);
    }
  }
  
  if(D4_ && !A0_ && A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Atras(100);
    Parar_Motores(500);
    Mover_Direita();
    delay(200);
    Parar_Motores(500); 
  }
  
  if(D4_ && !A0_ && !A1_ && A2_ && !A3_){
    Parar_Motores(500);
    Mover_Atras(100);
    Parar_Motores(500);
    Mover_Esquerda();
    delay(200);
    Parar_Motores(500);
  }
  
  /*if(!D4_ && A0_ && A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Frente(300);
    Mover_90_Graus_Direita();
    Mover_Atras(200);
    Parar_Motores(500);
  }/*
  
  /*if(!D4_ && !A0_ && !A1_ && A2_ && A3_){
    Parar_Motores(500);
    Mover_Frente(300);
    Mover_90_Graus_Esquerda();
    Mover_Atras(100);
    Parar_Motores(500);
  }*/
  
  if(!D4_ && A0_ && A1_ && A2_ && A3_){
    Parar_Motores(500);
    Mover_Frente(100);
    Parar_Motores(500);
    if(cor_E > Limiar_Min_E_Verde && cor_D < Limiar_Max_E_Verde && cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Mover_Atras(200);
      Parar_Motores(500);
      Virar_Ao_Contrario();
    }else if(cor_E > Limiar_Min_E_Verde && cor_D < Limiar_Max_E_Verde){
      Parar_Motores(500);
      Mover_90_Graus_Esquerda();
      Mover_Atras(100);
      Parar_Motores(500);
    }else if(cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Mover_90_Graus_Direita();
      Mover_Atras(100);
      Parar_Motores(500);
    }else{
      Mover_Frente(200);
      Mover_90_Graus_Esquerda();
      Mover_Atras(100);
      Parar_Motores(500);
    }
  }
  
  if(D4_ && A0_ && A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Frente(100);
    if(cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Parar_Motores(500);
      Mover_90_Graus_Direita();
      Mover_Atras(100);
      Parar_Motores(500);
    }else{
      Mover_Frente(0);
    }
  }
  
  if(D4_ && !A0_ && !A1_ && A2_ && A3_){
    Parar_Motores(500);
    Mover_Frente(100);
    if(cor_D > Limiar_Min_D_Verde && cor_D < Limiar_Max_D_Verde){
      Mover_90_Graus_Esquerda();
      Mover_Atras(100);
      Parar_Motores(500);
    }else{
      Mover_Frente(0);
    }
  }
  
  if(!D4_ && !A0_ && A1_ && A2_ && !A3_){
    Parar_Motores(500);
    Mover_Direita();
    delay(400);
    Parar_Motores(500);
    Mover_Atras(100);
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
    delay(150);
    Parar_Motores(500);
  }

  if(!D4_ && !A0_ && !A1_ && A2_ && !A3_){
    Parar_Motores(500);
    Mover_Esquerda();
    delay(150);
    Parar_Motores(500);
  }

  if(!D4_ && A0_ && A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Direita();
    delay(400);
    Parar_Motores(500);
    Mover_Atras(100);
    Parar_Motores(500);
  }

  if(!D4_ && !A0_ && !A1_ && A2_ && A3_){
    Parar_Motores(500);
    Mover_Esquerda();
    delay(400);
    Parar_Motores(500);
    Mover_Atras(100);
    Parar_Motores(500);
  }

  /*if(!D4_ && A0_ && A1_ && A2_ && !A3_){
    Parar_Motores(500);
    Mover_Frente(300);
    Mover_90_Graus_Direita();
    Mover_Atras(300);
    Parar_Motores(500);
  }

  if(!D4_ && !A0_ && A1_ && A2_ && A3_){
    Parar_Motores(500);
    Mover_Frente(300);
    Mover_90_Graus_Esquerda();
    Mover_Atras(300);
    Parar_Motores(500);
  }*/

  if(D4_ && A0_ && A1_ && A2_ && !A3_){
    Mover_Frente(0);
  }

  if(D4_ && !A0_ && A1_ && A2_ && A3_){
    Mover_Frente(0);
  }

  if(!D4_ && A0_ && !A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Direita();
    delay(300);
    Parar_Motores(500); 
    Mover_Frente(200);
    Parar_Motores(500);
  }

  if(!D4_ && !A0_ && !A1_ && !A2_ && A3_){
    Parar_Motores(500);
    Mover_Esquerda();
    delay(300);
    Parar_Motores(500); 
    Mover_Frente(200);
    Parar_Motores(500);
  }

  if(D4_ && !A0_ && !A1_ && !A2_ && A3_){
    Parar_Motores(500);
    Mover_Atras(100);
    Parar_Motores(500);
    Mover_Esquerda();
    delay(300);
    Parar_Motores(500); 
    Mover_Frente(100);
    Parar_Motores(500);
  }

  if(D4_ && A0_ && !A1_ && !A2_ && !A3_){
    Parar_Motores(500);
    Mover_Atras(100);
    Parar_Motores(500);
    Mover_Direita();
    delay(300);
    Parar_Motores(500); 
    Mover_Frente(100);
    Parar_Motores(500);
  }

  delay(50);
}


void Mover_Frente(int del){
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);
  delay(del);
}

void Mover_Esquerda(){
  analogWrite(PWMd, 75);
  analogWrite(PWMe, 75);
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);
}

void Mover_Direita(){
  analogWrite(PWMd, 75);
  analogWrite(PWMe, 75);
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
  Mover_Esquerda();
  delay(500);
}

void Mover_90_Graus_Direita(){
  Parar_Motores(500);
  Mover_Direita();
  delay(500);
}

void Virar_Ao_Contrario(){
  Parar_Motores(500);
  Mover_Direita();
  delay(1400);
}

void Mover_45_Graus_Esquerda(){
}

void Mover_45_Graus_Direita(){;
}


/*  Mapear_Area(){
  Mover_90_Graus_Direita(); //Mover o robô de frente à parede para calcular as matrizes
  if(Dist_Mei > 5){
    while(Dist_Mei > 5){
      Mover_Frente(0);
    }
    Parar_Motores();
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
  }else if(index == 1){
    return analogRead(IR_Pins[1]) < 400;
  }else {
    return analogRead(IR_Pins[index]) < 200;
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

void Garra(){
  myStepper.step(-1024);
  delay(500);

  for (int pos = 0; pos <= 150; pos++) {
    servo1.write(150 - pos);
    servo2.write(pos);
    delay(15);
  }

  delay(1000);

  myStepper.step(1024);

  delay(500);
}
