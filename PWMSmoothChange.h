#ifndef LedSmoothChange_h
#define LedSmoothChange_h

#include <Arduino.h>

class LedSmoothChange {
private:
  enum lcFlag {
    STOP = -1,
    DOWN = 0,
    UP = 1
  };

  uint16_t icr = icrMax - icrMin ; // resolution = PWM resolution
  double dSqrtIcr = 0;
  unsigned long iChngDelayTme = 0; // var for calculatd dealy time

  //double dVCur = 0;     // variables for even brightness changing
  double dVMax = 0; // max value for brightness
  double dVStep = 0;
  unsigned long ulStartTme = 0; // timer for start of change
  bool bChangeFinished = 0;

//   Private Functions:
  void calcNewPWM(int iNewPV);
  bool lightChange (bool bDirection);
public:
  int calcCurPVal ();
  LedSmoothChange (int attachTo);
  int LedPin; // D4crMi
  void setLightValue(int ilVal);
  void stopChange ();
  void setup();
  void loop();
  bool changeFinished();
  int iTm;      // time from 0 to 100 % lightness
  // int currentLightLevel = 0;
  lcFlag LightChangeFlag = STOP;
  int iCurPV = 0;  // current percent light value
  uint16_t iNewPWM = 0;    // counter for current PWM Value
  uint16_t iFinalPWM = 0;          // new PWM Value
  uint16_t iLastPWM = 0;      // curent PWM Value
  uint16_t icrMax = 1024;
  uint16_t icrMin = 0;
  int iBrSteps = 100;           // no of brightness steps
};

#endif
