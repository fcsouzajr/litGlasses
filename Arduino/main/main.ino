#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 8

SSD1306AsciiAvrI2c oled; //objeto da biblioteca para manipulação gráfica do display

SoftwareSerial Tx(10, 11); //Criando objeto para a biblioteca e se comunicar com o modulo bluetooth
SoftwareSerial Cell(4, 5); //criando objeto para outro módulo bluetooth se comunicar com o celular

#define Bot 2 //definição dos botões
#define Bot2 3
#define led 7

#define Colunas 3 //configuração para as colunas e a largura (em pixeis);
#define Largura 40;

String fraseDigitada = ""; //indica a frase que está sendo digitada
String palavrasCorretas[3];  // Para armazenar até 3 palavras corretas
uint8_t contador = 0; //contador para saber quantas palavras foram adicionadas no vetor palavras corretas, 
String fraseFinal = ""; //frase que aparece no final, que foi escrita
int8_t indiceN = -1; //para selecionar a palavra indicada que tu quer escrever (é -1 pq posso adicionar +1 no codgo sem se preocupar)
static bool emSelecionar = false; //Saber se está navegando entre as letras ou aa palavras indicadas
int cont = 0; //contador para saber se está navegandoi no teclado ou nas opções de palavra

unsigned long UltimoClique; //Variaveis para contar cliques no botão 1
uint8_t Contador = 0; //contador de cliques
unsigned long UltimoClique2; //variaveis para contar os cliques no botão 2
uint8_t Contador2 = 0;
int LimiteTempo = 1000; //limite de tempo para piscar, ou seja 1 segundo
static bool Condi = false;
static bool antCondi = false; //

//Estrutura e controle de opções
struct opData {
  const char* mensagem; //mensagem que aparece no display
  const char* comandoSerial; //codigo que é enviado por comunicação UART
  bool state = false; //vai mostrar o estado que está o pino em questão
};

opData Sala[] = {
  {"RELE 1", "0"},
  {"RELE 2", "1"},
  {"TV", "2"}
};

opData Quarto1[] = {
  {"RELE 1", "3"},
  {"RELE 2", "4"},
  {"TV", "5"}
};

opData Quarto2[] = {
  {"RELE 1", "6"},
  {"RELE 2", "7"},
  {"TV", "8"}
}

opData Cozinha[] = {
  {"RELE 1", "9"},
  {"RELE 2", "10"},
  {"Eletro", "11"}
};

