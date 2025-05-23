<<<<<<< Updated upstream
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2C.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 8

SSD1306AsciiAvrI2c oled; //objeto da biblioteca para manipulação gráfica do display

SoftwareSerial Tx(10, 11); //Criando objeto para a biblioteca e se comunicar com o modulo bluetooth

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
};

opData Sala[] = {
  {"RELE 1", "0"},
  {"RELE 2", "1"},
  {"TV", "2"}
};

opData Quarto[] = {
  {"RELE 1", "3"},
  {"RELE 2", "4"},
  {"TV", "5"}
};

opData Cozinha[] = {
  {"RELE 1", "6"},
  {"RELE 2", "7"},
  {"Eletro", "8"}
};

//textos dos menus
const char* menuPrincipal[] = {"Comunicacao", "Automacao"};
const char* subMenuComunicacao[] = {"Digitar"};
const char* subMenuAutomacao[] = {"Sala", "Quarto", "Cozinha"};
//teclado
const char* tecladoABC []= {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L","M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"," "};
//opções do submenu automação
const char* opSala[] = {"Rele 1", "Rele 2", "TV"};
const char* opQuarto[] = {"Rele 1", "Rele 2", "TV"};
const char* opCozinha[] = {"Rele 1", "Rele 2", "Eletrodomestico"};

// Variáveis para controle de menu
uint8_t indice = 0;
uint8_t menuAtual = 0; //(Comunicação = 1 e automação = 2);
uint8_t opAtual = 0; //Será a opção, sendo respectivamente o que está nas funções acima, começando do 1
static bool emSub = false; //saber se está no submenu
static bool emOp = false; //saber se está nas opç~es de submenu
static bool digitar = false; //variável para saber se está no modo de digitação

void setup() {
  Tx.begin(9600); //definindo baund rate do modulo bluetooth
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
  uint8_t tamanho;

  switch (opAtual) {
    case 1:
      oled.setCursor(0, 0);
      oled.print(F("Sala-Auto"));
      opMenu = opSala;
      tamanho = sizeof(opSala) / sizeof(opSala[0]);
    break;
    case 2:
      oled.setCursor(0, 0);
      oled.print(F("Quarto-Auto"));
      opMenu = opQuarto;
      tamanho = sizeof(opQuarto) / sizeof(opQuarto[0]);
    break;
    case 3:
      oled.setCursor(0, 0);
      oled.print(F("Cozinha-Auto"));
      opMenu = opCozinha;
      tamanho = sizeof(opCozinha) / sizeof(opCozinha[0]);
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
    oled.print(opMenu[i]);
  }
=======
#include <Ultrasonic.h> // Inclui a biblioteca do ultrassônico

Ultrasonic ultrassom(7, 6); // Cria o objeto ultrassom, define os pinos onde está ligado o TRIG(pino 5) e o ECHO(pino 4)

int DF = 8;
int DT = 9;
int EF = 10;
int ET = 11;

long distancia; // Cria a variável distancia do tipo long

unsigned long lastMillis = 0; // Variável para armazenar o último tempo da leitura
unsigned long intervalo = 100; // Intervalo de tempo para ler o sensor (em milissegundos)

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial, com velocidade de comunicação de 9600
  pinMode(DF, OUTPUT);
  pinMode(DT, OUTPUT);
  pinMode(EF, OUTPUT);
  pinMode(ET, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis(); // Obtém o tempo atual
>>>>>>> Stashed changes

  // Verifica se já passou o intervalo desejado
  if (currentMillis - lastMillis >= intervalo) {
    lastMillis = currentMillis; // Atualiza o último tempo de leitura

    distancia = ultrassom.read(); // Obtém a distância em centímetros (usando o método read)
    Serial.print("Distância = "); // Imprime na serial o texto "Distância = "
    Serial.print(distancia); // Imprime na serial o valor da variável distancia
    Serial.println(" cm"); // Imprime na serial o texto "cm"

    if (distancia <= 30) {  // Correção do erro de comparação
      conferir();
    } else {
      frente();
    }
  }

  // Aqui você pode adicionar outras lógicas que não dependem da leitura do sensor
}

void conferir() {
  direita();
  
  if (distancia >= 30) {
    frente();
  } else if (distancia <= 30) {
    esquerda();
    delay(1000);
    esquerda();
    
    if (distancia >= 30) {
      frente();
    } else if (distancia <= 30) {
      direita();
      delay(1000);
      tras();
      delay(1000);
      direita();
      direita();
    }
  }
}

void direita() {
  digitalWrite(DT, HIGH);
  digitalWrite(ET, HIGH);
  digitalWrite(EF, LOW);
  digitalWrite(DF, LOW);
  delay(380);
  digitalWrite(DT, LOW);
  digitalWrite(EF, LOW);
  digitalWrite(ET, LOW);
  digitalWrite(DF, LOW);
}

void esquerda() {
  digitalWrite(DF, LOW);
  digitalWrite(ET, LOW);
  digitalWrite(EF, HIGH);
  digitalWrite(DT, HIGH);
  delay(380);
  digitalWrite(DT, LOW);
  digitalWrite(EF, LOW);
  digitalWrite(ET, LOW);
  digitalWrite(DF, LOW);
}

void frente() {
  digitalWrite(DF, LOW);
  digitalWrite(EF, LOW);
  digitalWrite(ET, HIGH);
  digitalWrite(DT, HIGH);
}

void tras() {
  digitalWrite(DT, LOW);
  digitalWrite(ET, LOW);
  digitalWrite(EF, HIGH);  // Corrigido o erro de parêntese
  digitalWrite(DF, HIGH);
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
  if(emOp){
    executarOp();
  }else if(emSub){
    if(digitar){
      executarFrase();
    }else{
      opAtual = indice + 1;
      emOp = true;
      emSub = false;
      indice = 0;
      mostrarOp();
    }
  }else{
    menuAtual = indice + 1;
    emSub = true;
    emOp = false;
    indice = 0;
    if(menuAtual == 1){
      digitar = true;
      mostrarTeclado();
    }else{
      mostrarSubMenu();
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
      dados = Quarto;
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
