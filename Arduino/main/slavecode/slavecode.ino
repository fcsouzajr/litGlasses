#include <SoftwareSerial.h>

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
  while (Serial.available()) {
    char c = Serial.read();

    // Se for ENTER (nova linha), processa
    if (c == '\n' || c == '\r') {
      buffer[index] = '\0';  // finaliza a string

      uint8_t numero = atoi(buffer);  // converte para inteiro

      Serial.print("Número recebido: ");
      Serial.println(numero);

      
      for(uint8_t x = 0; x < sizeof(pins) / sizeof(pins[0]); x++){
        if(pins[x] == numero){
          digitalWrite(pins[x], !digitalRead(pins[x]));
          break;
        }
      }

      // limpa para a próxima entrada
      index = 0;
      memset(buffer, 0, sizeof(buffer));  // limpa o conteúdo do buffer
    }
    // Se for caractere normal e ainda há espaço no buffer
    else if (index < sizeof(buffer) - 1) {
      buffer[index++] = c;
    }
  }
}
