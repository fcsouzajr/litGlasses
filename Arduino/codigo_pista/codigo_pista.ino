uint8_t pins_sensor[] = {2, 3, 4, 5};
uint8_t pins_output[] = {6, 7, 8, 9};

uint16_t init_time1 = 0;
uint16_t init_time2 = 0;

uint16_t total_time1 = 0;
uint16_t total_time2 = 0;

bool ant2 = true, ant3 = true, ant4 = true, ant5 = true;

void setup() {
  Serial.begin(9600);
  Serial.print("Começando");

  for(uint8_t x = 0; x < 4; x++){
    pinMode(pins_sensor[x], INPUT);
    pinMode(pins_output[x], OUTPUT);
  }

  for(uint8_t y = 0; y < 3; y++){
    digitalWrite(pins_output[y], HIGH);
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(300);
  }

}

void loop() {


  if(digitalRead(pins_sensor[0]) != ant2){
    init_time1 = millis();
  }
  if(digitalRead(pins_sensor[2]) != ant3){ 
    init_time2 = millis();
  }

  if(init_time1 > 0 && digitalRead(pins_sensor[1]) != ant4){
    total_time1 = millis() - init_time1;
    Serial.print("Tempo do carro 1: ");
    Serial.println(total_time1);
  }

  if(init_time2 > 0 && digitalRead(pins_sensor[3]) != ant5){
    total_time2 = millis() - init_time2;
    Serial.print("Tempo do carro 2: ");
    Serial.println(total_time2); 
  }

  ant2 = digitalRead(pins_sensor[0]);
  ant3 = digitalRead(pins_sensor[1]);
  ant4 = digitalRead(pins_sensor[2]);
  ant5 = digitalRead(pins_sensor[3]);
}
