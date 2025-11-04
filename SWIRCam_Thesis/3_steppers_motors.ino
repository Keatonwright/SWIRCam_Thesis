// ---------------------------------------------------
// ---------------STEPPER MOTORS----------------------
// ---------------------------------------------------

// Simple single X and Y step
void moveStepX() {
  digitalWrite(STEP_X, HIGH);
  delayMicroseconds(stepSignalDelay);
  digitalWrite(STEP_X, LOW);
  delayMicroseconds(stepSignalDelay);
}

void moveStepY() {
  delayMicroseconds(stepSignalDelay);
  digitalWrite(STEP_Y, HIGH);
  delayMicroseconds(stepSignalDelay);
  digitalWrite(STEP_Y, LOW);
}

// Taring and centering
void centre() {
  digitalWrite(ENABLE_XY, LOW);
  delay(100);
  moveTareX();
  moveCentreX();
  moveTareY();
  moveCentreY();
  digitalWrite(ENABLE_XY, HIGH);
}

// Step motor x until micro-switch closes
void moveTareX() {
  Serial.println("Start taring process X.");
  digitalWrite(DIR_X, HIGH);
  delay(100);

  while(digitalRead(SWITCH_XY)==LOW){
    digitalWrite(STEP_X, HIGH);
    delay(1);
    digitalWrite(STEP_X, LOW);
    delay(1);
  }
  Serial.println("Contact made! Tared X.");
}

// Step motor y until micro-switch closes
void moveTareY() {
  Serial.println("Start taring process Y.");
  digitalWrite(DIR_Y, LOW);
  delay(100);

  while(digitalRead(SWITCH_XY)==LOW){
    digitalWrite(STEP_Y, HIGH);
    delay(1);
    digitalWrite(STEP_Y, LOW);
    delay(1);
  }
  Serial.println("Contact made! Tared Y.");
}

// Step motor x after taring until centred
void moveCentreX() {
  Serial.println("Start centering process.");
  digitalWrite(DIR_X, LOW);
  int steps_X = 0;
  int target_X = stepsUntilContactX*stepsMultiplierX-120;
  delay(100);

  while(steps_X < target_X){
    digitalWrite(STEP_X, HIGH);
    delay(1);
    digitalWrite(STEP_X, LOW);
    delay(1);
    steps_X+=1;
  }
  Serial.println("Centered!");
}

// Step motor y after taring until centred
void moveCentreY() {
  Serial.println("Start centering process.");
  digitalWrite(DIR_Y, HIGH);
  int steps_Y = 0;
  int target_Y = stepsUntilContactY*stepsMultiplierY-120;
  delay(100);

  while(steps_Y < target_Y){
    digitalWrite(STEP_Y, HIGH);
    delay(1);
    digitalWrite(STEP_Y, LOW);
    delay(1);
    steps_Y+=1;
  }
  Serial.println("Centered!");
}

