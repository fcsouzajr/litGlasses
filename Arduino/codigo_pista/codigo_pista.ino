//
int led_vermelho = 8;
int led_amarelo = 7;
int led_verde = 6;


void setup()
{
  
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
}

void loop()

{
  digitalWrite(led_vermelho,HIGH);
  delay(2000);
  digitalWrite(led_vermelho,LOW);
  delay(2000);
  
 
  
  
  digitalWrite(led_verde,HIGH);
  delay(1000);
  digitalWrite(led_verde,LOW);
  digitalWrite(led_verde,0);
  delay(1000);
 
  
  
  
  digitalWrite(led_amarelo,HIGH);
  delay(500);
  digitalWrite(led_amarelo,LOW);
  digitalWrite(led_amarelo,0);
  delay(500);
 
  
  
  
}