#include <SD.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>

#define SD_CS 8

SSD1306AsciiAvrI2c oled; //objeto da biblioteca para manipulação gráfica do display

SoftwareSerial Cell(4, 5); //criando objeto para outro módulo bluetooth se comunicar com o celular, RX --- TX

#define Bot 2 //definição dos botões
#define Bot2 3
#define led 13

#define Colunas 3 //configuração para as colunas e a largura (em pixeis);
#define Largura 40;

char fraseDigitada[31] = ""; //indica a frase que está sendo digitada
char palavrasCorretas[3][31];  // Para armazenar até 3 palavras corretas
uint8_t contador = 0; //contador para saber quantas palavras foram adicionadas na matriz palavras corretas, 
char fraseFinal[40] = ""; //frase que aparece no final, que foi escrita
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
static bool execut = false;

//Estrutura e controle de opções
struct opData {
  const char* mensagem; //mensagem que aparece no display
  bool state; //vai mostrar o estado que está o pino em questão
};

opData Sala[] = {
  {"LS", false}, 
  {"LS", false},
  {"RS", false}
};

opData Quarto1[] = {
  {"L1Q1", false},
  {"L1Q1", false},
  {"PQ1", false}
};

opData Quarto2[] = {
  {"L1Q2", false},
  {"L2Q2", false},
  {"PQ2", false}
};

opData Cozinha[] = {
  {"L1C", false},
  {"L2C", false},
  {"RC", false}
};

typedef void (*funcMain)(); //definindo uma função de ponteiro para ser de cada vetor da struct

void funcAUTON();
void funcTCLN();
void funcSALA();
void funcQRT1();
void funcQRT2();
void funcCZNH();
void funcVM();
void funcRL1Q1();
void funcRL2Q1();
void funcTVQ1();
void funcRL1Q2();
void funcRL2Q2();
void funcTVQ2();
void funcRL1CZ();
void funcRL2CZ();
void funcETRCZ();
void funcRL1SL();
void funcRL2SL();
void funcTVSL();

struct Comando{
  const char* nome;
  funcMain func;
};

Comando cmds[] = {
  {"AT", funcAUTON}, //comando para ir pro submenu automação
  {"TC", funcTCLN}, //comando pra ir pro submenu de teclado
  {"SA", funcSALA}, //comando para ir pras opções de sala
  {"Q1", funcQRT1}, //comando para ir pras opções de quarto 1
  {"Q2", funcQRT2}, //comando pra ir pras opções de quarto 2
  {"CO", funcCZNH}, //comando pra ir pras opções da cozinha
  {"VT", funcVM}, //comando para voltar pro menu principal
  {"L1Q1", funcRL1Q1}, //comando para selecionar a opção de rele 1 de quarto 1
  {"R1", funcRL2Q1}, //comando para selecionar a opção de rele 2 quarto 2
  {"PQ1", funcTVQ1}, //comando para selecionar opção de TV do quarto 1
  {"L1Q2", funcRL1Q2}, //comando para selecioar opção rele 1 quarto 2
  {"R2", funcRL2Q2}, //comando para selecionar opção do rele 2 quarto 2
  {"PQ2", funcTVQ2}, //comando para selecionar opção tv quarto 2
  {"L1C", funcRL1CZ}, //comando para selecionar opção rele 1 cozinha
  {"L2C", funcRL2CZ}, //comando para selecionar opção rele 2 cozinha
  {"RC", funcETRCZ}, // comano para seleconar eletrodoméstico de cozinha
  {"LS", funcRL1SL}, //comando para selecionar relé 1 sala
  {"L2S", funcRL2SL}, //comando para selecionar relé 2 sala
  {"RS", funcTVSL}, //comando para selecionar TV sala
  {"VM", funcVM}
};

#define Menu_Total 3
#define SubAuto_Total 4
#define SubMen_Total 2
#define opSala_Total 3
#define opQ1_Total 3
#define opQ2_Total 3
#define opCz_Total 3


//textos dos menus
const char menu_P_Com[] PROGMEM = "Comunicacao";
const char menu_P_Auto[] PROGMEM = "Automacao";
const char menu_P_Men[] PROGMEM = "Mensagem";

const char* const menuPrincipal[] PROGMEM = {menu_P_Com, menu_P_Auto, menu_P_Men};

