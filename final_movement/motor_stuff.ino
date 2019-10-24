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

char moveForward(){
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
  while(i<1380){//1342
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
  while(i<1515){
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


boolean calibrate(int r, int l,int count){
  l+=0;
  r+=0;
  if((r-0)>8&&l>8){
       md.setM1Speed(-100);
       md.setM2Speed(100);
       count=0;
       delay(110);
    moveStop();
     }
    if(((r-0)<8&&l<8)){
      md.setM1Speed(100);
      md.setM2Speed(-100); 
      count=0;
      delay(110);
    moveStop();
    }
    if(((r-0)==8&&l!=8)||(count>10)){
      if(l<(r-0)){
        md.setM2Speed(-100);
        md.setM1Brake(400);
        count=0;
        delay(110);
    moveStop();
      }
      if(l>(r-0)){
        md.setM2Speed(100);
        md.setM1Brake(400);
        count=0;
        delay(110);
    moveStop();
      }
    }
    if((r-0)!=8&&l==8){
      if(l>(r-0)){
        md.setM1Speed(100);
        md.setM2Brake(400);
        count=0;
        delay(110);
    moveStop();
      }
      if(l<(r-0)){
        md.setM1Speed(-100);
        md.setM2Brake(400);
        count=0;
        delay(108);
    moveStop();
      }
     } 
    if((r-0)==8&&l==8){
      md.setM2Speed(100);
      delay(115);
      moveStop();
      count=0;
      return true;
    }
return false;
}
