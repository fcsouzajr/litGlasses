#include <Ultrasonic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

uint8_t IR_Pins[5] = {4, 5, 6, A2, A3}; //pinos dos sensores infravermelhos
uint8_t Motor[4] = {22, 23, 24, 25}; //pinos dos motores (ponte H)
uint8_t Posicao[4] = {0, 0, 0, 0}; 
//Uma matriz para guardar onde está cada área para colocar os mortos e vivos, sendo posicao[0] o vertice inferior direito, assim por diante

Ultrasonic Ultra_Drt(26, 27);
Ultrasonic Ultra_Esq(28, 29);
Ultrasonic Ultra_Mei(30, 31);
Adafruit_MPU6050 mpu;

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

  if (!mpu.begin()) {
    Serial.println("Falha no MPU6050");
    while(1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

}

void loop() {

  imprimirLeituras();
  delay(100);

  bool D4 = naLinha(IR_Pins[0]);
  bool A0 = naLinha(IR_Pins[1]);
  bool A1 = naLinha(IR_Pins[2]);
  bool A2 = naLinha(IR_Pins[3]);
  bool A3 = naLinha(IR_Pins[4]);

  if(!D4 && analoRead(A0) > 500 && analogRead(A3) > 500){
    Mapear_Area();
  }

  if(D4 && A0 && A1 && A2 && A3){
    Mover_Frente(0);
  }else if(D4 && !A0 && A1 && !A2 && !A3){
    Mover_Direita();  
  }else if(D4 && !A0 && !A1 && A2 && !A3){
    Mover_Esquerda();
  }else if(!D4 && A0 && A1 && !A2 && !A3){
    Mover_90_Graus_Direita();
  }else if(!D4 && !A0 && !A1 && A2 && A3){
    Mover_90_Graus_Esquerda();
  }else if(!D4 && A0 && A1 && A2 && A3){
    Mover_90_Graus_Esquerda();
  }else if(!D4 && !A0 && A1 && !A2 && !A3){
    Mover_45_Graus_Direita();
  }else if(!D4 && !A0 && !A1 && A2 && !A3){
    Mover_45_Graus_Esquerda();
  }else if(D4 && A0 && A1 && !A2 && !A3){
    Mover_Frente(0);
  }else if(D4 && !A0 && !A1 && A2 && A3){
    Mover_Frente(0);
  }else if(!D4 && !A0 && A1 && A2 && !A3){
    Mover_90_Graus_Direita();
  }else{
    Mover_Frente(0);
  }
  
}

void Mover_Frente(int del){
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);
  delay(del);
}

void Mover_Esquerda(){
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);
}

void Mover_Direita(){
  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);
}

void Parar_Motores() {
  for(int i=0;i<4;i++){
    digitalWrite(Motor[i], LOW);
  }
}

void Mover_Atras(int del){
  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);
  delay(del);
}

void Mover_90_Graus_Esquerda(){
  Girar(-90);
}

void Mover_90_Graus_Direita(){
  Girar(90);
}

void Virar_Ao_Contrario(){
  Girar(180);
}

void Mover_45_Graus_Esquerda(){
  Girar(-45);
}

void Mover_45_Graus_Direita(){
  Girar(45);
}

void Girar(float angulo){
  Parara_Motores();
  delay(100); 
  
  Z_Angulo = 0;
  timeZ = micros();

  if(angulo > 0){
    while(Z_Angulo < angulo){
      atualizar_Z_Angulo();
      Mover_Direita();
    }
  } else {
    while(Z_Angulo > angulo){
      atualizar_Z_Angulo();
      Mover_Esquerda();
    }
  }
  
  Parara_Motores();
  delay(100);
}

void atualizar_Z_Angulo() {
  float deltaT = (float)(micros() - timeZ) / 1000000.0;
  timeZ = micros();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float gyroZ = g.gyro.z * (180.0 / PI); // graus/s
  Z_Angulo += gyroZ * deltaT;
}

Mapear_Area(){
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
    bool A0 = naLinha(IR_Pins[1]);
    bool A1 = naLinha(IR_Pins[2]);
    bool A2 = naLinha(IR_Pins[3]);
    bool A3 = naLinha(IR_Pins[4]);

    if(A0 && A1 && A2 && A3){
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
  
}

bool naLinha(uint8_t index) {
  if (index == 0) {
    return digitalRead(IR_Pins[0]) == LOW;
  } else {
    return analogRead(IR_Pins[index]) < 300;
  }
}


void imprimirLeituras() {
  Serial.print("D4=");
  Serial.print(naLinha(0) ? "LINHA" : "BRANCO");
  Serial.print(" | A0=");
  Serial.print(naLinha(1) ? "LINHA" : "BRANCO");
  Serial.print(" | A1=");
  Serial.print(naLinha(2) ? "LINHA" : "BRANCO");
  Serial.print(" | A2=");
  Serial.print(naLinha(3) ? "LINHA" : "BRANCO");
  Serial.print(" | A3=");
  Serial.println(naLinha(4) ? "LINHA" : "BRANCO");
}
