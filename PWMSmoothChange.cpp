#include "PWMSmoothChange.h"

  // Constructor

PWMSmoothChange::PWMSmoothChange (int MaxPWM){
  pwmMax = MaxPWM;
}

// SETUP

void PWMSmoothChange::setup(int transTime){
  transTime = transTime;
  pwm = pwmMax - pwmMin;
  dVMax = sqrt(pwm); // max value for brightness
  dVStep = dVMax / iBrSteps;
  dSqrtpwm = sqrt(pwm);
  Serial.println("Led initialized");
  Serial.println(dVStep);
}


// METHODS

bool PWMSmoothChange::check(){
  bool bNewPWM = false;
  if (LightChangeFlag >= 0) {
    bChangeFinished = lightChange(LightChangeFlag);
    if (!bChangeFinished) {
        if (iChngDelayTme>0) {
          bNewPWM = true;
        }
    } else {
      LightChangeFlag = STOP;
    }
  }
  return bNewPWM;
}

bool PWMSmoothChange::changeFinished(){
  bool bchF = 0;
  if (bChangeFinished){
    bchF = 1;
    bChangeFinished = 0;
  }
  return bchF;
}

void PWMSmoothChange::setLightValue(int ilVal){
  calcNewPWM(ilVal);
  LightChangeFlag = DOWN;
  if (iLastPWM < iFinalPWM) LightChangeFlag = UP;
  Serial.print("iVal = ");
  Serial.println(ilVal);
}

void PWMSmoothChange::calcNewPWM(int iNewPV){
  double   dNewV = iNewPV * dVStep;
  bool bEnd = 0;

  if (iNewPWM <= pwmMin) {
    iNewPWM = pwmMin;
    iLastPWM = pwmMin;
  }
  iFinalPWM = (dNewV * dNewV) + pwmMin;
}

int PWMSmoothChange::calcCurPVal () {
  if (iLastPWM > pwmMin) {
    int iFinalPWM = iLastPWM - pwmMin;
    double dCurV = sqrt(iFinalPWM);
    iCurPV = dCurV/dVStep;
  } else iCurPV = 0;
  return iCurPV;
}

void PWMSmoothChange::stopChange () {
  ulStartTme = 0;
  iChngDelayTme = 0;
  iFinalPWM = iLastPWM;
  LightChangeFlag = STOP;

}

bool PWMSmoothChange::lightChange (bool bDirection) {
  bool bIsFinished = 0;
  if (ulStartTme > 0) {
    if (bDirection) {
      if (iNewPWM < iFinalPWM && iNewPWM < pwmMax) {
        if (millis() - ulStartTme > iChngDelayTme) {
          iNewPWM++;
          iChngDelayTme = (sqrt(iNewPWM) - sqrt(iLastPWM))*transTime/dSqrtpwm;
        }
      } else bIsFinished = 1;
    } else if (iNewPWM > iFinalPWM && iNewPWM > pwmMin) {
      if (millis() - ulStartTme > (iChngDelayTme)) {
        iNewPWM--;
        iChngDelayTme = (sqrt(iLastPWM) - sqrt(iNewPWM))*transTime/dSqrtpwm;
      }
    } else bIsFinished = 1;
  } else {
    iNewPWM = iLastPWM;
    ulStartTme = millis();
    Serial.print("start of change: ");
    Serial.println(millis());
  }
  if (bIsFinished) {
    ulStartTme = 0;
    iChngDelayTme = 0;
    if (iNewPWM <= pwmMin) {
      iNewPWM = 0;
    }
    iLastPWM = iNewPWM;
    Serial.print("end of change: ");
    Serial.println(millis());

  }
  return bIsFinished;
}
