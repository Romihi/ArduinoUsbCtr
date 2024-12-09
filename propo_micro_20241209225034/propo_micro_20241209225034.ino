#include "Joystick.h"

#define CHAN1 0
#define CHAN2 1
#define CHAN3 2
#define CHAN4 3

// Constant that maps the phyical pin to the joystick button.
const int pinToButtonMap = 2;

// Last state of the button
int lastButtonState[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#define buttonCh3 1 //B
#define buttonCh4 11 //start

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  16, 0,                 // Button Count, Hat Switch Count
  true, false, true,     // X, Y, Z
  false, true, false,  // Rx, Ry, Rz
  false, false,         // Rudder, Throttle
  false, false, false); // Accelerator, Brake, Steering
  
volatile unsigned int ch1Value = 1500;
volatile unsigned int ch2Value = 1500;
volatile unsigned int ch3Value = 1500;
volatile unsigned int ch4Value = 1500;
volatile unsigned int ch1 = 1500;
volatile unsigned int ch2 = 1500;
volatile unsigned int ch3 = 1500;
volatile unsigned int ch4 = 1500;
volatile bool ch1Flg = false;
volatile bool ch2Flg = false;
volatile bool ch3Flg = false;
volatile bool ch4Flg = false;
volatile unsigned long time1;
volatile unsigned long time2;
volatile unsigned long time3;
volatile unsigned long time4;

void setup() {
  Joystick.setXAxisRange(1000,2000);//プロポによっては 750～2250ぐらいかも
  //Joystick.setYAxisRange(1000,2000);
  Joystick.setRyAxisRange(1000,2000);
  Joystick.setZAxisRange(1000,2000);
  Joystick.setXAxis(ch1Value);
  //Joystick.setYAxis(ch1Value);
  Joystick.setRyAxis(ch1Value);
  Joystick.setZAxis(ch1Value);

  pinMode(CHAN1, INPUT_PULLUP);
  pinMode(CHAN2, INPUT_PULLUP);
  pinMode(CHAN3, INPUT_PULLUP);
  pinMode(CHAN4, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  Joystick.begin();

  attachInterrupt(digitalPinToInterrupt(CHAN1), isrPin1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHAN2), isrPin2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHAN3), isrPin3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHAN4), isrPin4, CHANGE);

}

void isrPin1()
{
  unsigned long timeNow = micros();
  if (digitalRead(CHAN1)){
    time1 = timeNow;
  }
  else if (timeNow > time1)
  {
    ch1Value = (timeNow - time1);
    ch1Flg = true;
  }
}

void isrPin2()
{
  unsigned long timeNow = micros();
  if (digitalRead(CHAN2)){
    time2 = timeNow;
  }
  else if (timeNow > time2)
  {
    ch2Value = (timeNow - time2);
    ch2Flg = true;
  }
}

void isrPin3()
{
  unsigned long timeNow = micros();
  if (digitalRead(CHAN3)){
    time3 = timeNow;
  }
  else if (timeNow > time3)
  {
    ch3Value = (timeNow - time3);
    ch3Flg = true;
  }
}

void isrPin4()
{
  unsigned long timeNow = micros();
  if (digitalRead(CHAN4)){
    time4 = timeNow;
  }
  else if (timeNow > time4)
  {
    ch4Value = (timeNow - time4);
    ch4Flg = true;
  }
}


void loop() {
    if (ch1Flg)
    {
      ch1Flg = false;
      ch1 = (ch1 + ch1Value) / 2;
      Joystick.setXAxis(ch1);
      //Serial.print("CH1");
      //Serial.println(ch1Value);
    }
  
    if (ch2Flg)
    {
      ch2Flg = false;
      ch2 = (ch2 + ch2Value) / 2;
      //Joystick.setYAxis(ch2);
      Joystick.setRyAxis(ch2);
      //Serial.print("CH2");
      //Serial.println(ch2Value);
    }
    if (ch3Flg)
    {
      ch3Flg = false;
      ch3 = (ch3 + ch3Value) / 2;
      Joystick.setZAxis(ch3);
      //Joystick.setButton(buttonCh3, lastButtonState[buttonCh3]);
      if (ch3 > 1000 && lastButtonState[buttonCh3]){
        Joystick.setButton(buttonCh3, 1);
        }
      else if (ch3 < 1000 && !lastButtonState[buttonCh3]){
        Joystick.setButton(buttonCh3, 0);
        }
      lastButtonState[buttonCh3] = !lastButtonState[buttonCh3];
      //Serial.print("CH3");
      //Serial.println(ch3Value);
    }

    if (ch4Flg)
    {
      ch4Flg = false;
      ch4 = (ch4 + ch4Value) / 2;
      if (ch4 > 1000 && lastButtonState[buttonCh4]){
        Joystick.setButton(buttonCh4, 1);
        }
      else if (ch4 < 1000 && !lastButtonState[buttonCh4]){
        Joystick.setButton(buttonCh4, 0);
        }
      lastButtonState[buttonCh4] = !lastButtonState[buttonCh4];
      //Serial.print("CH4");
      //Serial.println(ch4Value);
    }
    
    //ここの数値を小さくするとFPSが上がります。３でFPS400ぐらい
    delay(3);
}
