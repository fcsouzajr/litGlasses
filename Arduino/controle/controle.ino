uint8_t Motor[4] = {4, 5, 6, 7};

#define PWMd 10
#define PWMe 9

int maxValue[2];
int minValue[2];
int maxValueConta[2];
int minValueConta[2];
int NumMin[2], NumMax[2]; 
int values[2];

int P, I, D, erroAnt, PID;

float Kp = 10, Kd = 1.5, Ki = 0.01;

uint8_t baseSpeed = 90;
uint8_t Vd, Ve;


void setup() {

  Serial.begin(9600);

  Serial.println("Indo");

  for(uint8_t x = 0; x < 4; x++){
    pinMode(Motor[x], OUTPUT);
  }

  Serial.println("motores ok");

  pinMode(PWMd, OUTPUT);
  pinMode(PWMe, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  Serial.println("Pinos OK");
  
  analogWrite(PWMd, baseSpeed);
  analogWrite(PWMe, baseSpeed);

  calibrar();

  while(true){
    if(digitalRead(2) == LOW){
      break;
    }
  }

}

void loop() {

  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], HIGH);
  digitalWrite(Motor[3], LOW);

  ler();

  int erro = values[0] - values[1];

  P = erro;
  I = I + erro;
  D = erro - erroAnt;

  erroAnt = erro;

  PID = (Kp * P) + (Ki * I) + (Kd * D);


  Ve = baseSpeed - PID;
  Vd = baseSpeed + PID;

  analogWrite(PWMd, Vd);
  analogWrite(PWMe, Ve);

  //Serial.print(erro);
  /*Serial.print(" ");
  Serial.print(Vd);
  Serial.print(" ");
  Serial.print(Ve);*/
  //Serial.print(" ");
  Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.print(minValue[0]);
  Serial.print(" ");
  Serial.print(analogRead(A1));
  Serial.print(" ");
  Serial.println(minValue[1]);
  

}

void calibrar(){

  Serial.println("Iniciando calibração");

  for(uint8_t x = 0; x < 2; x++){
    minValueConta[x] = analogRead(x);
    maxValueConta[x] = analogRead(x);
  }
  
  Serial.println("p2");

  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], HIGH);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);

  for(int x = 0; x < 10000; x++){
    for(uint8_t i = 0; i < 2; i++){
      
      int leitura = analogRead(i);
      
      if(leitura < minValueConta[i]){
        minValueConta[i] = leitura;
        minValue[i] += leitura;
        NumMin[i]++;
      }

      if(leitura > maxValueConta[i]){
        maxValueConta[i] = leitura;
        maxValue[i] += leitura;
        NumMax[i]++;
      }
    }
    delay(1);
  }
  
  Serial.println("P3");

  for(uint8_t x = 0; x < 2; x++){
    minValue[x] = minValue[x] / NumMin[x];
    maxValue[x] = maxValue[x] / NumMax[x];
  }

  digitalWrite(Motor[0], LOW);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], LOW);

  Serial.println("Calibrado");
}



void ler(){
  
  for(int i = 0; i < 2; i++){
    values[i] = map(analogRead(i), minValue[i], maxValue[i], 0, 100);
    //values[i] = map(analogRead(i), 0, 1023, 0, 100);
  } 
}
