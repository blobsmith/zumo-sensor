
// Info led define
#define LED13 13
  
// these might need to be tuned for different motor types
#define DETECT_SPEED      50
#define NORMAL_SPEED      200
#define TURN_SPEED        400

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

typedef enum {STOP, RUN, DETECT} state_t;
state_t zumo_state = STOP;
int zumo_loopCount = 0;

//boolean zumo_debug(){
//  boolean debugMode = false;
//  #ifdef DEBUG
//    debugMode = true;
//  #endif
//  return debugMode;
//}

void zumo_changeSpeed(int right, int left){
  #ifdef DEBUG
    Serial.print("SPEED: R->");
    Serial.print(right);
    Serial.print(" L->");
    Serial.println(left);
  #else
    motors.setSpeeds(right, left);
  #endif
}

void zumo_waitForButton()
{
  digitalWrite(LED13, HIGH);
  button.waitForButton();
  digitalWrite(LED13, LOW);
}

void zumo_changeWay(){
  switch(ultraSonic_getWay()){
    case GO_VERY_LEFT:
    zumo_changeSpeed(TURN_SPEED, (-TURN_SPEED));
    delay(75);
    break;
    
    case GO_LEFT:
    zumo_changeSpeed(TURN_SPEED, (-TURN_SPEED));
    delay(40);
    break;
    
    case GO_STRAIGHT:
    break;
    
    case GO_RIGHT:
    zumo_changeSpeed((-TURN_SPEED), TURN_SPEED);
    delay(40);
    break;
    
    case GO_VERY_RIGHT:
    zumo_changeSpeed((-TURN_SPEED), TURN_SPEED);
    delay(75);
    break;
    
    case QUARTER_LEFT:
    zumo_changeSpeed(TURN_SPEED, (-TURN_SPEED));
    delay(150);
    break;
    
    case QUARTER_RIGHT:
    zumo_changeSpeed((-TURN_SPEED), TURN_SPEED);
    delay(150);
    break;
    
    case HALF_TURN:
    zumo_changeSpeed(TURN_SPEED, (-TURN_SPEED));
    delay(300);
    break;
  }
  zumo_changeSpeed(NORMAL_SPEED, NORMAL_SPEED);
}

void zumo_checkState(){
  #ifdef DEBUG
    Serial.print("State: ");
  #endif
  switch(zumo_state){
    case STOP:
      #ifdef DEBUG
        Serial.println("STOP");
      #endif
      zumo_changeSpeed(NORMAL_SPEED, NORMAL_SPEED);
      zumo_state = RUN;
    break;
    
    case RUN:
    #ifdef DEBUG
        Serial.println("RUN");
    #endif
    if(zumo_loopCount < 200){
      zumo_changeSpeed(NORMAL_SPEED, NORMAL_SPEED);
      ultraSonic_checkStraightDistance(); 
    }
    else{
      zumo_loopCount = 0;
      zumo_changeSpeed(NORMAL_SPEED, NORMAL_SPEED);
      ultraSonic_checkAllDistances();
    }
    zumo_loopCount++;
    if(ultraSonic_getWay() != GO_STRAIGHT){
      zumo_state = DETECT;
    }
    break;
    
    case DETECT:
    #ifdef DEBUG
        Serial.println("DETECT");
    #endif
    zumo_changeSpeed(0, 0);
    delay(300);
    ultraSonic_checkAllDistances();
    zumo_changeWay();
    zumo_state = RUN;
    break;
    
    default:
    #ifdef DEBUG
        Serial.println("NO STATE");
    #endif
    break;
  }
}
