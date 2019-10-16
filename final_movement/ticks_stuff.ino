void m1_Interrupt(){
  m1_tick++;
}

void m2_Interrupt(){
  m2_tick++;
}

void rotate_counter(int degree){
  //moveStop();
  //delay(110);
  int total_count = 0;
  total_count = 8.851*degree-41.4-9;
  m1_oldtick = m1_tick;
  md.setM1Speed(-200);
  md.setM2Speed(-200);
  while((m1_tick - m1_oldtick)<total_count){
    Serial.println(m1_tick - m1_oldtick);
  }
  moveStop();
  delay(50);
  return;
}

void rotate_clock(int degree){
  //moveStop();
  //delay(110);
  int total_count = 0;
  total_count = 8.851*degree-41.4+10;
  m1_oldtick = m1_tick;
  md.setM1Speed(200);
  md.setM2Speed(200);
  while((m1_tick - m1_oldtick)<total_count){
    Serial.println(m1_tick - m1_oldtick);
  }
  moveStop();
  delay(50);
  return;
}
