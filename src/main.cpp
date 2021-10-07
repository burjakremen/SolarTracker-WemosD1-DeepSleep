#include <Servo.h>

#define PrintLog Serial.println

const int sleepSeconds = 10;
const bool showlog = true;
const bool usedeepsleep = false;

Servo servoT; // Up/Down Motor
int angleservoT = 90;
const int servoTpin = 4; //(4)D2 Pin

Servo servoB; // Left/Right Motor
int angleservoB = 90;
const int servoBpin = 5; //(5)D1 Pin

// LDR pin connections
// name = analogpin;
int ldrTL = 14; //Pin D5 - LDR top left  - Верхний левый
int ldrTR = 12; //Pin D6 - LDR top rigt  - Верхний правый
int ldrBL = 13; //Pin D7 - LDR down left - Нижний левый
int ldrBR = 15; //Pin D8 - ldr down rigt - Нижний правый

int adcread(int pin)
{
   digitalWrite(pin,1);
   int value = analogRead(A0);
   digitalWrite (pin,0);
   return (value);
}


void setup()
{
  // servo connections
  if (showlog)
       {
         Serial.begin(115200);
       }
  servoT.attach(servoTpin);
  servoB.attach(servoBpin);
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
  int TR= adcread(ldrTR);  // Top-right
  int TL= adcread(ldrTL);  // Top-left
  int BL= adcread(ldrBL);  // Bottom-right
  int BR= adcread(ldrBR);  // Bottom-left

  int avTOP = (TL+TR)/2;  // average value top
  int avBOTTOM = (BL+BR)/2; // average value bottom
  int avLEFT = (BL+TL)/2; // average value left
  int avRIGHT = (BR+TR)/2;// average value right

  int dservoTOP = avTOP - avBOTTOM; // check the diffirence of top and bottom
  int dservoBOTTOM = avLEFT - avRIGHT;  // check the diffirence of left and rigt

   if (-100> dservoTOP  || dservoTOP > 100)
   {
   if( avTOP > avBOTTOM)
        {
        angleservoT--;
        if(angleservoT > 180)
           {
             angleservoT=180;
           }
        }
     else if(avBOTTOM > avTOP)
     {
        angleservoT++;
        if(angleservoT < 0)
        {
          angleservoT=0;
        }
     }
     servoT.write(angleservoT);
   }

   if(-100 > dservoBOTTOM || dservoBOTTOM > 100)
   {
    if(avRIGHT > avLEFT)
    {
      angleservoB++;
      if(angleservoB > 180)
        angleservoB=180;
    }
    else if(avLEFT>avRIGHT)
    {
      angleservoB--;
      if(angleservoB < 0)
        angleservoB=0;
    }
    servoB.write(angleservoB);
   }
  if (showlog)
       {
         PrintLog("Going to DeepSlep for:");
         PrintLog(sleepSeconds);
         PrintLog("seconds");
       }
  if (usedeepsleep)
     {
       ESP.deepSleep(sleepSeconds * 1000000);
     }
}
