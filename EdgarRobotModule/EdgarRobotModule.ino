#include <Servo.h>
#include <SoftwareSerial.h>

#include "LedControlMS.h"
#include "robot.h"

byte arrows[5][8] = {
  {
    B00000000,
    B00001000,
    B00000100,
    B01111110,
    B01111110,
    B00000100,
    B00001000,
    B00000000,
  }, {
    B00000000,
    B00010000,
    B00100000,
    B01111110,
    B01111110,
    B00100000,
    B00010000,
    B00000000,
  }, {
    B00000000,
    B00011000,
    B00011000,
    B00011000,
    B01011010,
    B00111100,
    B00011000,
    B00000000,
  }, {
    B00000000,
    B00011000,
    B00111100,
    B01011010,
    B00011000,
    B00011000,
    B00011000,
    B00000000,
  }, {
    B00000000,
    B00011000,
    B00111100,
    B01110110,
    B01101110,
    B00111100,
    B00011000,
    B00000000,
  }
};

byte faces[9][8] = {
  {
    B00000000,
    B00000000,
    B00100100,
    B01000000,
    B01000000,
    B00100100,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B00100000,
    B01000100,
    B01000000,
    B00100000,
    B00000100,
    B00000000,
  }, {
    B00000000,
    B00000100,
    B00100000,
    B01000000,
    B01000100,
    B00100000,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B01000100,
    B00100000,
    B00100000,
    B01000100,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B00100100,
    B01100000,
    B01100000,
    B00100100,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B00100010,
    B01000000,
    B01000000,
    B00100010,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B00101000,
    B01000000,
    B01000000,
    B00101000,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B00100100,
    B01000000,
    B01000000,
    B10000100,
    B00000000,
    B00000000,
  }, {
    B00000000,
    B00000000,
    B10000100,
    B01000000,
    B01000000,
    B00100100,
    B00000000,
    B00000000,
  },
};

Servo servoLeft;
Servo servoRight;

LedControl lc = LedControl(10, 9, 8, 1);

const int RxD = 7;
const int TxD = 6;

char direct = ' ';
char last_direct = ' ';

SoftwareSerial btSerial(RxD, TxD);

int time;
int instructionTimeDelta = 0;
int lightsTimeDelta = 0;
int nextDelay = 0;
int instruction = 0;
int instructions[][2] = {
  {1, 1000},
  {2, 1000},
  {3, 790},
  {4, 790},
};
int numInstructions = sizeof(instructions) / (sizeof(int) * 2);

void setup() {
  Serial.begin(9600);
  btSerial.begin(38400);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  delay(2000);
  pinMode(5, OUTPUT);
  servoLeft.attach(12);
  servoRight.attach(13);
  Serial.flush();
  btSerial.flush();
  while (btSerial.available()) {
    btSerial.read();
  }
  stp();
}

void loop() {
  if (btSerial.available()) {
    direct = btSerial.read();
    delay(10);
    Serial.print("Recieved ");
    Serial.println(direct);
    if (!(direct == last_direct)) {
      switch (direct) {
        case '0': fwd(); break;
        case '1': bwd(); break;
        case '2': lft(); break;
        case '3': rgt(); break;
        case '4': stp(); break;
        case '5': sendBoop(); break;
      }
    }
    last_direct = direct;
  }
}
int getMicros(int dir) {
  return map(dir, -200, 200, 1300, 1700);
}

void sendBoop() {
  showFace(boop);
  tone(5, 880);
}
void fwd() {
  Serial.println("fwd");
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
  showFace(lookdown);
}
void bwd() {
  Serial.println("bwd");
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1700);
  showFace(lookup);
}
void lft() {
  Serial.println("lft");
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1300);
  showFace(lookleft);
}
void rgt() {
  Serial.println("rgt");
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);
  showFace(lookright);
}

void stp() {
  servoLeft.writeMicroseconds(1510);
  servoRight.writeMicroseconds(1510);
  noTone(5);
  showFace(happy);
}

void showArrow(dir arrow) {
  writeGrid(arrows[arrow]);
}
void showFace(face f) {
  writeGrid(faces[f]);
}
void writeGrid(byte grid[8]) {
  Serial.println("Writing leds");
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, grid[i]);
  }
}
