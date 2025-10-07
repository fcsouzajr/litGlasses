#include <SoftwareSerial.h>

SoftwareSerial Cell(2, 3); //rx e tx

uint8_t pins[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

void funcL1Q1ON();
void funcL1Q1OFF();
void funcL2Q1ON();
void funcL2Q1OFF();
void funcPQ1ON();
void funcPQ1OFF();
void funcL1Q2ON();
void funcL1Q2OFF();
void funcL2Q2ON();
void funcL2Q2OFF();
void funcPQ2ON();
void funcPQ2OFF();
void funcLSON();
void funcLSOFF();
void funcL2SON();
void funcL2SOFF();
void funcRSON();
void funcRSOFF();
void funcL1CON();
void funcL1COFF();
void funcL2CON();
void funcL2COFF();
void funcRCON();
void funcRCOFF();


struct Recebido{
  const char* receber;
  funcMain func;
};

Recebido rcb[] = {
  {"L1Q1ON", funcL1Q1ON},
  {"L1Q1OFF", funcL1Q1OFF},
  {"L2Q1ON", funcL2Q1ON},
  {"L2Q1OFF", funcL2Q1OFF},
  {"PQ1ON", funcPQ1ON},
  {"PQ1OFF", funcPQ1OFF},
  {"L1Q2ON", funcL1Q2ON},
  {"L1Q2OFF", funcL1Q2OFF},
  {"L2Q2ON", funcL2Q2ON},
  {"L2Q2OFF", funcL2Q2OFF},
  {"PQ2ON", funcPQ2ON},
  {"PQ2OFF", funcPQ2OFF},
  {"LSON", funcLSON},
  {"LSOFF", funcLSOFF},
  {"L2SON", funcL2SON},
  {"L2SOFF", funcL2SOFF},
  {"RSON", funcRSON},
  {"RSOFF", funcRSOFF},
  {"L1CON", funcL1CON},
  {"L1COFF", funcL1COFF},
  {"L2CON", funcL2CON},
  {"L2COFF", funcL2COFF},
  {"RCON", funcRCON},
  {"RCOFF", funcRCOFF}
};

void setup() {
  Cell.begin(9600);
  Serial.begin(9600);
  
  for(uint8_t x = 0; x < sizeof(pins) / sizeof(pins[0]); x++){
    pinMode(pins[x], OUTPUT);
    digitalWrite(pins[i], HIGH); // relé desligado no início
  }

}

void loop() {

  if(Cell.available() > 0){
    
    char recebido[10]; //criamos um char para armazenar as mensagens recebidas
    int len = Cell.readBytesUntil('\n', recebido, 10); //lê o que é recebido pelo serial até encontrar o caractere '\n' ou completar o limite de bytes (10)
    recebido[len] = '\0'; \\completa o char para poder utilizar os métodos de C
    
    for(uint8_t x = 0; x < sizeof(rcb) / sizeof(rcb[0]); x++){
      if(strcmp(recebido, rcb[x].receber) == 0){ //se o dado recebido for igual ao caractere que está no vetor
        rcb[x].func(); //executa a função do struct
        break; //para o loop do for
      }
    }
  }
}

// ===== L1Q1 =====
void funcL1Q1ON(){ 
  digitalWrite(pins[0], LOW); 
}
void funcL1Q1OFF(){
  digitalWrite(pins[0], HIGH); 
}

// ===== L2Q1 =====
void funcL2Q1ON(){ 
  digitalWrite(pins[1], LOW); 
}
void funcL2Q1OFF(){ 
  digitalWrite(pins[1], HIGH); 
}

// ===== PQ1 =====
void funcPQ1ON(){ 
  digitalWrite(pins[2], LOW);
}
void funcPQ1OFF(){ 
  digitalWrite(pins[2], HIGH); 
}

// ===== L1Q2 =====
void funcL1Q2ON(){ 
  digitalWrite(pins[3], LOW); 
}
void funcL1Q2OFF(){ 
  digitalWrite(pins[3], HIGH); 
}

// ===== L2Q2 =====
void funcL2Q2ON(){ 
  digitalWrite(pins[4], LOW); 
}
void funcL2Q2OFF(){ 
  digitalWrite(pins[4], HIGH); 
}

// ===== PQ2 =====
void funcPQ2ON(){ 
  digitalWrite(pins[5], LOW); 
}
void funcPQ2OFF(){ 
  digitalWrite(pins[5], HIGH); 
}

// ===== LS =====
void funcLSON(){ 
  digitalWrite(pins[6], LOW); 
}
void funcLSOFF(){ 
  digitalWrite(pins[6], HIGH);
}

// ===== L2S =====
void funcL2SON(){
  digitalWrite(pins[7], LOW); 
}
void funcL2SOFF(){ 
  digitalWrite(pins[7], HIGH); 
}

// ===== RS =====
void funcRSON(){ 
  digitalWrite(pins[8], LOW); 
}
void funcRSOFF(){ 
  digitalWrite(pins[8], HIGH); 
}

// ===== LC =====
void funcL1CON(){
  digitalWrite(pins[9], LOW); 
}
void funcL1COFF(){
  digitalWrite(pins[9], HIGH);
}

// ===== L2C =====
void funcL2CON(){ 
  digitalWrite(pins[0], LOW);
}
void funcL2COFF(){ 
  digitalWrite(pins[0], HIGH);
}

// ===== RC =====
void funcRCON(){ 
  digitalWrite(pins[1], LOW); 
}
void funcRCOFF(){ 
  digitalWrite(pins[1], HIGH); 
}
