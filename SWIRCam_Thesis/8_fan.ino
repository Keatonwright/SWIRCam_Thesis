//----------------------------------------------------
//----------------------FAN SETUP---------------------
//----------------------------------------------------

// Set fan to lowest mode (off)
void setFanRed() {
  io.analogWrite(pwmPin, 255);
  io.analogWrite(greenLedPin, 0);
  io.analogWrite(yellowLedPin, 0);
  io.analogWrite(redLedPin, 255);
  Serial.println("Fan set to RED");
  fanCurrentState = 0;
  fanNextState = -1;
}

// Set fan to middle mode (50% PWM)
void setFanYellow() {
  io.analogWrite(greenLedPin, 0);
  io.analogWrite(yellowLedPin, 255);
  io.analogWrite(redLedPin, 0);
  Serial.println("Fan set to YELLOW");
  // Set to max briefly to avoid motor stalling
  if (fanCurrentState == 0){ 
    io.analogWrite(pwmPin, 0);
    delay(750);
  }
  io.analogWrite(pwmPin, 128);
  fanCurrentState = 1;
  fanNextState = -1;
}

// Set fan to highest mode (100% PWM)
void setFanGreen() {
  io.analogWrite(pwmPin, 0);
  io.analogWrite(greenLedPin, 255);
  io.analogWrite(yellowLedPin, 0);
  io.analogWrite(redLedPin, 0);
  Serial.println("Fan set to GREEN");
  fanCurrentState = 2;
  fanNextState = -1;
}

// Method to check for fan button inputs using debounce
// Fan states are 0, 1, and 2 (red, yellow, green, respectively)
void checkFanButtonInputs() {
  int upValue = io.digitalRead(fanUpPin);
  int downValue = io.digitalRead(fanDownPin);

  if (upValue != fanUpLastStableState) {
    if (millis() - fanUpLastDebounceTime > fanDebounceDelay) {
      fanUpLastStableState = upValue;
      if (upValue == HIGH) {
        Serial.println("Fan UP button pressed");
        fanNextState = min(fanCurrentState+1,2);
      }
    }
    fanUpLastDebounceTime = millis();
  }

  if (downValue != fanDownLastStableState) {
    if (millis() - fanDownLastDebounceTime > fanDebounceDelay) {
      fanDownLastStableState = downValue;
      if (downValue == HIGH) {
        Serial.println("Fan DOWN button pressed");
        fanNextState = max(fanCurrentState-1,0);
      }
    }
    fanDownLastDebounceTime = millis();
  }
}

// Case statement to set fan
void updateFan() {
  switch (fanNextState) {
    case 2:
      setFanGreen();
      break;
    case 1:
      setFanYellow();
      break;
    case 0:
      setFanRed();
      break;
    default:
      break;
  }
}

// Loop function to go in main loop
void fanLoop() {
  checkFanButtonInputs();
  if (fanNextState != -1){
    updateFan();
  }
}
