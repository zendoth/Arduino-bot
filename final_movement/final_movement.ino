#include "DualVNH5019MotorShield.h"
//#include "SoftwareSerial.h"
#include <SharpIR.h>
#include <EnableInterrupt.h>
DualVNH5019MotorShield md;
//Definitions//
#define M1_PIN 3
#define M2_PIN 11
#define INTERVAL 100

double setRPM = 200.0;   //Desired RPM/120
double rkp = 1.13; //1.09208      //Kp value for Right Wheel PID
double rki = 0;       //Ki value for Right Wheel PID
double rkd = 0.15;//0.09;       //Kd value for Right Wheel PID 12
double lkp = 1.087;  //1.0168;       //Kp value for Left Wheel PID 16.5
double lki = 0;       //Ki value for Left Wheel PID
double lkd = 0.05;;       //Kd value for Left Wheel 10.5
double ruk0 = 0;        //u[k-1] for Right Wheel Digital PID
double ruk1 = 0;        //u[k] for Right Wheel Digital PID
double rek0 = 0;        //e[k-2] for Right Wheel Digital PID
double rek1 = 0;        //e[k-1] for Right Wheel Digital PID
double rek2 = 0;        //e[k] for Right Wheel Digital PID
double rek = 0;         //Computed Right Wheel Error Value
double luk0 = 0;        //u[k-1] for Left Wheel Digital PID
double luk1 = 0;        //u[k] for Left Wheel Digital PID
double lek0 = 0;        //e[k-2] for Left Wheel Digital PID
double lek1 = 0;        //e[k-1] for Left Wheel Digital PID
double lek2 = 0;        //e[k] for Left Wheel Digital PID
double lek = 0;         //Computed Left Wheel Error Value
double actualR = 200;   //Initial Right Wheel Speed before PID
double actualL = 200;   //Initial Left Wheel SPeed before PID
volatile int flag=0;
#define ledPin 13
#define modelshort 1080 //short
#define modellong 20150 //long
//A0: Front right, A1: Front left, A2: mid right, A3: mid left, A4: longLeft, A5:front mid
int frontRight= A0, frontLeft=A1, midRight=A2, midLeft=A3, longLeft=A4, frontMid=A5;
SharpIR SharpIR0(frontRight,modelshort);
SharpIR SharpIR1(frontLeft,modelshort);
SharpIR SharpIR2(midRight,modelshort);
SharpIR SharpIR3(midLeft,modelshort);
SharpIR SharpIR4(longLeft,modellong);
SharpIR SharpIR5(frontMid,modelshort);
volatile unsigned long m1_tick=0, m2_tick=0, m1_oldtick=0, m2_oldtick=0, m1_current=0, m2_current=0, m1_tickchange=0, m2_tickchange=0;
unsigned long time1, time2;
volatile int counter=0;
volatile int count;
void setup() {
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  pinMode(M1_PIN, INPUT);
  pinMode(M2_PIN, INPUT);
  attachPinChangeInterrupt(M1_PIN, m1_Interrupt, CHANGE);
  attachPinChangeInterrupt(M2_PIN, m2_Interrupt, CHANGE);
  md.init();
  count=1;
}
boolean start=true;

///////////////////////////////////////////PID code////////////////////////////////////////////////
void digitalPIDleft() {             //Digital PID implementation for Left Wheel
  double lk1 = lkp + lki + lkd;
  double lk2 = (-lkp) - (2 * lkd);
  double lk3 = lkd;
  lek = (lk1 * lek2) + (lk2 * lek1) + (lk3 * lek0);
  luk1 = luk0 + lek;
  luk0 = luk1; 
  lek0 = lek1;
  lek1 = lek2;
}

void realtimePID() {        //PID function to modify speed values using digital PID implementation

  //Perform digital PID for Right Wheel
  ruk1 = getRPM(1); //read encoder rpm into 'y' (data acquisition)
  rek2 = getRPM(1)-setRPM; //error between 'set_rpm' and 'y'
  digitalPIDright();
  actualR= rpmToPwm(ruk1);
  actualR*=-1;
  
  
  //Perform digital PID for Left Wheel
  luk1 = getRPM(2);
  lek2 = getRPM(2)-setRPM;
  digitalPIDleft();
  actualL = rpmToPwm(luk1);//lek;  
  actualL*=-1;
}


void rotate_counter(int degree){
  moveStop();
  delay(110);
  int total_count = 0;
  total_count = 8.851*degree-41.4;
  m1_oldtick = m1_tick;
  md.setM1Speed(-200);
  md.setM2Speed(-200);
  while((m1_tick - m1_oldtick)<total_count);
  moveStop();
  delay(50);
  return;
}
void rotate_clock(int degree){
  moveStop();
  delay(110);
  int total_count = 0;
  total_count = 8.851*degree-41.4;
  m1_oldtick = m1_tick;
  md.setM1Speed(200);
  md.setM2Speed(200);
  while((m1_tick - m1_oldtick)<total_count);
  moveStop();
  delay(50);
  return;
}
///////////////////////////////////////////////////////////////////////////////
boolean flagf,flagl;
char choice='Z';
int ccount;
int x=0;
void loop() { 

  while (Serial.available()>0){
    int dummy= Serial.peek();
      if(dummy>90||dummy<49){
        dummy= Serial.read();
        continue;
      }
    
    choice=Serial.read();
   
     switch (choice){
      case 'W'://forward
      while(count%62!=0){//63
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
      choice='Z';
      //delay(10);
      count=1;
        
      break;
      case 'S'://backwards
        moveBackwards();
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
        break;
      case 'R'://rotate clock
        //moveClock();
        rotate_clock(95);
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
      break;                          
      case 'L'://rotate counter
        //moveCounter();
        rotate_counter(94);
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4)); 
        break;
      case 'B':
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4)); 
        break;
      case 'C':
        ccount=0;
        while(flagf!=true){
          int l,r;
          l=medianvalue(SharpIR0);
          r=medianvalue(SharpIR1);
          flagf=calibrate(l,r,ccount);
          ccount++;
        }
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
        flagf=false;
        break;
       case 'T':
       while((medianvalue(SharpIR0)>14)||(medianvalue(SharpIR5)>14)||(medianvalue(SharpIR1)>14)||(medianvalue(SharpIR0)==-1)||(medianvalue(SharpIR5)==-1)||(medianvalue(SharpIR1)==-1)){
          choice= moveForward();
       }
       moveStop();
       ccount=0;
        while(flagf!=true){
          flagf=calibrate(medianvalue(SharpIR0),medianvalue(SharpIR1),ccount);
          ccount++;
        }
        flagf=false;
       break;
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
        break;
      default://else dont move
        moveStop();
      break;
    }
  }

}
