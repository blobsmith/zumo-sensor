#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

//#define DEBUG

#include "servoDriver.h"
#include "ultrasonicDriver.h"
#include "zumoSoft.h"

void setup(){ 
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  servo_setup();
  zumo_waitForButton();
  #ifdef DEBUG
    Serial.println("Starting...");
  #endif
  ultraSonic_checkAllDistances();
}

void loop()
{
    zumo_checkState();
     #ifdef DEBUG
      Serial.println("**************** NEXT STEP **************");
    #endif
}
