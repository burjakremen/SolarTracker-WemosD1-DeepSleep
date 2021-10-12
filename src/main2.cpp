#include <Servo.h>

#define PrintLog Serial.println

const int sleepSeconds = 10;
const bool showlog = true;
const bool usedeepsleep = false;

int dtime = 1; // read potentiometers
int tol = 30;

//Servo servov; // Up/Down Motor
//int angleservoT = 0;
const int servovpin = 4; //(4)D2 Pin

//Servo servoh; // Left/Right Motor
int angleservoB = 0;
const int servohpin = 5; //(5)D1 Pin

Servo horizontal; // horizontal servo
int servoh = 90; // stand horizontal servo

Servo vertical; // vertical servo 
int servov = 90; // stand vertical servo

// LDR pin connections
// name = analogpin;
int ldrTL = 14; //Pin D5 - LDR top left  - Верхний левый
int ldrTR = 12; //Pin D6 - LDR top rigt  - Верхний правый
int ldrBL = 13; //Pin D7 - LDR down left - Нижний левый
int ldrBR = 15; //Pin D8 - ldr down rigt - Нижний правый

int adcread(int pin)
{
   digitalWrite(pin,1);
   //delay(100);
   int value = analogRead(A0);
   //delay(1000);
   PrintLog (pin);
   PrintLog (value);
   digitalWrite (pin,0);
   //delay(100);
   return (value);
}


void setup()
{
  // servo connections
  if (showlog)
       {
         Serial.begin(115200);
       }
  vertical.attach(servovpin);
  horizontal.attach(servohpin);
  //servoB.write(0);
  //servoT.write(0);
  pinMode(ldrTL,OUTPUT);
  pinMode(ldrTR,OUTPUT);
  pinMode(ldrBL,OUTPUT);
  pinMode(ldrBR,OUTPUT);
  pinMode(D0, WAKEUP_PULLUP);
}

void loop()
{
  if (showlog)
       {
         PrintLog("Start void loop()");
       }
  int rt= adcread(ldrTR);  // Top-right
//  PrintLog (TR);
  int lt= adcread(ldrTL);  // Top-left
//  PrintLog (TL);
  int ld= adcread(ldrBL);  // Bottom-right
//  PrintLog (BL);
  int rd= adcread(ldrBR);  // Bottom-left
//  PrintLog (BR);



int avt = (lt + rt) / 2; // average value top
int avd = (ld + rd) / 2; // average value down
int avl = (lt + ld) / 2; // average value left
int avr = (rt + rd) / 2; // average value right

int dvert = avt - avd; // check the diffirence of up and down
int dhoriz = avl - avr;// check the diffirence og left and rigt

if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
{
if (avt > avd)
{
servov = ++servov;
if (servov > 180)
{
servov = 180;
}
}
else if (avt < avd)
{
servov= --servov;
if (servov < 0)
{
servov = 0;
}
}
vertical.write(servov);
}

if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
{
if (avl > avr)
{
servoh = --servoh;
if (servoh < 0)
{
servoh = 0;
}
}
else if (avl < avr)
{
servoh = ++servoh;
if (servoh > 180)
{
servoh = 180;
}
}
else if (avl == avr)
{
// nothing
}
horizontal.write(servoh);
}
delay(dtime);
} 
