#include <Servo.h>

#define PrintLog Serial.println

const bool showlog = true; //true if need to show log to terminal
const bool usedeepsleep = false; // true if need use DeepSleep mode
const int  deepsleepSeconds = 10; // time in seconds for DeepSleep mode
const bool useinitposition = true; //if true - using variable initial position(servoh,servov) or use last servo position 
const int  maxVangle = 180; //maximum vertical position
const int  minVangle = 0; //minimum verticsl position
const int  maxHangle = 180; //maximum horizontal position
const int  minHangle = 0; //minimum horizontal position
const int  startHangle = 90;
const int  startVangle = 90;
int dtime = 1; // delay time
int tol = 30;  // tolerance

bool       cansleepV = false;
bool       cansleepH = false;

//Define ESP pin`s
const int servovpin = 4; //(4)D2 Pin - Vertical Motor
const int servohpin = 5; //(5)D1 Pin - Horizontal Motor
//   -----  LDR pin connections -------
int ldrTL = 14; //Pin D5 - LDR top left  - Верхний левый
int ldrTR = 12; //Pin D6 - LDR top rigt  - Верхний правый
int ldrBL = 13; //Pin D7 - LDR down left - Нижний левый
int ldrBR = 15; //Pin D8 - ldr down rigt - Нижний правый

//Define servos
Servo horizontal; // horizontal servo
int servoh; // initial horizontal position;
Servo vertical; // vertical servo
int servov; // initial vertical position;

int adcread(int pin)
{
   digitalWrite(pin,1);
   int value = analogRead(A0);
   if (showlog)
     {
         PrintLog ("Work ESP PIN:");
		     PrintLog (pin);
		     PrintLog ("ADC Value:");
         PrintLog (value);
     };
   digitalWrite (pin,0);
   return (value);
}

void setup()
{
  // servo connections
  if (showlog)
     {
         Serial.begin(115200);
     };
  vertical.attach(servovpin);
  horizontal.attach(servohpin);
  pinMode(ldrTL,OUTPUT);
  pinMode(ldrTR,OUTPUT);
  pinMode(ldrBL,OUTPUT);
  pinMode(ldrBR,OUTPUT);
  pinMode(D0, WAKEUP_PULLUP);
  if (useinitposition)
     {
		   servov = startVangle;
		   servoh = startHangle;
     };
}

void loop()
{
  if (showlog)
       {
         PrintLog("Start void loop()");
       };
  int rt = adcread(ldrTR);  // Top-right
  if (showlog)
     {
       PrintLog ("Top Right Value:");
		   PrintLog (rt);
	   };
  int lt = adcread(ldrTL);  // Top-left
  if (showlog)
     {
       PrintLog ("Top Left Value:");
		   PrintLog (lt);
	   };
  int ld = adcread(ldrBL);  // Bottom-left
  if (showlog)
     {
       PrintLog ("Bottom Left Value:");
		   PrintLog (ld);
	   };
  int rd = adcread(ldrBR);  // Bottom-right
  if (showlog)
     {
       PrintLog ("Bottom right Value:");
		   PrintLog (rd);
	   };


 int avt = (lt + rt) / 2; // average value top
 int avd = (ld + rd) / 2; // average value down
 int avl = (lt + ld) / 2; // average value left
 int avr = (rt + rd) / 2; // average value right

 int dvert = avt - avd; // check the diffirence of up and down
 int dhoriz = avl - avr;// check the diffirence og left and rigt

 if (-1*tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
     {
 	     cansleepV = false;
       if (avt > avd)
          {
             servov = ++servov;
             if (servov > maxVangle)
               {
                 servov = maxVangle;
               };
          }
       else if (avt < avd)
          {
             servov= --servov;
             if (servov < minVangle)
               {
                 servov = minVangle;
               };
          };
       if (showlog)
           {
             PrintLog ("Moving vertical value:");
	 	         PrintLog (servov);
	         };
 	     if (vertical.attached())
 	         {
             vertical.write(servov);
           }
     }
 else
     {
       cansleepV = true;
     }


 if (-1*tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
    {
	     cansleepH = false;
         if (avl > avr)
            {
               servoh = --servoh;
               if (servoh < minHangle)
                   {
                       servoh = minHangle;
                   };
            }
         else if (avl < avr)
            {
               servoh = ++servoh;
               if (servoh > maxHangle)
                   {
                       servoh = maxHangle;
                   };
            };
       if (showlog)
           {
             PrintLog ("Moving horizontal value:");
	  	       PrintLog (servoh);
	         };
       if (horizontal.attached())
 	         {
             horizontal.write(servoh);
		       }
    }
 else
    {
       cansleepH = true;
    }

 delay (dtime);

 if (cansleepH && cansleepV)
    {
       if (showlog)
         {
             PrintLog ("Destination position reached");
	       };
		   if (usedeepsleep)
	 	      {
			       if (showlog)
               {
                   PrintLog ("Deepsleep seconds:");
		               PrintLog (deepsleepSeconds);
	             };
             ESP.deepSleep(deepsleepSeconds * 1000000);
          };
    };
}
