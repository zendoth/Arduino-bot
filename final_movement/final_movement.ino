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
double rkp = 1.18; //1.09208      //Kp value for Right Wheel PID
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
volatile int total_count=0;
int d0,d1,d2,d3,d4,d5;
void setup() {
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  pinMode(M1_PIN, INPUT);
  pinMode(M2_PIN, INPUT);
  attachPinChangeInterrupt(M1_PIN, m1_Interrupt, HIGH);
  attachPinChangeInterrupt(M2_PIN, m2_Interrupt, HIGH);
  md.init();
  count=1;
}
boolean start=true;
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
      while(count%65!=0){//63
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      d0=modeValue(SharpIR0,0);
      delay(10);
      d1=modeValue(SharpIR5,5);
      delay(10);
      d2=modeValue(SharpIR1,1);
      delay(10);
      d3=modeValue(SharpIR2,2);
      delay(10);
      d4=modeValue(SharpIR3,3);
      delay(10);
      d5=modeLong(SharpIR4);
      delay(10);
        printSensor(d0,d1,d2,d3,d4,d5); 
      choice='Z';
      //delay(10);
      count=1;
      break;
      case 'S'://backwards
        moveBackwards();
      d0=modeValue(SharpIR0,0);
      delay(10);
      d1=modeValue(SharpIR5,5);
      delay(10);
      d2=modeValue(SharpIR1,1);
      delay(10);
      d3=modeValue(SharpIR2,2);
      delay(10);
      d4=modeValue(SharpIR3,3);
      delay(10);
      d5=modeLong(SharpIR4);
      delay(10);
        printSensor(d0,d1,d2,d3,d4,d5); 
        break;
      case 'R'://rotate clock
        rotate_clock(94);
      d0=modeValue(SharpIR0,0);
      delay(10);
      d1=modeValue(SharpIR5,5);
      delay(10);
      d2=modeValue(SharpIR1,1);
      delay(10);
      d3=modeValue(SharpIR2,2);
      delay(10);
      d4=modeValue(SharpIR3,3);
      delay(10);
      d5=modeLong(SharpIR4);
      delay(10);
        printSensor(d0,d1,d2,d3,d4,d5); 
      break;                          
      case 'L'://rotate counter
        rotate_counter(95);
      d0=modeValue(SharpIR0,0);
      delay(10);
      d1=modeValue(SharpIR5,5);
      delay(10);
      d2=modeValue(SharpIR1,1);
      delay(10);
      d3=modeValue(SharpIR2,2);
      delay(10);
      d4=modeValue(SharpIR3,3);
      delay(10);
      d5=modeLong(SharpIR4);
      delay(10);
        printSensor(d0,d1,d2,d3,d4,d5); 
        break;
      case 'B':
      d0=modeValue(SharpIR0,0);
      delay(10);
      d1=modeValue(SharpIR5,5);
      delay(10);
      d2=modeValue(SharpIR1,1);
      delay(10);
      d3=modeValue(SharpIR2,2);
      delay(10);
      d4=modeValue(SharpIR3,3);
      delay(10);
      d5=modeLong(SharpIR4);
      delay(10);
        printSensor(d0,d1,d2,d3,d4,d5); 
        break;
      case 'C':
        ccount=0;
        while(flagf!=true&&total_count<500){
          int l,r;
          l=modeCalibrate(SharpIR0);
          r=modeCalibrate(SharpIR1);
          flagf=calibrate(l,r,ccount);
          ccount++;
          total_count++;
        }
        total_count=0;
      d0=modeValue(SharpIR0,0);
      delay(10);
      d1=modeValue(SharpIR5,5);
      delay(10);
      d2=modeValue(SharpIR1,1);
      delay(10);
      d3=modeValue(SharpIR2,2);
      delay(10);
      d4=modeValue(SharpIR3,3);
      delay(10);
      d5=modeLong(SharpIR4);
      delay(10);
        printSensor(d0,d1,d2,d3,d4,d5); 
        flagf=false;
        break;
        case 'D'://rotate clock
        rotate_clock(94);
        Serial.println("pscan|");
        break;                          
        case 'A'://rotate counter
        rotate_counter(95);
        Serial.println("pscan|");
        break;
        case 'G':
        while(count%(65)!=0){//1 grids
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      Serial.println("pscan|");
      choice='Z';
      count=1;
      break;
        case 'H':
        while(count%(65*2)!=0){//2 grids
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      Serial.println("pscan|");
      choice='Z';
      count=1;
      break;
        case 'I':
        while(count%((65*3)+3)!=0){//3 grids
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      Serial.println("pscan|");
      choice='Z';
      count=1;
      break;
        case 'J':
        while(count%((65*4)+4)!=0){//4 grids
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      Serial.println("pscan|");
      choice='Z';
      count=1;
      break;
        case 'K':
        while(count%((65*5)+10)!=0){//63
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      Serial.println("pscan|");
      choice='Z';
      count=1;
      break;
        case 'M':
        while(count%((65*6)+12)!=0){//6 grids
        choice=moveForward();
        realtimePID();
        count++;
      }
      actualR=200;
      actualL=200;
      moveStop();
      Serial.println("pscan|");
      choice='Z';
      count=1;
      break;
      default://else dont move
        moveStop();
      break;
    }
  }

}
