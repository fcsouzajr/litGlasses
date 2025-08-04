uint8_t Motor[4] = {4, 5, 6, 7};

#define PWMd 10
#define PWMe 9

int maxValue[2];
int minValue[2];

int values[2];

int P, I, D, erroAnt, PID;

float Kp = 3.0, Kd = 1.5, Ki = 0.01;

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

  //calibrar();
}

void loop() {
  // Configura os motores para frente (ajusta conforme teu motor)
  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);

  lerSensores();

  int erro = values[0] - values[1];

  P = erro;
  I = I + erro;
  D = erro - erroAnt;

  erroAnt = erro;

  PID = (Kp * P) + (Ki * I) + (Kd * D);

  // Limita PID para não dar pau no motor
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

/*void calibrar() {
  Serial.println("Iniciando calibração...");

  // Inicializa os min e max
  for (uint8_t i = 0; i < 2; i++) {
    minValue[i] = 1023;
    maxValue[i] = 0;
  }

  // Liga motores em sentido contrário pra passar na linha (ajusta conforme teu motor)
  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);

  // Lê várias vezes para pegar min e max reais
  for (int x = 0; x < 10000; x++) {
    for (uint8_t i = 0; i < 2; i++) {
      int leitura = analogRead(i);
      if (leitura < minValue[i]) minValue[i] = leitura;
      if (leitura > maxValue[i]) maxValue[i] = leitura;
    }
    delay(1);
  }

  // Desliga motores
  for (uint8_t x = 0; x < 4; x++) {
    digitalWrite(Motor[x], LOW);
  }

  Serial.println("Calibração feita:");
  for (uint8_t i = 0; i < 2; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" Min: ");
    Serial.print(minValue[i]);
    Serial.print(" Max: ");
    Serial.println(maxValue[i]);
  }
}*/

void lerSensores() {
  values[0] = constrain(analogRead(A0), 185, 820);
  values[1] = constrain(analogRead(A1), 180, 830);

  values[0] = map(values[0], 195, 805, 0, 100);
  values[1] = map(values[1], 180, 820, 0, 100);

}
