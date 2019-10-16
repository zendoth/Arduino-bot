
int getdistance(SharpIR x){
  int d;
  d=x.distance();
  if(d<7||d>44){
    d=-1;
  }
  return d;
}

int getLdistance(SharpIR x){
  int d;
  d=x.distance();
  if(d<=45||d>100){
    d=-1;
  }
  return d;
}

void serialFlush(){
  while(Serial.available()>0){
    char t=Serial.read();
  }
}

int distancetoGrid(int d){
  d+=5;
  d/=10;
  return d;
}

int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}

int medianvalue(SharpIR x){
  int list[10];
  int i=0;
  for(i=0;i<10;i++){
    list[i]=getdistance(x);
  }
  int list_length = sizeof(list) / sizeof(list[0]);
  qsort(list, list_length, sizeof(list[0]), sort_desc);
  return list[5];
}

int modevalue(SharpIR x){
  int list[10];
  int i =0;
    for(i=0;i<10;i++){
    list[i]=getdistance(x);
  }
  int list_length = sizeof(list) / sizeof(list[0]);
  int maxVal = 0;
  int maxCnt = 0;
  i =0;
  int j =0;

  for (i=0; i <list_length; ++i){
      int count = 0;
      for (j=0; j < list_length; j++) {
        if(list[j]== list[i])
        ++count;
      }
      if (count > maxCnt) {
        maxCnt = count;
        maxVal = list[i];
      }
    }

  return maxVal;
}

int modeLong(SharpIR x){
  int list[10];
  int i =0;
    for(i=0;i<10;i++){
    list[i]=getLdistance(x);
  }
  int list_length = sizeof(list) / sizeof(list[0]);
  int maxVal = 0;
  int maxCnt = 0;
  i =0;
  int j =0;

  for (i=0; i <list_length; ++i){
      int count = 0;
      for (j=0; j < list_length; j++) {
        if(list[j]== list[i])
        ++count;
      }
      if (count > maxCnt) {
        maxCnt = count;
        maxVal = list[i];
      }
    }

  return maxVal;
}
void printSensor(int ifr, int ifm, int ifl, int imr, int iml, int ill){
  String sfr, sfm, sfl, smr,sml,sll;
   if(ifr==-1){
    sfr= String(ifr);
   }
   else{
    ifr+=0;//3
    if(ifr<5){
      sfr="-1";
    }
    else{
      sfr= String(distancetoGrid(ifr));
    }
   }


   if(ifm==-1){
    sfm= String(ifm);
   }
   else{ 
    ifm+=0;//2
    if(ifm<5){
      sfm="-1";
    }
    else{
      sfm= String(distancetoGrid(ifm));
    }
   }

   if(ifl==-1){
    sfl= String(ifl);
   }
   else{
    ifl+=0;//3
    if(ifl<5){
      sfl="-1";
    }
    else{
      sfl= String(distancetoGrid(ifl));
    }
   }

   if(imr==-1){
    smr= String(imr);
   }
   else{
    imr+=2;//7
    if(imr<5){
      smr="-1";
    }
    else{
      smr= String(distancetoGrid(imr));
    }
   }

   if(iml==-1){
    sml= String(iml);
   }
   else{
    iml-=4;//7
    if(iml<5){
      sml="-1";
    }
    else{
      sml= String(distancetoGrid(iml));
    }
   }

   if(ill==-1){
    sll=String(ill);
   }
   else{
      ill-=0;//6
      sll= String(distancetoGrid(ill));
   }

   Serial.print(ifr);//front right
   Serial.print(" ");
   Serial.print(ifm);//front mid
   Serial.print(" ");
   Serial.print(ifl);//front left
   Serial.print(" ");
   Serial.print(imr);//mid right
   Serial.print(" ");
   Serial.print(iml);//mid left
   Serial.print(" ");
   Serial.println(ill);//long left 


   Serial.print("pscan ");
   Serial.print(sfr);//front right
   Serial.print(" ");
   Serial.print(sfm);//front mid
   Serial.print(" ");
   Serial.print(sfl);//front left
   Serial.print(" ");
   Serial.print(smr);//mid right
   Serial.print(" ");
   Serial.print(sml);//mid left
   Serial.print(" ");
   Serial.println(sll);//long left 


}
/*
void printSensor(SharpIR fr,SharpIR fm,SharpIR fl,SharpIR mr,SharpIR ml,SharpIR ll){
   String sfr, sfm, sfl, smr,sml,sll;
   int ifr = getdistance(SharpIR0);
   if(ifr==-1){
    sfr= String(ifr);
   }
   else{
    ifr-=3;
    if(ifr<5){
      sfr="-1";
    }
    else{
      sfr= String(distancetoGrid(ifr));
    }
   }


   int ifm = getdistance(SharpIR5);
   if(ifm==-1){
    sfm= String(ifm);
   }
   else{ 
    ifm-=2;
    if(ifm<5){
      sfm="-1";
    }
    else{
      sfm= String(distancetoGrid(ifm));
    }
   }

   
   int ifl = getdistance(SharpIR1);
   if(ifl==-1){
    sfl= String(ifl);
   }
   else{
    ifl-=3;
    if(ifl<5){
      sfl="-1";
    }
    else{
      sfl= String(distancetoGrid(ifl));
    }
   }

   
   int imr = getdistance(SharpIR2);
   if(imr==-1){
    smr= String(imr);
   }
   else{
    imr-=7;
    if(imr<5){
      smr="-1";
    }
    else{
      smr= String(distancetoGrid(imr));
    }
   }

   
   int iml = getdistance(SharpIR3);
   if(iml==-1){
    sml= String(iml);
   }
   else{
    iml-=7;
    if(iml<5){
      sml="-1";
    }
    else{
      sml= String(distancetoGrid(iml));
    }
   }

   
   int ill = getLdistance(SharpIR4);
   if(ill==-1){
    sll=String(ill);
   }
   else{
      ill-=6;
      sll= String(distancetoGrid(ill));
   }

   
   Serial.print("pscan ");
   Serial.print(sfr);//front right
   Serial.print(" ");
   Serial.print(sfm);//front mid
   Serial.print(" ");
   Serial.print(sfl);//front left
   Serial.print(" ");
   Serial.print(smr);//mid right
   Serial.print(" ");
   Serial.print(sml);//mid left
   Serial.print(" ");
   Serial.println(sll);//long left 
  
}*/
