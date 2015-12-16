#define TRIG_OR_ECHO 4
long distances[5];
byte angles[5] = {130, 115, 70, 85, 100};
typedef enum {GO_VERY_LEFT, GO_LEFT, GO_STRAIGHT, GO_RIGHT, GO_VERY_RIGHT, QUARTER_LEFT, QUARTER_RIGHT, HALF_TURN} way_t;
way_t ultraSonic_lastWay;

int ultraSonic_getDistance(long timeoutMicro = 100000){
  long startTime = 0;
  long endTime = 0;
  int distance = 0;
  boolean found = false;
  DDRD |= 1<<TRIG_OR_ECHO;
  PORTD |= 1<<TRIG_OR_ECHO;
  delayMicroseconds(10); 
  PORTD &= ~(1<<TRIG_OR_ECHO);
  DDRD &= ~(1<<TRIG_OR_ECHO);
  
  while(1){
    if((PIND & (1<<TRIG_OR_ECHO))){
        if(startTime == 0){
            startTime = micros();
        }
       endTime = micros();
       if((startTime + timeoutMicro) < endTime){
         break;
       }
    }
    else{
      if(startTime != 0){
        found = true;
        break;
      }
    }
  }
  
  if(found){
     distance = ((endTime-startTime)/ 58);
  }
  #ifdef DEBUG
    Serial.print("Distance: ");
    Serial.println(distance);
  #endif
  return distance;
}

void ultraSonic_checkStraightDistance(){
  distances[4] = ultraSonic_getDistance();
}

void ultraSonic_checkAllDistances(){
  byte angleNumbers = sizeof(angles);
  for(byte checkIndex = 0; checkIndex < angleNumbers; checkIndex++){
    servo_rotate(angles[checkIndex]);
    distances[checkIndex] = ultraSonic_getDistance();
  }
}

byte ultraSonic_checkObstruction(){
  byte obstruction = 0;
  if(distances[0] <= 46 && distances[0] != 0){
    obstruction |= 1;
  }
  
  if(distances[2] <= 46 && distances[2] != 0){
    obstruction |= 1<<4;
  }
  
  if(distances[1] <= 42 && distances[1] != 0){
    obstruction |= 1<<1;
  }
  
  if(distances[3] <= 42 && distances[3] != 0){
    obstruction |= 1<<3;
  }
 
  if(distances[4] <= 40 && distances[4] != 0){
    obstruction |= 1<<2;
  }
  
  #ifdef DEBUG
    Serial.print("Obstruction: ");
    Serial.println(obstruction, BIN);
  #endif
  return obstruction;
}

boolean ultraSonic_checkStraight(byte obstruction){
  return (!(obstruction & 1<<2) && !(obstruction & 1<<1) && !(obstruction & 1<<3));
}

way_t ultraSonic_getWay(){
    byte obstruction = ultraSonic_checkObstruction();
    
    if(ultraSonic_checkStraight(obstruction)){
      #ifdef DEBUG
        Serial.println("GO_STRAIGHT");
      #endif
      ultraSonic_lastWay = GO_STRAIGHT;
      return GO_STRAIGHT;
    }
    
    if(!(obstruction & 1<<2)){      //NO OBSTRUCTION AT THE MIDDLE       
      
      if(!(obstruction & 1<<1) && ultraSonic_lastWay != GO_LEFT){    //NO OBSTRUCTION AT RIGHT
        #ifdef DEBUG
          Serial.println("GO_RIGHT");
        #endif
        ultraSonic_lastWay = GO_RIGHT;
        return GO_RIGHT;
      }
       
      if(!(obstruction & 1<<3) && ultraSonic_lastWay != GO_RIGHT){    //NO OBSTRUCTION AT LEFT
        #ifdef DEBUG
          Serial.println("GO_LEFT");
        #endif
        ultraSonic_lastWay = GO_LEFT;
        return GO_LEFT;
      }
    }
    else{
      if(!(obstruction & 1<<1) && !(obstruction & 1<<0) && ultraSonic_lastWay != GO_VERY_LEFT){    //NO OBSTRUCTION AT RIGHT AND VERY RIGHT
        #ifdef DEBUG
          Serial.println("GO_VERY_RIGHT");
        #endif
        ultraSonic_lastWay = GO_VERY_RIGHT;
        return GO_VERY_RIGHT;
      }
      
      if(!(obstruction & 1<<3) && !(obstruction & 1<<4) && ultraSonic_lastWay != GO_VERY_RIGHT){    //NO OBSTRUCTION AT LEFT AND VERY LEFT
        #ifdef DEBUG
          Serial.println("GO_VERY_LEFT");
        #endif
        ultraSonic_lastWay = GO_VERY_LEFT;
        return GO_VERY_LEFT;
      }
      
      if(!(obstruction & 1<<0) && ultraSonic_lastWay != QUARTER_LEFT){  //NO OBSTRUCTION AT VERY RIGHT
        #ifdef DEBUG
          Serial.println("QUARTER_RIGHT");
        #endif
        ultraSonic_lastWay = QUARTER_RIGHT;
        return QUARTER_RIGHT;
      }
      
      if(!(obstruction & 1<<4) && ultraSonic_lastWay != QUARTER_RIGHT){ //NO OBSTRUCTION AT VERY LEFT
        #ifdef DEBUG
          Serial.println("QUARTER_LEFT");
        #endif
        ultraSonic_lastWay = QUARTER_LEFT;
        return QUARTER_LEFT;
      }
    }
    
    #ifdef DEBUG
      Serial.println("HALF_TURN");
    #endif
    ultraSonic_lastWay = HALF_TURN;
    return HALF_TURN;
}


