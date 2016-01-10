#include <SoftwareSerial.h>
#include <Wire.h>
#include <SFE_MMA8452Q.h>

MMA8452Q accel;

const int Rx = 7;
const int Tx = 6;
float x_offset;
float y_offset;
float z_offset;
bool is_up = true;
unsigned long last_activity;

SoftwareSerial btserial(Rx, Tx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  btserial.begin(38400);
  accel.init();
  Calibrate();

  Serial.flush();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (accel.available()) {
    accel.read();
    printCalculatedAccels();
    delay(20);
  }
}

void printCalculatedAccels()
{
  float x = accel.cx - x_offset;
  float y = accel.cy - y_offset;
  float z = accel.cz - z_offset;
  
  if (is_up == true && z >= 0.5) {
    is_up = false;
    Serial.println("Going Down");
    btserial.println(0);
    last_activity = millis();
  }

  if (is_up == false && z <= -0.5) {
    is_up = true;
    Serial.println("Going Up");
    btserial.println(0);
    last_activity = millis();
  }

  if (millis() - last_activity >= 500) {
    Serial.println("Stop");
    btserial.println(4);
  }

  btserial.print(x);
  btserial.print("\t");
  btserial.print(y);
  btserial.print("\t");
  btserial.print(z);
  btserial.println();
}

void Calibrate() {
  while (!accel.available()) {
    delay(10);
  }
  accel.read();
  x_offset = accel.cx;
  y_offset = accel.cy;
  z_offset = accel.cz;
}
