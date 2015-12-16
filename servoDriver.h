#define SERVO_SIGNAL_PURPULE 6 // purpule 5(PORTC 6)

/**
* servoRotate
* angleDegree: 0 to 180 degree
**/
void servo_rotate(byte angleDegree){
  static boolean firstInit = false; 
  static byte lastAngle;
  byte loopNumber = 0;
  float microSecondToDegre = 10.5555;
  byte index = 0;
  
  if(!firstInit){
    loopNumber = 12;
    firstInit = true;
    lastAngle = 0;
  }
  else{
    if(angleDegree > lastAngle){
      loopNumber = angleDegree - lastAngle;
    }
    else{
      loopNumber = lastAngle - angleDegree;
    }
    loopNumber = loopNumber / 15;
  }
  
  while(index <= loopNumber){
    PORTC |= (1<<SERVO_SIGNAL_PURPULE);
    delayMicroseconds((int)(500 + angleDegree*microSecondToDegre));  // 500 -> 2400 microseconds
    PORTC &= ~(1<<SERVO_SIGNAL_PURPULE);
    
    delay(25);
    index++;
  }
  lastAngle = angleDegree;
}

void servo_setup(){
  DDRC |= 1<<SERVO_SIGNAL_PURPULE;
  servo_rotate(100);
}
