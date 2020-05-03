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
// initializing PWMSmoothChange object as pwm with a max PWM value of 255
PWMSmoothChange pwm(255);


void setup(){
  Serial.begin(9600);
  inputString.reserve(100);
  while (!Serial);
  pinMode(ledPIN, OUTPUT);
  // setup transition time for about 10 s (from 0 to 100%)
  pwm.setup(10240);
}

void loop(){
  // wait for string with a new value from 0 to 100
  if (stringComplete) {
    Serial.print("Final PWM: ");
    int newValue = inputString.toInt();
    if (newValue >= 0) {
      // passing the new value to pwm object
      pwm.setLightValue(newValue);
      Serial.println(pwm.iFinalPWM);
    }
    stringComplete = false;
    inputString = "";
  }
  // in every loop checks if new PWM value should be change and if yes 
  //  writes to the specified pin with analogWrite()
  if (pwm.check()) {
    analogWrite(ledPIN, pwm.iNewPWM);
  }
}
