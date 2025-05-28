#include <PID_v1.h>

#define PIN_INPUT 2
#define PIN_OUTPUT 9
int pin[] = {6, 7};
bool ant = HIGH;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
double Kp=2, Ki=1.5, Kd=0.3;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup()
{
  //initialize the variables we're linked to
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(9, OUTPUT);
  for(int x = 0; x++; x < 2){
    pinMode(pin[x], OUTPUT);
  }
  Input = 0;
  Setpoint = 50;

  digitalWrite(pin[0], LOW);
  digitalWrite(pin[1], HIGH);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255.0, 255.0);
}

void loop()
{

  while(abs(Setpoint - Input) > 1){

    if(Output < 0){
      digitalWrite(pin[0], HIGH);
      digitalWrite(pin[1], LOW);
      if(digitalRead(PIN_INPUT) == LOW && ant == HIGH){
      Input--;
      }
    }else{
      if(digitalRead(PIN_INPUT) == LOW && ant == HIGH){
      Input++;
      }
    }

    analogWrite(PIN_OUTPUT, Output < 0 ? (Output * (-1)) : Output);
    myPID.Compute();
    ant = digitalRead(PIN_INPUT);

    Serial.print(Input);
    Serial.print(" ");
    Serial.println(Output);

    /*if(Input == 48 || Input == 47){
      Input+= 10;
    }*/

  }

  analogWrite(PIN_OUTPUT, 0);
  Serial.print(Input);
  Serial.print(" ");
  Serial.println("Acabou");

}