
int getdistance(SharpIR x){
  int d;
  d=x.distance();
  if(d<7||d>34){
    d=-1;
  }
  return d;
}

int getLdistance(SharpIR x){
  int d;
  d=x.distance();
  //if(d<=35||d>44){
    //d=-1;
  //}
  if(d>54||d<34){
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

int modeValue(SharpIR x, int s){
  int list[10];
  int i=0;
  for(i=0;i<10;i++){
    int d= getdistance(x);
    if(d==-1){
      list[i]=-1;
    } 
    else{
      if(s==0){
        d+=0;//2
      }
      if(s==1){
        d+=0;//3
      }
      if(s==2){
        d+=0;//3
      }
      if(s==3){
        d-=1;
      }
      if(s==4){
        d-=3;//2
      }
      list[i]=distancetoGrid(d);
    }
  }
  int list_length = sizeof(list) / sizeof(list[0]);
  qsort(list, list_length, sizeof(list[0]), sort_desc);

  int modeCt = 0;
  int modeV = -1;
  int ct = 0;
  int v = -1;
  for(int i = 0; i< 10; i++) {
    if(list[i] != v) {
      v = list[i];
      ct = 0;
    }
    ct ++;
    if(ct > modeCt) {
      modeCt = ct;
      modeV = v;
    }
  }
  return modeV;
}

int modeCalibrate(SharpIR x){
  int list[20];
  int i=0;
  for(i=0;i<20;i++){
    int d= getdistance(x);
    if(d==-1){
      list[i]=-1;
    } else{
      list[i]=d;
    }
  }
  int list_length = sizeof(list) / sizeof(list[0]);
  qsort(list, list_length, sizeof(list[0]), sort_desc);

  int modeCt = 0;
  int modeV = -1;
  int ct = 0;
  int v = -1;
  for(int i = 0; i< 20; i++) {
    if(list[i] != v) {
      v = list[i];
      ct = 0;
    }
    ct ++;
    if(ct > modeCt) {
      modeCt = ct;
      modeV = v;
    }
  }
  return modeV;
}

int modeLong(SharpIR x){
  int list[10];
  int i=0;
  for(i=0;i<10;i++){
    int d= getLdistance(x);
    
    if(d==-1){
      list[i]=-1;
    } else{
      d+=0;
      list[i]=distancetoGrid(d);
    }
  }
  int list_length = sizeof(list) / sizeof(list[0]);
  qsort(list, list_length, sizeof(list[0]), sort_desc);

  int modeCt = 0;
  int modeV = -1;
  int ct = 0;
  int v = -1;
  for(int i = 0; i< 10; i++) {
    if(list[i] != v) {
      v = list[i];
      ct = 0;
    }
    ct ++;
    if(ct > modeCt) {
      modeCt = ct;
      modeV = v;
    }
  }
  return modeV;
}
void printSensor(int ifr, int ifm, int ifl, int imr, int iml, int ill){
  String sfr, sfm, sfl, smr,sml,sll;
    sfr= String(ifr);
    sfm= String(ifm);
    sfl= String(ifl);
    smr= String(imr);
    sml= String(iml);
    sll=String(ill);

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
   Serial.print(sll);//long left 
   Serial.println("|");
}
