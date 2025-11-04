//----------------------------------------------------
//----------------------DISPLAY-----------------------
//----------------------------------------------------

// Universal display photo method (case statement for zoom)
void displayPhoto(){
  int pixel = 0;
  int scaleVal = 255/maxVal;
  Serial.print("Displaying "); Serial.print(zoom); Serial.println("x Zoom Image");  // 120x120 result

  switch (zoom) {
    case 1:
      display1xPhoto();
      break; 
    case 2:
      display2xPhoto();
      break;
    default:
      Serial.println("Invalid zoom");
      break; 
  }
}

// Populates LCD from downsampled 120x120 image and displays it full.
void display1xPhoto() {
  Serial.println("Displaying Photo 1x (fast stream)");

  const int cropSize = 120;
  const int displayX = 4;
  const int displayY = 4;

  float scaleVal = stretch ? (255.0 / (maxVal + 0.001)) : 1.0;

  // Set up drawing window once
  tft.startPushData(displayX, displayY, displayX + cropSize - 1, displayY + cropSize - 1);

  for (int i = 0; i < cropSize; i++) {
    for (int j = 0; j < cropSize; j++) {
      int val = image_downsampled[i][j] * scaleVal;
      val = constrain(val, 0, 255);
      uint16_t color = tft.Color565(val, val, val);
      tft.pushData(color);
    }
  }

  tft.endPushData();  // done sending pixels
}


// Displays photo using x/y offsets (no resampling)
void display2xPhoto() {
  Serial.println("Displaying Photo 2x (fast stream)");

  const int cropSize = 120;
  const int displayX = 4;
  const int displayY = 4;

  float scaleVal = stretch ? (255.0 / (maxVal + 0.001)) : 1.0;

  tft.startPushData(displayX, displayY, displayX + cropSize - 1, displayY + cropSize - 1);

  for (int i = 0; i < cropSize; i++) {
    for (int j = 0; j < cropSize; j++) {
      int val = image[i + y_start][j + x_start] * scaleVal;
      val = constrain(val, 0, 255);
      uint16_t color = tft.Color565(val, val, val);
      tft.pushData(color);
    }
  }

  tft.endPushData();
}

void displayBitmapToRegion(const uint8_t *bmp) {
    const int cropSize = 119;
    const int displayX = 4;
    const int displayY = 4;

    float scaleVal = stretch ? (255.0 / (maxVal + 0.001)) : 1.0;

    tft.startPushData(displayX, displayY, displayX + cropSize - 1, displayY + cropSize - 1);

    for (int row = 0; row < cropSize; row++) {
        for (int col = 0; col < cropSize; col++) {
            // Compute index for row-major bitmap stored as 1D array in PROGMEM
            int idx = row * cropSize + col;

            // Read pixel value from PROGMEM
            int val = pgm_read_byte(&bmp[idx]) * scaleVal;
            val = constrain(val, 0, 255);

            // Convert to 16-bit color
            uint16_t color = tft.Color565(val, val, val);
            tft.pushData(color);
        }
    }

    tft.endPushData();
}



void displayScale (){
  int pixel = 0;
  int scaleVal = 255/maxVal;
  Serial.println("Displaying 1:1 scale crop");

  for (int i=0; i<120; i++){
    for (int j=0; j<120; j++) {
      if (stretch == 0) {
        pixel = image[i+60][j+60];
      }
      else {
        pixel = (image[i+60][j+60]) * scaleVal;
      }
      tft.fillRect(119-i+4, j+4, 1, 1, tft.Color565(pixel, pixel, pixel));
    }
  }
}

// Zoom in LCD (switch to 2x zoom)
void displayZoomIn() {
  zoom = 2;
  y_start = 60;
  x_start = 60;
  displayPhoto();
  nextDisplayMove = -1;
}

// Zoom out LCD (switch to 1x zoom)
void displayZoomOut() {
  zoom = 1;
  y_start = 0;
  x_start = 0;
  displayPhoto();
  nextDisplayMove = -1;
}

