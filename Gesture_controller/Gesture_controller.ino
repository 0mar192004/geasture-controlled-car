
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
int x , y;
Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("");
  delay(100);
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  x = a.acceleration.x;
  y = a.acceleration.y;


//  Serial.print("Acceleration X: ");
//  Serial.print(a.acceleration.x);
//  Serial.print(", Y: ");
//  Serial.print(a.acceleration.y);
//  Serial.print(", Z: ");
//  Serial.print(a.acceleration.z);
//  Serial.println(" m/s^2");



if(x >= -3 && x<= 3 && y >= -3 && y<= 3) {
  Serial.println("Stop");
  Serial.write("S");

  }
  else if(x >= -3 && x<= 3 && y <= -4){
      Serial.println("Forward");
      Serial.write("F");
    }
    else if(x >= -3 && x<= 3 && y >= 4){
      Serial.println("Backward");
      Serial.write("B");
    }
    else if(x >= 4 && y >= -3 && y<= 3){
      Serial.println("Left");
      Serial.write("L");
    }
    else if(x<= -4 && y >= -3 && y<= 3){
      Serial.println("Right");
      Serial.write("R");
    }
   
}
