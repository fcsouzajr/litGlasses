#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX e TX

#define pinMotor1A     2 //vd
#define pinMotor1B     4 //az
#define pinMotor2A     3  //br
#define pinMotor2B     5  //cz

// int buzPin = 2;  // Buzzer pin
const int ledPin = A5; // LED pin

char mensagem = "Paralelepipedo";


void setup() {
    Serial.begin(9600);
    BT.begin(9600);
    //pinMode(buzPin, OUTPUT);
    //pinMode(ledPin, OUTPUT);

  // Set initial motor speed
  pinMode(pinMotor1A, OUTPUT);
  pinMode(pinMotor1B, OUTPUT);
  pinMode(pinMotor2A, OUTPUT);
  pinMode(pinMotor2B, OUTPUT);
}

void loop() {
    while (BT.available() > 0) {
        mensagem = BT.read(); // Read command from serial
        Serial.println(mensagem);
        switch(mensagem) {
            case 'F':   // Move forward
                digitalWrite(pinMotor1A, HIGH);
                digitalWrite(pinMotor1B, LOW);;
                digitalWrite(pinMotor2A, LOW);
                digitalWrite(pinMotor2B, HIGH);
                break;

            case 'B':   // Move backward
                digitalWrite(pinMotor1A, HIGH);
                digitalWrite(pinMotor1B, LOW);;
                digitalWrite(pinMotor2A, LOW);
                digitalWrite(pinMotor2B, HIGH);
                break;

            case 'R':   // Turn right
                digitalWrite(pinMotor1A, HIGH);
                digitalWrite(pinMotor1B, LOW);;
                digitalWrite(pinMotor2A, LOW);
                digitalWrite(pinMotor2B, HIGH);
                break;

            case 'L':   // Turn left
                digitalWrite(pinMotor1A, HIGH);
                digitalWrite(pinMotor1B, LOW);;
                digitalWrite(pinMotor2A, LOW);
                digitalWrite(pinMotor2B, HIGH);
                break;

            case 'S':   // Stop all motors
                digitalWrite(pinMotor1A, LOW);
                digitalWrite(pinMotor1B, LOW);
                digitalWrite(pinMotor2A, LOW);
                digitalWrite(pinMotor2B, LOW);
                break;
        }
    }
}
