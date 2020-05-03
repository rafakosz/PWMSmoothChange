#ifndef PWMSmoothChange_h
#define PWMSmoothChange_h

#include <Arduino.h>

class PWMSmoothChange {
private:
  enum lcFlag {
    STOP = -1,
    DOWN = 0,
    UP = 1
  };

  uint16_t pwm = pwmMax - pwmMin ; // PWM resolution
  double dSqrtpwm = 0;
  unsigned long iChngDelayTme = 0; // var for calculatd delay time

  //double dVCur = 0;     // variables for even brightness changing
  double dVMax = 0; // max value for brightness
  double dVStep = 0;
  unsigned long ulStartTme = 0; // timer for start of change
  bool bChangeFinished = 0;
  int iBrSteps = 100;           // no of brightness steps

//   Private Functions:
  void calcNewPWM(int iNewPV);
  bool lightChange (bool bDirection);
public:
  int calcCurPVal ();
  PWMSmoothChange (int transTime);
  void setLightValue(int ilVal);
  void stopChange ();
  void setup(int transTime);
  bool check();
  bool changeFinished();
  int transTime;      // time from 0 to 100 % lightness
  lcFlag LightChangeFlag = STOP;
  int iCurPV = 0;  // current percent light value
  uint16_t iNewPWM = 0;    // counter for current PWM Value
  uint16_t iFinalPWM = 0;          // final PWM Value
  uint16_t iLastPWM = 0;      // last PWM Value
  uint16_t pwmMax = 1024;
  uint16_t pwmMin = 0;
};

#endif