const char SubAuto_Sl[] PROGMEM = "Sala";
const char SubAuto_Q1[] PROGMEM = "Quarto1";
const char SubAuto_Q2[] PROGMEM = "Quarto2";
const char SubAuto_Coz[] PROGMEM = "Cozinha";

const char* const subMenuAutomacao[] PROGMEM = {SubAuto_Sl, SubAuto_Q1, SubAuto_Q2, SubAuto_Coz};

const char SubMen_Perig[] PROGMEM = "Perigo";
const char SubMen_Nesce[] PROGMEM = "Nescessidade";

const char* const subMenuMensagem[] PROGMEM = {SubMen_Perig, SubMen_Nesce}; 
//teclado
const char  tecladoABC[28] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
//opções do submenu automação
const char opSala_Rl1[] PROGMEM = "Rele 1";
const char opSala_Rl2[] PROGMEM = "Rele 2";
const char opSala_Tv[] PROGMEM = "TV";

const char* const opSala[] PROGMEM = {opSala_Rl1, opSala_Rl2, opSala_Tv};

const char opQ1_Rl1[] PROGMEM = "Lamp 1";
const char opQ1_Rl2[] PROGMEM = "Rele";
const char opQ1_Tv[] PROGMEM = "PORTA";

const char* const opQuarto1[] PROGMEM = {opQ1_Rl1, opQ1_Rl2, opQ1_Tv};

const char opQ2_Rl1[] PROGMEM = "lamp 1";
const char opQ2_Rl2[] PROGMEM = "Rele";
const char opQ2_Tv[] PROGMEM = "PORTA";

const char* const opQuarto2[] PROGMEM = {opQ2_Rl1, opQ2_Rl2, opQ2_Tv};

const char opCz_Rl1[] PROGMEM = "Rele 1";
const char opCz_Rl2[] PROGMEM = "Rele 2";
const char opCz_Elt[] PROGMEM = "Eletro";

const char* const opCozinha[] PROGMEM = {opCz_Rl1, opCz_Rl2, opCz_Elt};

char buffer[15];

// Variáveis para controle de menu
uint8_t indice = 0;
uint8_t menuAtual = 0; //(Comunicação = 1 e automação = 2);
uint8_t opAtual = 0; //Será a opção, sendo respectivamente o que está nas funções acima, começando do 1
static bool emSub = false; //saber se está no submenu
static bool emOp = false; //saber se está nas opç~es de submenu
static bool digitar = false; //variável para saber se está no modo de digitação

void setup() {
  Cell.begin(9600); 
  Serial.begin(9600);
  Serial.println("Iniciando");
  oled.begin(&Adafruit128x64, 0x3C);
  if(!SD.begin(SD_CS)){
    Serial.println(F("Erro ao inciar SD"));
  }else{
    Serial.println(F("SD PRONTO!"));
  }
  oled.setFont(System5x7); //definindo a fonte //System5x7
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

  if(Cell.available() > 0){

    char recebido[8];
    int len = Cell.readBytesUntil('\n', recebido, 8);
    recebido[len] = '\0'; // completa o char para usar os métodos de C

    for(uint8_t x = 0; x < sizeof(cmds) / sizeof(cmds[0]); x++){ //fica repetindo 
      if(strcmp(recebido, cmds[x].nome) == 0){ //se o dado recebido for igual ao nome 
        cmds[x].func(); //executa a função
        break; //para o loop
      }
    }

    if(execut){
      Cell.println(F("OK")); //manda pro app que deu certo
      execut = false;
    }

  }

  digitalWrite(led, Condi);
  bot1();
  bot2();
  /*Serial.print("emSub: "); Serial.println(emSub);
  Serial.print("emOp: "); Serial.println(emOp);
  Serial.print("digitar: "); Serial.println(digitar);
  Serial.print("emSelecionar: "); Serial.println(emSelecionar);
  Serial.println("----------------------");*/
  delay(200);
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
          navegar(13);
        }
      }else{
        executar();
      }
    }else if (Contador == 3) {
      if(digitar){ //se estiver no teclado digitando, e estiver selecionando uma palavra indicada
        if(emSelecionar){
          
          snprintf(fraseFinal, sizeof(fraseFinal), "%s %s", fraseFinal, palavrasCorretas[indiceN]);
          memset(fraseDigitada, '\0', sizeof(fraseDigitada));
          mostrarTeclado();
        }else{
          executar();
        }
      }else{
        Condi = !Condi;
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
        navegar(-13);  
      }else{
        navegar(1);
      }
    }else if (Contador2 == 3 && Condi) {
      if(digitar){
        indiceN = -1;
        cont = 0;
        digitar = false;
        emSub = false;
        emSelecionar = false;
        oled.clear();
        oled.print(fraseFinal);
        Cell.print("MSG: "); //mandando a frase para o celular
        Cell.println(fraseFinal);
        delay(3000);
        memset(fraseDigitada, '\0', sizeof(fraseDigitada));
        memset(fraseFinal, '\0', sizeof(fraseFinal));
        indice = 0;
        mostrarMenu();
      }else{
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
          Cell.println("VOLTAR");
          mostrarMenu();
        }
      }
    }
    Contador2 = 0;
  }
}

