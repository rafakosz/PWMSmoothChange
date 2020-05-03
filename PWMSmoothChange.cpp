#include "LedSmoothChange.h"

  // Constructor

LedSmoothChange::LedSmoothChange (int attachTo):
  LedPin(attachTo){
}

// SETUP

void LedSmoothChange::setup(){
  pinMode(LedPin, OUTPUT);
  icr = icrMax - icrMin;
  dVMax = sqrt(icr); // max value for brightness
  dVStep = dVMax / iBrSteps;
  dSqrtIcr = sqrt(icr);
  //hide Serial.println("Led initialized");
  //hide Serial.println(dVStep);
}

// LOOP

void LedSmoothChange::loop(){
  if (LightChangeFlag >= 0) {
    ////hide Serial.print('.');
    bChangeFinished = lightChange(LightChangeFlag);
    if (bChangeFinished) {
      LightChangeFlag = STOP;
    }
  }
}

// METHODS

bool LedSmoothChange::changeFinished(){
  bool bchF = 0;
  if (bChangeFinished){
    bchF = 1;
    bChangeFinished = 0;
  }
  return bchF;
}

void LedSmoothChange::setLightValue(int ilVal){
  //hide Serial.println("Setting Light Value");
  calcNewPWM(ilVal);
  LightChangeFlag = DOWN;
  //hide Serial.print("iLastPWM =");
  //hide Serial.println(iLastPWM);
  //hide Serial.print("iFinalPWM =");
  //hide Serial.println(iFinalPWM);
  if (iLastPWM < iFinalPWM) LightChangeFlag = UP;
  //hide Serial.print("iVal = ");
  //hide Serial.println(ilVal);
  //hide Serial.println(LightChangeFlag);
}

void LedSmoothChange::calcNewPWM(int iNewPV){
  // int iFinalPWM = 0;
  // double dNewPV = (iNewPV * 0.73) + 27;
  double   dNewV = iNewPV * dVStep;
  bool bEnd = 0;
  //hide Serial.print("dNewV: ");
  //hide Serial.println(dNewV);

  if (iNewPWM <= icrMin) {
    iNewPWM = icrMin;
    iLastPWM = icrMin;
  }

  iFinalPWM = (dNewV * dNewV) + icrMin;


  //hide Serial.print("Calculated PWM: ");
  //hide Serial.println(iFinalPWM);

  // return iFinalPWM;
}

int LedSmoothChange::calcCurPVal () {
  if (iLastPWM > icrMin) {
    int iFinalPWM = iLastPWM - icrMin;
    double dCurV = sqrt(iFinalPWM);
    iCurPV = dCurV/dVStep;
  } else iCurPV = 0;
  return iCurPV;
  //hide Serial.println(iCurPV);
}

void LedSmoothChange::stopChange () {
  ulStartTme = 0;
  iChngDelayTme = 0;
  iFinalPWM = iLastPWM;
  LightChangeFlag = STOP;

}

bool LedSmoothChange::lightChange (bool bDirection) {
  ////hide Serial.println("...");
  bool bIsFinished = 0;
  if (ulStartTme > 0) {
    //    iCurPV//hide Serial.print(".");
    if (bDirection) {
      if (iNewPWM < iFinalPWM && iNewPWM < icrMax) {
        if (millis() - ulStartTme > iChngDelayTme) {
          analogWrite(LedPin, iNewPWM);
          iNewPWM++;
          iChngDelayTme = (sqrt(iNewPWM) - sqrt(iLastPWM))*iTm/dSqrtIcr;
        }
      } else bIsFinished = 1;
    } else if (iNewPWM > iFinalPWM && iNewPWM > icrMin) {
      if (millis() - ulStartTme > (iChngDelayTme)) {
        iNewPWM--;
        analogWrite(LedPin, iNewPWM);
        iChngDelayTme = (sqrt(iLastPWM) - sqrt(iNewPWM))*iTm/dSqrtIcr;
      }
    } else bIsFinished = 1;
  } else {
    iNewPWM = iLastPWM;
    ulStartTme = millis();
    ////hide Serial.println(icr);
    //hide Serial.print("iFinalPWM: ");
    //hide Serial.println(iFinalPWM);
    //hide Serial.print("iNewPWM: ");
    //hide Serial.println(iNewPWM);
    //hide Serial.print("start of change: ");
    //hide Serial.println(millis());
  }
  if (bIsFinished) {
    ulStartTme = 0;
    iChngDelayTme = 0;
    if (iNewPWM <= icrMin) {
      iNewPWM = 0;
      analogWrite(LedPin, iNewPWM);
      //hide Serial.print("zero");
    }
    iLastPWM = iNewPWM;
    //hide Serial.print("end of change: ");
    //hide Serial.println(millis());

  }
  return bIsFinished;
}
