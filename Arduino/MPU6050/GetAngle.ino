
#include <MPU6050_tockn.h>
#include <Kalman.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
uint32_t time;
float KalAngX, KalAngY, giroXAng, giroYAng, roll, pitch;
Kalman KalmanX;
Kalman KalmanY;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  getValue();

  KalmanX.setAngle(roll);
  KalmanY.setAngle(pitch);
}

void loop() {

  float deltaT = (float)(micros() - time) / 1000000;
  time = micros();
  
  getValue();

  KalAngX = KalmanX.getAngle(roll, giroXAng, deltaT);
  KalAngY = KalmanY.getAngle(pitch, giroYAng, deltaT);

  //Serial.print("angleX : ");
  //Serial.print("\tangleY : ");
  Serial.print(mpu6050.getGyroAngleX());
  Serial.print(" ");
  Serial.println(KalAngX);
}

void getValue(){

  mpu6050.update();

  pitch = mpu6050.getGyroAngleY();
  roll  = mpu6050.getGyroAngleX();
  giroXAng = mpu6050.getGyroX();
  giroYAng = mpu6050.getGyroY();

}
