uint8_t Motor[4] = {4, 5, 6, 7};

#define PWMd 10
#define PWMe 9

int maxValue[2];
int minValue[2];

int values[2];

float P, I, D, erroAnt, PID;

float Kp = 1.0, Kd = 1.0, Ki = 0.01;

uint8_t baseSpeed = 90;
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
  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);

  float erro = (1.5*analogRead(A0) + 0.5*analogRead(A1)) - (1.5*analogRead(A3) + 0.5*analogRead(A2));

  P = erro;
  I = I + erro;
  D = erro - erroAnt;

  erroAnt = erro;

  PID = (Kp * P) + (Ki * I) + (Kd * D);

  // Limita PID
  if (PID > 255) PID = 255;
  else if (PID < -255) PID = -255;

  Ve = constrain(baseSpeed - PID, 0, 255);
  Vd = constrain(baseSpeed + PID, 0, 255);

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
  Serial.print(values[0]);
  Serial.print(" | S1: ");
  Serial.println(values[1]);

  delay(10); // pequeno delay pra não lotar o serial
}
