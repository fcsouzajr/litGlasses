uint8_t Motor[4] = {4, 5, 6, 7};

#define PWMd 10
#define PWMe 9

int maxValue[2];
int minValue[2];
int media[2];

int P, I, D, erroAnt, PID;

float Kp = 0.6, Kd = 6, Ki = 0;

uint8_t baseSpeed = 120;
uint8_t Vd, Ve;


void setup() {

  for(uint8_t x = 0; x < 4; x++){
    pinMode(Motor[x], OUTPUT);
  }

  pinMode(PWMd, OUTPUT);
  pinMode(PWMe, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  
  calibrar();

  while(true){
    if(digitalRead(2) == LOW){
      break;
    }
  }

  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);

}

void loop() {
  int erro = (analogRead(0) - analogRead(1));

  P = erro;
  I = I + erro;
  D = erro - erroAnt;

  PID = (Kp * P) + (Ki * I) + (Kd * D);

  erroAnt = erro;
  Ve = (baseSpeed - PID);
  Vd = (baseSpeed + PID);
  

}

void calibrar(){
  for(uint8_t x = 0; x < 2; x++){
    minValue[x] = analogRead(x);
    maxValue[x] = analogRead(x);
  }

  for(uint8_t x = 0; x <= 2000; x++){
    digitalWrite(Motor[0], HIGH);
    digitalWrite(Motor[1], LOW);
    digitalWrite(Motor[2], LOW);
    digitalWrite(Motor[3], HIGH);
    analogWrite(PWMe, baseSpeed);
    analogWrite(PWMd, baseSpeed);

    for(uint8_t x = 0; x < 2; x++){
      int read = analogRead(x);
      if(read < minValue[x]){
        minValue[x] = read;
      }

      if(read > maxValue[x]){
        maxValue[x] = read;
      }
    }
  }

  for(uint8_t x = 0; x < 2; x++){
    media[x] = (minValue[x] + maxValue[x]) / 2;
    Serial.println(media[x]);
  }

  Serial.println("Calibrado");
}