void mostrarMenu() {
  oled.clear();

  oled.setCursor(0, 0);
  oled.print(F("Menu"));

  indice %= 3;

  for(uint8_t i = 0; i < 3; i++){

     strcpy_P(buffer, (char*)pgm_read_word(&(menuPrincipal[i])));

    uint8_t y = i + 1;
    if (i == indice) {
      oled.setInvertMode(true);
    } else {
      oled.setInvertMode(false);
    }
    oled.setCursor(0, y);
    oled.print(buffer);

  }

  memset(buffer, '\0', sizeof(buffer));
}

void mostrarSubMenu(){
  oled.clear();

  indice %= SubAuto_Total;

  for(uint8_t i = 0; i < SubAuto_Total; i++){

    uint8_t x = (i % Colunas) * Largura;
    uint8_t y = (i / Colunas) + 1;

    strcpy_P(buffer, (char*)pgm_read_word(&(subMenuAutomacao[i])));

    if(i == indice){
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(x, y);
    oled.print(buffer);
  }

  memset(buffer, '\0', sizeof(buffer));
}

void mostrarOp(){

  oled.clear();

  const char** opMenu; //vetor que vai receber outro vetor
  uint8_t tamanho; //saber quantas opções tem em cada struct

  switch (opAtual) {
    case 1:
      oled.setCursor(0, 0);
      oled.print(F("Sala-Auto"));
      opMenu = opSala; //aponta em que endereço da memória (flash) está oq tu quer
      tamanho = opSala_Total;
      Cell.println("SA"); //envia pro aplicativo que tá na opção se sala
    break;
    case 2:
      oled.setCursor(0, 0);
      oled.print(F("Quarto1-Auto"));
      opMenu = opQuarto1;
      tamanho = opQ1_Total;
      Cell.println("Q1"); //envia para o aplicativo que tá na opção de quarto 1
    break;
    case 3:
      oled.setCursor(0,0);
      oled.print(F("Quarto2-Auto"));
      opMenu = opQuarto2;
      tamanho = opQ2_Total;
      Cell.println("Q2"); //envia para o aplicativo que tá na opção de quarto 2
    break;
    case 4:
      oled.setCursor(0, 0);
      oled.print(F("Cozinha-Auto"));
      opMenu = opCozinha;
      tamanho = opCz_Total;
      Cell.println(F("CO")); //envia para o aplicativo que tá na opção da cozinha 
    break;
  }

  indice %= tamanho;

  for(uint8_t i = 0; i < tamanho; i++){
    uint8_t x = (i % Colunas) * Largura;
    uint8_t y = (i / Colunas) + 1;

    strcpy_P(buffer, (char*)pgm_read_word(&(opMenu[i])));

    if(i == indice){
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(x, y);
    oled.print(buffer); //vai imprimir a opção que tá no vetor
  }

  memset(buffer, '\0', sizeof(buffer));
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

  for(uint8_t i = 0; i < (sizeof(tecladoABC) / sizeof(tecladoABC[0])); i++){
    uint8_t x = (i % 13) * 8;
    uint8_t y = (i / 13) + 1;

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
    }else if(menuAtual == 2){
      opAtual = indice + 1; //se não, vai entrar na opção que selecinou no menu de automação
      emOp = true;
      emSub = false;
      indice = 0;
      mostrarOp();
    }else{
      opAtual = indice + 1; //entra na opção de mensagem que você selecionou
      indice = 0;
      mostrarMsg();
      mostrarSubMensagem();
    }
  }else{
    menuAtual = indice + 1; //se menuAutal = 1 comunicação, se = 2 é de automção
    emSub = true;
    emOp = false;
    indice = 0;
    if(menuAtual == 1){ //se estiver em comunicão, vai mostrar o teclado
      digitar = true; 
      mostrarTeclado();
      Cell.println("TEC");
    }else if(menuAtual == 2){
      mostrarSubMenu(); //se tiver em automação vai mostrar as subopções (Quarto1 etc) de automação
      Cell.println("AUT");
    }else{
      mostrarSubMensagem();
      Cell.println("BTMSG");
    }
  }

}

void mostrarSubMensagem(){
  oled.clear();

  indice %= SubMen_Total;

  for(uint8_t i = 0; i < SubMen_Total; i++){
    uint8_t x = (i % Colunas) * Largura;
    uint8_t y = (i / Colunas) + 1;

    strcpy_P(buffer, (char*)pgm_read_word(&(subMenuMensagem[i])));

    if(i == indice){
      oled.setInvertMode(true);
    }else{
      oled.setInvertMode(false);
    }
    oled.setCursor(x, y);
    oled.print(buffer);
  }

  memset(buffer, '\0', sizeof(buffer));
}

void mostrarMsg(){

  oled.clear();

  if(opAtual == 1){
    Cell.println("Estou em perigo");
    oled.print(F("Perigo"));
  }else if(opAtual == 2){
    Cell.println("Nescessito");
    oled.print(F("Ajuda"));
  }
  delay(500);
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
  dados[indice].state = !dados[indice].state;
  Cell.print(dados[indice].mensagem);
  delay(2000);
  oled.clear();
  memset(buffer, '\0', sizeof(buffer)); //esvazia o buffer novamente
  mostrarOp();
}

void executarFrase(){
  uint8_t len = strlen(fraseDigitada);
  fraseDigitada[len] = tecladoABC[indice];
  fraseDigitada[len + 1] = '\0';

  char nomeArquivo[8];

  snprintf(nomeArquivo, sizeof(nomeArquivo), "%c.txt", fraseDigitada[0]);

  File arquivo = SD.open(nomeArquivo);
  if(!arquivo){
    Serial.println(F("Erro ao abrir"));
    return;
  }

  contador = 0;
  char palavra[31];

  while(arquivo.available()){

    uint8_t i = 0;
    while(arquivo.available()){
      char c = arquivo.read();
      if(c == '\n' || c == '\r' || i > sizeof(palavra)){
        break;
      }
      palavra[i] = c;
      i++;
    }

    palavra[i] = '\0';

    if(strncmp(palavra, fraseDigitada, strlen(fraseDigitada)) == 0){
      strncpy(palavrasCorretas[contador], palavra, sizeof(palavrasCorretas[contador]));
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
    }else if(menuAtual == 2){
      mostrarSubMenu();
    }else{
      mostrarSubMensagem();
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

void funcVM(){
  menuAtual = 0;
  digitar = false;
  emOp = false;
  emSub = false;
  indice = 0;

  mostrarMenu();
}

void funcTCLN(){
  indice = 0;
  executar();
  execut = true;
}

void funcAUTON(){
  indice = 1;
  executar();
  execut = true;
}

//==============================================================Quarto 1===========================================

void funcQRT1(){
  indice = 1;
  executar();
  execut = true;
}

void funcRL1Q1(){
  indice = 0;
  executar();
  execut = true;
}

void funcRL2Q1(){
  indice = 1;
  executar();
  execut = true;
}

void funcTVQ1(){
  indice = 2;
  executar();
  execut = true;
}

//==============================================================Quarto 2===========================================

void funcQRT2(){
  indice = 2;
  executar();
  execut = true;
}

void funcRL1Q2(){
  indice = 0;
  executar();
  execut = true;
}

void funcRL2Q2(){
  indice = 1;
  executar();
  execut = true;
}

void funcTVQ2(){
  indice = 2;
  executar();
  execut = true;
}

//==============================================================Cozinha===========================================

void funcCZNH(){
  indice = 3;
  executar();
  execut = true;
}

void funcRL1CZ(){
  indice = 0;
  executar();
  execut = true;
}

void funcRL2CZ(){
  indice = 1;
  executar();
  execut = true;
}

void funcETRCZ(){
  indice = 2;
  executar();
  execut = true;
}

//==============================================================Sala===========================================

void funcSALA(){
  indice = 0;
  executar();
  execut = true;
}

void funcRL1SL(){
  indice = 0;
  executar();
  execut = true;
}

void funcRL2SL(){
  indice = 1;
  executar();
  execut = true;
}

void funcTVSL(){
  indice = 2;
  executar();
  execut = true;
}



