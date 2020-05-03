#include <PWMSmoothChange.h>

#define ledPIN 5
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

//                                          Receiving serial
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

PWMSmoothChange pwm(255);

void setup(){
  Serial.begin(9600);
  inputString.reserve(100);
  while (!Serial);
  pinMode(ledPIN, OUTPUT);
  pwm.setup(10240);
}

void loop(){
  if (stringComplete) {
    Serial.print("Final PWM: ");
    int newValue = inputString.toInt();
    if (newValue >= 0) {
      pwm.setLightValue(newValue);
      Serial.println(pwm.iFinalPWM);
    }
    stringComplete = false;
    inputString = "";
  }
  if (pwm.check()) {
    analogWrite(ledPIN, pwm.iNewPWM);
  }
}
