#include <SoftwareSerial.h>

SoftwareSerial Cell(2, 3); //rx e tx

uint8_t pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

char buffer[10];  // para armazenar o texto digitado
byte index = 0;

void setup() {

  Serial.begin(9600);
  
  for(uint8_t x = 0; x < sizeof(pins) / sizeof(pins[0]); x++){
    pinMode(pins[x], OUTPUT);
  }

}

void loop() {

}
