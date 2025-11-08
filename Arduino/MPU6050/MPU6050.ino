// Start the simulation, click on the MPU6050 part, and
// change the x/y/z values to see changes in the plotter.

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Kalman.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
uint32_t time;

Kalman KalmanX;
Kalman KalmanY;

float KalAngX, KalAngY, giroXAng, giroYAng;
float pitch, roll;

void setup() {
  Serial.begin(9600);

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  getValue();

  KalmanX.setAngle(roll);
  KalmanY.setAngle(pitch);

  giroXAng = roll;
  giroYAng = pitch;

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  time = micros();
}

void loop() {

  float deltaT = (float)(micros() - time) / 1000000;
  time = micros();

  getValue();

  KalAngX = KalmanX.getAngle(roll, giroXAng, deltaT);
  KalAngY = KalmanY.getAngle(pitch, giroYAng, deltaT);

  Serial.print("°, Roll: ");
  Serial.println(roll);
  Serial.println("°");
  Serial.print("Filtro X");
  Serial.println(KalAngX);
  Serial.println(KalAngY);

}

void getValue(){

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  pitch = atan(a.acceleration.x / sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * RAD_TO_DEG;
  roll  = atan(a.acceleration.y / sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * RAD_TO_DEG;
  giroXAng = g.gyro.x * (180 / PI); //Converter para graus/s
  giroYAng = g.gyro.y * (180 / PI);

}
