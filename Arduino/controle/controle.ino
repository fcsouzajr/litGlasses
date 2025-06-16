uint8_t Motor[] = {4, 5, 6, 7};

#define PWMd 10
#define PWMe 9

int maxValue[2];
int minValue[2];
int values[2];

int P, I, D, erroAnt, PID;

float Kp = 0.8, Kd = 1.5, Ki = 0.01;

uint8_t baseSpeed = 255;
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

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);

  /*ler();

  int erro = values[0] - values[1];

  P = erro;
  I = I + erro;
  D = erro - erroAnt;

  erroAnt = erro;

  PID = (Kp * P) + (Ki * I) + (Kd * D);


  Ve = baseSpeed - PID;
  Vd = baseSpeed + PID;*/

  analogWrite(PWMd, 255);
  analogWrite(PWMe, 255);

  /*Serial.print(erro);
  Serial.print(" ");
  Serial.print(Vd);
  Serial.print(" ");
  Serial.print(Ve);
  Serial.print(" ");
  Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.println(analogRead(A1));*/
  

}

void calibrar(){

  Serial.println("Iniciando calibração");

  digitalWrite(Motor[0], HIGH);
  digitalWrite(Motor[1], LOW);
  digitalWrite(Motor[2], LOW);
  digitalWrite(Motor[3], HIGH);


  for(uint8_t x = 0; x < 2; x++){
    minValue[x] = analogRead(x);
    maxValue[x] = analogRead(x);
  }
  
  Serial.println("p2");

  for(int x = 0; x < 3000; x++){

    for(uint8_t i = 0; i < 2; i++){
      
      int leitura = analogRead(i);
      
      if(leitura < minValue[i]){
        minValue[i] = leitura;
      }

      if(leitura > maxValue[i]){
        maxValue[i] = leitura;
      }
    }
    delay(1);
  }
  
  Serial.println("P3");

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

//ATT 
