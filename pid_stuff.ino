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
