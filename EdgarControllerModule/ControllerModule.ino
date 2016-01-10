#include <SoftwareSerial.h>

#define X_PIN A0
#define Y_PIN A1
int x_val;
int y_val;
int last_command;
byte buttons = B00000000;

const int Rx = 7;
const int Tx = 6;

SoftwareSerial btserial(Rx, Tx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  btserial.begin(38400);

  delay(1000);
  Serial.flush();
  btserial.flush();

  while (btserial.available()) {
    btserial.read();
  }
}

void loop() {
  x_val = analogRead(X_PIN); // 500
  y_val = analogRead(Y_PIN); // 521
  if (y_val > 1000) {
    if (last_command != 0) {
      btserial.write('0');
      last_command = 0;
    }
  } else if (y_val < 20) {
    if (last_command != 1) {
      btserial.write('1');
      last_command = 1;
    }
  } else if (x_val > 1000) {
    if (last_command != 2) {
      btserial.write('2');
      last_command = 2;
    }
  } else if (x_val < 20) {
    if (last_command != 3) {
      btserial.write('3');
      last_command = 3;
    }
  } else if (!digitalRead(8)) {
    if (last_command != 5) {
      btserial.write('5');
      last_command = 5;
    }
  } else {
    if (last_command != 4) {
      btserial.write('4');
      last_command = 4;
    }
  }
  delay(5);
}
