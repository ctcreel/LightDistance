#include "DEBUG.h"
#include "../../Projects/HydroManager/Commands.h"
#include "../../Projects/HydroManager/Config.h"
#include <TextFinder.h>
#include <Time.h>
#include <TimeAlarms.h>
#include "eventStream.h"
#include "eventHandler.h"
#include "eventCallBack.h"
#include "generatorDeviceID.h"

#define RANGE_FINDER_DATA 53
#define RANGE_FINDER_POWER 52

generatorDeviceID gID;

eventStream *e;

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(19200);
  pinMode(RANGE_FINDER_DATA, INPUT);
  pinMode(RANGE_FINDER_POWER, OUTPUT);
  digitalWrite(RANGE_FINDER_POWER, HIGH);
  Serial3.begin(BAUD_RATE);
  e = new eventStream(&Serial3,&gID);
  new eventOutgoing(e, getDistance, SET_DISTANCE, GET_DISTANCE);
}

void loop() {
  e->check(10);
/*
  Serial.print("Distance ");
  Serial.println(getDistance());
*/
}

const unsigned long getDistance(void) {
  float pulse = (((float) pulseIn(RANGE_FINDER_DATA, HIGH)) / 147.0 + SENSOR_TO_PLANT_HEIGHT);
  DEBUG("Plant is "+String(pulse) + " inches away from the light");
  return ((unsigned long) pulse);
}
