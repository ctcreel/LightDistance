#include "DEBUG.h"
#include "../../Projects/HydroManager/Commands.h"
#include <SoftwareSerial.h>
#include <TextFinder.h>
#include <Time.h>
#include <TimeAlarms.h>
#include "eventStream.h"
#include "eventHandler.h"
#include "eventCallBack.h"
#include "generatorDeviceID.h"

#define RANGE_FINDER_DATA 53
#define RANGE_FINDER_POWER 52
#define ACCEPTABLE_HEIGHT 18
#define MEASURE_EVERY 

generatorDeviceID gID;

eventStream *e;
int lastNotified;

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(19200);
  pinMode(RANGE_FINDER_DATA, INPUT);
  pinMode(RANGE_FINDER_POWER, OUTPUT);
  digitalWrite(RANGE_FINDER_POWER, HIGH);
  lastNotified = 0;
  Serial3.begin(BAUD_RATE);
  e = new eventStream(&Serial3,&gID);
  new eventOutgoing(e, getDistance, SET_DISTANCE, GET_DISTANCE);
  Alarm.timerRepeat(900, measurement);
  measurement();
}

void loop() {
  e->check(60);
  unsigned long distance = getDistance();
  if(lastNotified > 0) {
    lastNotified--;
  } else if(distance < ACCEPTABLE_HEIGHT) {
    e->createEvent(getDistance(), SET_DISTANCE_ALARM);
    lastNotified = 10;
  }
}

const unsigned long getDistance(void) {
  const float deviceHeight = 3;
  float pulse = (((float) pulseIn(RANGE_FINDER_DATA, HIGH)) / 147.0 + deviceHeight);
  DEBUG("Plant is "+String(pulse) + " inches away from the light");
  return ((unsigned long) pulse);
}

void measurement(void) {
    e->createEvent(getDistance(), SET_DISTANCE);
}

