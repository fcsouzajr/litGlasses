#include <Ultrasonic.h>

uint8_t IR_Pins[5] = {4, 5, 6, A2, A3};
uint8_t Motor[4] = {22, 23, 24, 25};

Ultrasonic Ultra_Drt(26, 27);
Ultrasonic Ultra_Esq(28, 29);
Ultrasonic Ultra_Mei(30, 31);

#define PWMd 2
#define PWMe 3

int dist_Drt, dist_Esq, dist_Mei;


void setup() {
  Serial.begin(9600);

  for(uint8_t x = 0; x < 5; x++){
    pinMode(IR_Pins[x], INPUT);

    if(x < 4){
      pinMode(Motor[x], OUTPUT);
    }
  }

}

void loop() {

  imprimirLeituras();
  delay(100);

  bool D4 = naLinha(IR_Pins[0]);
  bool A0 = naLinha(IR_Pins[1]);
  bool A1 = naLinha(IR_Pins[2]);
  bool A2 = naLinha(IR_Pins[3]);
  bool A3 = naLinha(IR_Pins[4]);

  if (D4 && A0 && A1 && A2 && A3) {
    Serial.println("É uma interseção");
  }
  else if (D4 && A2 && A3 && !A0 && !A1) {
    Serial.println("Interseção ou curva direita");
  }
  else if (!D4 && A0 && A1 && !A2 && !A3) {
    Serial.println("Interseção ou curva esquerda");
  }
  else if (D4) {
    Serial.println("Seguindo reto");
  }
  else if (!D4 && (A2 || A3)) {
    Serial.println("Curva direita (D4 perdeu linha)");
  }
  else if (!D4 && (A0 || A1)) {
    Serial.println("Curva esquerda (D4 perdeu linha)");
  }
  else {
    Serial.println("Situação indefinida");
  }
}

void Mover_Frente(){
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);
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

bool naLinha(uint8_t index) {
  if (index == 0) {
    // sensor do meio é digital
    return digitalRead(IR_Pins[0]) == LOW;
  } else {
    // os demais são analógicos
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
