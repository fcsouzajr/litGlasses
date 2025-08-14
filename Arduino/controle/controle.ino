#include <Ultrasonic.h>

Ultrasonic Ultra_Drt(26, 27);
Ultrasonic Ultra_Esq(28, 29);
Ultrasonic Ultra_Mei(30, 31);

uint8_t Motor[4] = {22, 23, 24, 25};

#define PWMd 2
#define PWMe 3

float P, I, D, erroAnt, PID;

float Kp = 5.0, Kd = 1.5, Ki = 0.01;

uint8_t baseSpeed = 70;
uint8_t Vd, Ve;

void setup() {
  Serial.begin(9600);
  
  Serial.println("Iniciando");

  for (uint8_t x = 0; x < 4; x++) {
    pinMode(Motor[x], OUTPUT);
  }

  Serial.println("Motores ok");

  pinMode(PWMd, OUTPUT);
  pinMode(PWMe, OUTPUT);

  pinMode(2, INPUT_PULLUP);

  Serial.println("Pinos OK");

  analogWrite(PWMd, baseSpeed);
  analogWrite(PWMe, baseSpeed);

}

void loop() {
  // Configura os motores para frente (ajusta conforme teu motor)
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);

  float erro = (1.1*analogRead(A0) + 0.5*analogRead(A1)) - (1.1*(analogRead(A3)+60) + 0.5*analogRead(A2));

  P = erro;
  I = I + erro;
  D = erro - erroAnt;

  erroAnt = erro;

  PID = (Kp * P) + (Ki * I) + (Kd * D);

  // Limita PID
  if (PID > 30) PID = 30;
  else if (PID < -30) PID = -30;

  Ve = constrain(baseSpeed - (int)PID, 0, 255);
  Vd = constrain(baseSpeed + (int)PID, 0, 255);

  analogWrite(PWMd, Vd);
  analogWrite(PWMe, Ve);

  // Debug no serial
  Serial.print("Erro: ");
  Serial.print(erro);
  Serial.print(" | Vd: ");
  Serial.print(Vd);
  Serial.print(" | Ve: ");
  Serial.print(Ve);
  Serial.print(" | S0: ");
  Serial.print(analogRead(A0));
  Serial.print(" | S1: ");
  Serial.print(analogRead(A1));
  Serial.print(" | S2: ");
  Serial.print(analogRead(A2));
  Serial.print(" | S3: ");
  Serial.println(analogRead(A3));

  delay(10); // pequeno delay pra não lotar o serial
}
