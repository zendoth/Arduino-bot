#include "DualVNH5019MotorShield.h"
#include "SoftwareSerial.h"
#include <SharpIR.h>
DualVNH5019MotorShield md;


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
volatile int counter=0;
volatile int count;
void setup() {
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
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
      while(count%54!=0){//63
        choice=moveForward(count);
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
        moveClock();
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
      break;                          
      case 'L'://rotate counter
        moveCounter();
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4)); 
        break;
      case 'B':
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4)); 
        break;
      case 'C':
        ccount=0;
        while(flagf!=true){
          flagf=calibrate(medianvalue(SharpIR0),medianvalue(SharpIR5),medianvalue(SharpIR1),ccount);
          ccount++;
        }
        printSensormedian(medianvalue(SharpIR0), medianvalue(SharpIR5),medianvalue(SharpIR1),medianvalue(SharpIR2),medianvalue(SharpIR3),medianLong(SharpIR4));
        flagf=false;
        break;
      case 'K':
        while(flagl!=true){
          flagl=leftCalibrate(getdistance(SharpIR3),getdistance(SharpIR4));
        }
        flagl=false;
        break;
      default://else dont move
        moveStop();
      break;
    }
  }

}
