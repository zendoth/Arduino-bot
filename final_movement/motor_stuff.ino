float getRPM(int motor){
  double pin;
  float rpm=0;
  if(motor==1){//for motor 1
    pin=pulseIn(3,HIGH);
    rpm= (1/(((2*pin)/1000000)/60))/562.25;
    return rpm;
  }
  if(motor==2){
    pin=pulseIn(11,HIGH);
    rpm= (1/(((2*pin)/1000000)/60))/562.25;
    return rpm;
  }
}

char moveForward(int count){
  choice='W';
  md.setM1Speed(-actualR);
  md.setM2Speed(actualL);
    return choice;
}

void moveBackwards(){
  md.setM1Speed(actualR);
  md.setM2Speed(-actualL);
}

void moveClock(){
  int i=0;
  while(i<1342){//1240
    Serial.println("ello");
    md.setM1Speed(200);
    md.setM2Speed(200);
    i++;
  }
    md.setM1Brake(400);
    md.setM2Brake(400);
    delay(50);
}

void moveCounter(){
  int i=0;
  while(i<1500){//1320
    Serial.println("ello");
    md.setM1Speed(-200);
    md.setM2Speed(-200);
    i++;
  }
    md.setM1Brake(400);
    md.setM2Brake(400);
    delay(50);
}

void moveStop(){
  md.setM1Brake(400);
  md.setM2Brake(400);
}

double rpmToPwm(double rpm){
  double pwm= map(rpm,27.73403,103.6323,100,300);
  return pwm;
}

void digitalPIDright() {            //Digital PID implementation for Right Wheel
  double rk1 = rkp + rki + rkd;
  double rk2 = (-rkp) - (2 * rkd);
  double rk3 = rkd;
  rek = (rk1 * rek2) + (rk2 * rek1) + (rk3 * rek0); // all the k1,k2,k3 multiplied with error.
  ruk1 = ruk0 + rek; //u[k]= u[k-1]+ (all the k1,k2,k3 multiplied with error)
  ruk0 = ruk1;//saves the new u[k] into an older u[k-1] for next cycle
  rek0 = rek1; //saves error from 2 cycles ago into e[k-2]
  rek1 = rek2; //saves latest error into e[k-1]
}

boolean calibrate(int r, int m,int l,int count){
 // if(r==-1||l==-1){
  //  Serial.println("Whatswrong?");
    //return true;
 // }
         if((r-1)>8&&l>8){
       md.setM1Speed(-100);
       md.setM2Speed(100);
       count=0;
     }
    if(((r-1)<8&&l<8)){
      md.setM1Speed(100);
      md.setM2Speed(-100); 
      count=0;
    }
    if(((r-1)==8&&l!=8)){
      if(l<(r-1)){
        md.setM2Speed(-100);
        md.setM1Brake(300);
        count=0;
      }
      if(l>(r-1)){
        md.setM2Speed(100);
        md.setM1Brake(300);
        count=0;
      }
    }
    if((r-1)!=8&&l==8){
      if(l>(r-1)){
        md.setM1Speed(100);
        md.setM2Brake(300);
        count=0;
      }
      if(l<(r-1)){
        md.setM1Speed(-100);
        md.setM2Brake(300);
        count=0;
      }
     } 
    if((r-1)==8&&l==8){
      moveStop();
      count=0;
      delay(10);
      return true;
    }
return false;
}

boolean leftCalibrate(int s, int l){ 
  if(s==-1||l==-1){
    return true;
  }
  s+=1;
  if(s>l){
    md.setM2Speed(-100);
  }
  if(l>s){
    md.setM2Speed(100);
  }
  if(l==s){
    md.setM2Brake(300);
    return true;
  }
  return false;
}