// Shift display left (only applicable to 2x zoom)
void displayShiftLeft() {
  if (zoom == 2) {
    x_start = min(x_start + 40, 120);
    Serial.print("Moving left.");
    displayPhoto();
  } else {
    Serial.print("Zoom in to move left.");
  }
  nextDisplayMove = -1;
}

// Shift display right (only applicable to 2x zoom)
void displayShiftRight() {
  if (zoom == 2) {
    x_start = min(x_start - 40, 0);
    Serial.print("Moving right.");
    displayPhoto();
  } else {
    Serial.print("Zoom in to move right.");
  }
  nextDisplayMove = -1;
}

// Shift display up (only applicable to 2x zoom)
void displayShiftUp() {
  if (zoom == 2) {
    y_start = min(y_start + 40, 240);
    Serial.print("Moving up.");
    displayPhoto();
  } else {
    Serial.print("Zoom in to move up.");
  }
  nextDisplayMove = -1;
}

// Shift display down (only applicable to 2x zoom)
void displayShiftDown() {
  if (zoom == 2) {
    y_start = min(y_start - 40, 120);
    Serial.print("Moving down.");
    displayPhoto();
  } else {
    Serial.print("Zoom in to move down.");
  }
  nextDisplayMove = -1;
}

// General method for checking button input with debounce implemented
void checkButtonPress(
  int currentValue, 
  int &lastStableState, 
  unsigned long &lastDebounceTime, 
  int debounceDelay, 
  const char* buttonName, 
  int displayMoveValue, 
  int &nextDisplayMove
) {
  if (currentValue != lastStableState) {
    if (millis() - lastDebounceTime > debounceDelay) {
      lastStableState = currentValue;
      if (currentValue == HIGH) {
        Serial.print("display ");
        Serial.print(buttonName);
        Serial.println(" button pressed");
        nextDisplayMove = displayMoveValue;
      }
    }
    lastDebounceTime = millis();
  }
}

// Check all LCD buttons for inputs
void checkDisplayButtonInputs() {
  int shiftLeftValue = io.digitalRead(displayLeftPin);
  int shiftRightValue = io.digitalRead(displayRightPin);
  int shiftUpValue = io.digitalRead(displayUpPin);
  int shiftDownValue = io.digitalRead(displayDownPin);
  int zoomInValue = io.digitalRead(displayZoomInPin);
  int zoomOutValue = io.digitalRead(displayZoomOutPin);

  checkButtonPress(shiftLeftValue, displayLeftLastStableState, displayLeftLastDebounceTime, displayDebounceDelay, "LEFT", 1, nextDisplayMove);
  checkButtonPress(shiftRightValue, displayRightLastStableState, displayRightLastDebounceTime, displayDebounceDelay, "RIGHT", 2, nextDisplayMove);
  checkButtonPress(shiftUpValue, displayUpLastStableState, displayUpLastDebounceTime, displayDebounceDelay, "UP", 3, nextDisplayMove);
  checkButtonPress(shiftDownValue, displayDownLastStableState, displayDownLastDebounceTime, displayDebounceDelay, "DOWN", 4, nextDisplayMove);
  checkButtonPress(zoomInValue, displayZoomInLastStableState, displayZoomInLastDebounceTime, displayDebounceDelay, "ZOOM IN", 5, nextDisplayMove);
  checkButtonPress(zoomOutValue, displayZoomOutLastStableState, displayZoomOutLastDebounceTime, displayDebounceDelay, "ZOOM OUT", 6, nextDisplayMove);
}

// Complete case statement to translate button inputs into display actions
void updateDisplay() {
  switch (nextDisplayMove) {
    case 1:
      displayShiftLeft();
      break;
    case 2:
      displayShiftRight();
      break;
    case 3:
      displayShiftUp();
      break;
    case 4:
      displayShiftDown();
      break;
    case 5:
      displayZoomIn();
      break;
    case 6:
      displayZoomOut();
      break;
    default:
      break;
  }
}

// LCD loop to go in main loop
void displayLoop() {
  checkDisplayButtonInputs();
  if (nextDisplayMove != 0){
    updateDisplay();
  }
}
