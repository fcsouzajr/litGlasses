uint8_t pins_sensor[] = {2, 3, 4, 5};
uint8_t pins_output[] = {6, 7, 8, 9};

uint16_t init_time1 = 0;
uint16_t init_time2 = 0;

uint16_t total_time1 = 0;
uint16_t total_time2 = 0;

void setup() {
  Serial.begin(9600);

  for(uint8_t x = 0; x < 4; x++){
    pinMode(pins_sensor[x], INPUT);
    pinMode(pins_output[x], OUTPUT);
  }

  for(uint8_t y = 0; y < 3; y++){
    digitalWrite(pins_output[y], HIGH);
    digitalWrite(9, HIGH);
    delay(700);
    digitalWrite(9, LOW);
    delay(300);
  }

}

void loop() {
  if(pins_sensor[0] == LOW){
    init_time1 = millis();
  }
  if(pins_sensor[2] == LOW){ 
    init_time2 = millis();
  }

  if(init_time1 > 0 && pins_sensor[1] == LOW){
    total_time1 = millis() - init_time1;
    Serial.print("Tempo do carro 1: ");
    Serial.println(total_time1);
  }

  if(init_time2 > 0 && pins_sensor[3] == LOW){
    total_time2 = millis() - init_time2;
    Serial.print("Tempo do carro 2: ");
    Serial.println(total_time2); 
  }
}
