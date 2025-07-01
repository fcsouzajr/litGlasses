#include<SoftwareSerial.h>

SoftwareSerial BT(10, 11); // RX e TX

// Pinos dos botões
int botaoteclado = 19;
int botaotelefone = 20;
int botaoautomacao = 21;
int botaoOK = 2;

String estado = "";
String mensagem = "Paralelepipedo";

// Flags para envio por botão
bool botaoDPressionado = false;
bool botaoTelefonePressionado = false;
bool botaoEPressionado = false;
bool botaoOKPressionado = false;

// Flag para controlar envio manual
bool envioManual = false;

void setup()
{
  pinMode(botaoteclado, INPUT_PULLUP);
  pinMode(botaotelefone, INPUT_PULLUP);
  pinMode(botaoautomacao, INPUT_PULLUP);
  pinMode(botaoOK, INPUT_PULLUP);

  Serial.begin(9600);
  BT.begin(9600);
}

void loop()
{
  // ENVIO MANUAL do Serial para o Bluetooth
  if (Serial.available() > 0) {
    String entradaSerial = Serial.readStringUntil('\n');
    BT.println("MSG:" + entradaSerial);  // envia para o app com prefixo
    envioManual = true;
  }

  // Se não foi envio manual, permite envio por botões
  if (!envioManual) {

    // Botão TECLADO
    if (digitalRead(botaoteclado) == LOW) {
      if (!botaoDPressionado) {
        BT.println("BTTECLADO: Primeira mensagem recebida com sucesso");
        Serial.println("Botão TECLADO enviado");
        botaoDPressionado = true;
      }
    } else {
      botaoDPressionado = false;
    }

    // Botão TELEFONE
    if (digitalRead(botaotelefone) == LOW) {
      if (!botaoTelefonePressionado) {
        BT.println("BTTELEFONE");
        Serial.println("Botão TELEFONE enviado");
        botaoTelefonePressionado = true;
      }
    } else {
      botaoTelefonePressionado = false;
    }

    // Botão AUTOMAÇÃO
    if (digitalRead(botaoautomacao) == LOW) {
      if (!botaoEPressionado) {
        BT.println("BTAUTOMACAO: ");
        Serial.println("Botão AUTOMAÇÃO enviado");
        botaoEPressionado = true;
      }
    } else {
      botaoEPressionado = false;
    }

    // Botão OK
    if (digitalRead(botaoOK) == LOW) {
      if (!botaoOKPressionado) {
        BT.println("VOLTAR");
        Serial.println("Botão OK enviado");
        botaoOKPressionado = true;
      }
    } else {
      botaoOKPressionado = false;
    }
  }

  // Após envio manual, faz uma pequena pausa e libera botões
  if (envioManual) {
    delay(100); // tempo para garantir envio
    envioManual = false;
  }

  // RECEBER do Bluetooth e mostrar no Serial
  if (BT.available() > 0) {
    estado = BT.readStringUntil('\n');
    Serial.println("Recebido via BT: " + estado);
  }
}
