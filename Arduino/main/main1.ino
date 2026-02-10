#include <Keyboard.h>

#define Bot   2
#define Bot2  3

unsigned long UltimoClique = 0;
unsigned long UltimoClique2 = 0;

uint8_t Contador = 0;
uint8_t Contador2 = 0;

const int LimiteTempo = 600;   // tempo entre cliques (ms)
const int debounce = 40;

bool EstadoAnt = HIGH;
bool EstadoAnt2 = HIGH;


void enviarTecla(uint8_t tecla){
  Keyboard.press(tecla);
  delay(30);
  Keyboard.releaseAll();
}


void botDireita(){ //sensor direita
  bool Estado = digitalRead(Bot);

  if (Estado == LOW && EstadoAnt == HIGH) {
    delay(debounce);

    unsigned long agora = millis();

    if (agora - UltimoClique < LimiteTempo) {
      Contador++;
    } else {
      Contador = 1;
    }

    UltimoClique = agora;
  }

  EstadoAnt = Estado;

  if (millis() - UltimoClique > LimiteTempo && Contador > 0) {

    if (Contador == 1) {
      enviarTecla(KEY_RIGHT_ARROW);
    }
    else if (Contador == 2) {
      enviarTecla(KEY_UP_ARROW);
    }
    else if (Contador == 3) {
      enviarTecla(KEY_RETURN);
    }

    Contador = 0;
  }
}

void botEsquerda(){ //sensor esqurda
  bool Estado2 = digitalRead(Bot2);

  if (Estado2 == LOW && EstadoAnt2 == HIGH) {
    delay(debounce);

    unsigned long agora2 = millis();

    if (agora2 - UltimoClique2 < LimiteTempo) {
      Contador2++;
    } else {
      Contador2 = 1;
    }

    UltimoClique2 = agora2;
  }

  EstadoAnt2 = Estado2;

  if (millis() - UltimoClique2 > LimiteTempo && Contador2 > 0) {

    if (Contador2 == 1) {
      enviarTecla(KEY_LEFT_ARROW);
    }
    else if (Contador2 == 2) {
      enviarTecla(KEY_DOWN_ARROW);
    }
    else if (Contador2 == 3) {
      enviarTecla(KEY_ESC);
    }

    Contador2 = 0;
  }
}


void setup() {
  pinMode(Bot, INPUT_PULLUP);
  pinMode(Bot2, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  botDireita();
  botEsquerda();
}