//textos dos menus
const char* menuPrincipal[] = {"Comunicacao", "Automacao"};
const char* subMenuComunicacao[] = {"Digitar"};
const char* subMenuAutomacao[] = {"Sala", "Quarto1", "Quarto2", "Cozinha"};
//teclado
const char* tecladoABC []= {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L","M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"," "};
//opções do submenu automação
const char* opSala[] = {"Rele 1", "Rele 2", "TV"};
const char* opQuarto1[] = {"Rele 1", "Rele 2", "TV"};
const char* opQuarto2[] = {"Rele 1", "Rele 2", "TV"};
const char* opCozinha[] = {"Rele 1", "Rele 2", "Eletro"};

// Variáveis para controle de menu
uint8_t indice = 0;
uint8_t menuAtual = 0; //(Comunicação = 1 e automação = 2);
uint8_t opAtual = 0; //Será a opção, sendo respectivamente o que está nas funções acima, começando do 1
static bool emSub = false; //saber se está no submenu
static bool emOp = false; //saber se está nas opç~es de submenu
static bool digitar = false; //variável para saber se está no modo de digitação

void setup() {
  Tx.begin(9600); //definindo baund rate do modulo bluetooth
  Cell.begin(9600);
  Serial.begin(9600);
  oled.begin(&Adafruit128x64, 0x3C);
  if(!SD.begin(SD_CS)){
    Serial.println("Erro ao inciar SD");
  }else{
    Serial.println("SD PRONTO!");
  }
  oled.setFont(System5x7); //definindo a fonte
  oled.clear(); //limpando o display
  pinMode(led, OUTPUT);
  pinMode(Bot, INPUT_PULLUP); //definindo os pinos como de entrada com resistor interno
  pinMode(Bot2, INPUT_PULLUP);
  mostrarMenu();
}

void loop() {

  if(digitar){ //se estiver no teclado, o limite de tempo para cada piscada é menor
    LimiteTempo = 800;
  }else{
    LimiteTempo = 1000;
  }

  digitalWrite(led, Condi);
  bot1();
  bot2();
  Serial.print("emSub: "); Serial.println(emSub);
  Serial.print("emOp: "); Serial.println(emOp);
  Serial.print("digitar: "); Serial.println(digitar);
  Serial.print("emSelecionar: "); Serial.println(emSelecionar);
  Serial.println("----------------------");
  Serial.println(fraseFinal);
  delay(50);
}

void bot1() {
  static bool EstadoAnt = HIGH;
  bool Estado = digitalRead(Bot);

  if (Estado == LOW && EstadoAnt == HIGH) {
    unsigned long Agora = millis();

    if (Agora - UltimoClique < LimiteTempo) {
      Contador++;
    } else {
      Contador = 1;
    }
    UltimoClique = Agora;
  }

  EstadoAnt = Estado;

  if (millis() - UltimoClique > LimiteTempo && Contador > 0) {

    if (Contador == 1 && digitar && !emSelecionar){
      navegar(1);

    }else if (Contador == 2 && Condi) {
      if(digitar){
        if(cont > 0){ //se tiver descido o suficiente, vai para parte de selecionar palavras indicadas
          emSelecionar = true;
          navegarSelecionar();
        }else{
          cont++;
          navegar(16);
        }
      }else{
        executar();
      }
    }else if (Contador == 3) {
      if(digitar){ //se estiver no teclado digitando, e estiver selecionando uma palavra indicada
        if(emSelecionar){
          fraseFinal += " ";
          fraseFinal += palavrasCorretas[indiceN];
          fraseDigitada = "";
          mostrarTeclado();
        }else{
          executar();
        }
      }else{
        Condi = !Condi;
        Serial.println("AA");
      }
    }
    Contador = 0;
  }
}

void bot2() {
  static bool EstadoAnt2 = HIGH;
  bool Estado2 = digitalRead(Bot2);

  if (Estado2 == LOW && EstadoAnt2 == HIGH) {
    unsigned long Agora2 = millis();

    if (Agora2 - UltimoClique2 < LimiteTempo) {
      Contador2++;
    } else {
      Contador2 = 1;
    }
    UltimoClique2 = Agora2;
  }

  EstadoAnt2 = Estado2;

  if (millis() - UltimoClique2 > LimiteTempo && Contador2 > 0) {
    if(Contador2 == 1 && digitar && !emSelecionar){
      navegar(-1);
    }else if(Contador2 == 2 && Condi && !emSelecionar){   
      if(digitar){
        cont--;
        if(cont < 0){
          cont = 0;
        }
        navegar(-16);  
      }else{
        navegar(1);
      }
    }else if (Contador2 == 3 && Condi) {
      if(digitar){
        indice = 0;
        digitar = false;
        emOp = false;
        emSub = false;
        emSelecionar = false;
        oled.clear();
        oled.print(fraseFinal);
        Cell.print(fraseFinal); //mandando a frase para o celular
        delay(3000);
        fraseDigitada = "";
        fraseFinal = "";
        indice = 0;
        mostrarMenu();
      }else{
        navegar(3);
      }
    }else if (Contador2 == 4 && Condi) {
      if(emOp){
        opAtual = 0;
        digitar = false;
        emOp = false;
        emSub = true;
        indice = 0;
        mostrarSubMenu();
      }else if(emSub){
        menuAtual = 0;
        digitar = false;
        emOp = false;
        emSub = false;
        indice = 0;
        mostrarMenu();
      }
    }
    Contador2 = 0;
  }
}

void mostrarMenu() {
  oled.clear();

  oled.setCursor(0, 0);
  oled.print(F("Menu"));

  indice %= (sizeof(menuPrincipal) / sizeof(menuPrincipal[0]));

  for(uint8_t i = 0; i < (sizeof(menuPrincipal) / sizeof(menuPrincipal[0])); i++){
    uint8_t y = i + 1;
    if(i == indice){
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(0, y);
    oled.print(menuPrincipal[i]);
  }
}

void mostrarSubMenu(){
  oled.clear();

  indice %= sizeof(subMenuAutomacao) / sizeof(subMenuAutomacao[0]);

  for(uint8_t i = 0; i < sizeof(subMenuAutomacao) / sizeof(subMenuAutomacao[0]); i++){
    uint8_t x = (i % Colunas) * Largura;
    uint8_t y = (i / Colunas) + 1;

    if(i == indice){
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(x, y);
    oled.print(subMenuAutomacao[i]);
  }
}

void mostrarOp(){

  oled.clear();

  const char** opMenu; //vetor que vai receber outro vetor
  uint8_t tamanho; //saber quantas opções tem em cada struct

  switch (opAtual) {
    case 1:
      oled.setCursor(0, 0);
      oled.print(F("Sala-Auto"));
      opMenu = opSala;
      tamanho = sizeof(opSala) / sizeof(opSala[0]);
      Cell.print("BTAUTO S"); //envia pro aplicativo que tá na opção se sala
    break;
    case 2:
      oled.setCursor(0, 0);
      oled.print(F("Quarto1-Auto"));
      opMenu = opQuarto1;
      tamanho = sizeof(opQuarto1) / sizeof(opQuarto1[0]);
      Cell.print("BTAUTO Q1"); //envia para o aplicativo que tá na opção de quarto 1
    break;
    case 3:
      oled.setCursor(0,0);
      oled.print(F("Quarto2-Auto"));
      opMenu = opQuarto2;
      tamanho = sizeof(opQuarto2) / sizeof(opQuarto2[0]);
      Cell.print("BTAUTO Q2"); //envia para o aplicativo que tá na opção de quarto 2
    case 4:
      oled.setCursor(0, 0);
      oled.print(F("Cozinha-Auto"));
      opMenu = opCozinha;
      tamanho = sizeof(opCozinha) / sizeof(opCozinha[0]);
      Cell.print("BTAUTO C"); //envia para o aplicativo que tá na opção da cozinha 
    break;
  }

  indice %= tamanho;

  for(uint8_t i = 0; i < tamanho; i++){
    uint8_t x = (i % Colunas) * Largura;
    uint8_t y = (i / Colunas) + 1;

    if(i == indice){
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(x, y);
    oled.print(opMenu[i]); //vai imprimir a opção que tá no vetor
  }
}

void mostrarTeclado(){
  oled.clear();

  oled.println(fraseFinal); //exibe as palavras que estão sendo escolhidas
  
  oled.setCursor(70, 4);
  oled.println(fraseDigitada); //exibe o que tu tá escrevendo 
  for(uint8_t i = 0; i < contador; i++){
    uint8_t y = 3 + (i * 1);
    if(i == indiceN && emSelecionar){ //se stiver descendo nas opções, vai ficar marcado, se não, evita ficar para não causar confusão
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(0, y);
    oled.print(palavrasCorretas[i]);
  }

  indice %= sizeof(tecladoABC) / sizeof(tecladoABC[0]);

  for(uint8_t i = 0; i < sizeof(tecladoABC) / sizeof(tecladoABC[0]); i++){
    uint8_t x = (i % 16) * 8;
    uint8_t y = (i / 16) + 1;

    if(i == indice && !emSelecionar){ //mesmo caso lá de cima, para evitar confusão tira a marcação
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(x, y);
    oled.print(tecladoABC[i]);
  }
}

void executar(){
  if(emOp){ //se estiver em uma opção, irá executar a opção selecionada
    executarOp();
  }else if(emSub){
    if(digitar){ //se digitar = true, vai executar a frase
      executarFrase();
    }else{
      opAtual = indice + 1; //se não, vai entrar na opção que selecinou no menu de automação
      emOp = true;
      emSub = false;
      indice = 0;
      mostrarOp();
    }
  }else{
    menuAtual = indice + 1; //se menuAutal = 1 comunicação, se = 2 é de automção
    emSub = true;
    emOp = false;
    indice = 0;
    if(menuAtual == 1){ //se estiver em comunicão, vai mostrar o teclado
      digitar = true; 
      mostrarTeclado();
    }else{
      mostrarSubMenu(); //se tiver em automação vai mostrar as subopções (Quarto1 etc) de automação
      Cell.print("BTAUTO");
    }
  }
}

void executarOp(){
  opData* dados = nullptr; //matriz que vai receber a estrutura

  switch (opAtual){ 
    case 1:
      dados = Sala;
    break;

    case 2:
      dados = Quarto1;
    break;

    case 3: 
      dados = Cozinha;
    break;

    default:
    break;
  }

  oled.clear();
  oled.setCursor(0, 0);
  oled.print(dados[indice].mensagem);
  Tx.print(dados[indice].comandoSerial);
  dados[indice].state = !dados[indice].state;
  Cell.print(String(dados[indice].mensagem) + (dados[indice].state ? " ON" : " OFF"));
  delay(2000);
  oled.clear();
  mostrarOp();
}

void executarFrase(){
  fraseDigitada += tecladoABC[indice];

  String nomeArquivo = String(fraseDigitada[0]) + ".txt";

  File arquivo = SD.open(nomeArquivo);
  if(!arquivo){
    Serial.println("Erro ao abrir");
    return;
  }                                                                                                                        

  contador = 0;
  String palavra;

  while(arquivo.available()){
    palavra = arquivo.readStringUntil('\n');
    if(palavra.startsWith(fraseDigitada)){
      palavrasCorretas[contador] = palavra; //quando contador é 0, vai colocar na posição 0 do vetor a palavra e etc
      contador++;
    }

    if(contador == 3){ //limita a quantindade de palavras em 3
      break;
    }
  }
  arquivo.close();
  mostrarTeclado();
}

void navegar(int num){
  indice += num;

  if(oled.invertMode() == true){
    oled.setInvertMode(false);
  }

  if(emOp){
    mostrarOp();
  }else if(emSub){
    if(digitar){
      mostrarTeclado();
    }else{
      mostrarSubMenu();
    }
  }else{
    mostrarMenu();
  }
}

void navegarSelecionar(){
  if(indiceN > (contador - 2)){
    indiceN = -1;
    indice = 0;
    cont = 0;
    emSelecionar = false;
    mostrarTeclado();
  }else if (emSelecionar){
    indiceN++;
    mostrarTeclado();
  }
}
