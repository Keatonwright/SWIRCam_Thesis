// ---------------------------------------------------
// -------------------SCANNING------------------------
// ---------------------------------------------------

// Full image capture routine
void takePhoto() {
  isSaved = false;
  digitalWrite(ENABLE_XY, LOW);
  runScan();
  resetAfterScan();
  digitalWrite(ENABLE_XY, HIGH);

  Serial.println("Downsampling (2x2 averaging) for LCD");  // 120x120 result
  for (int y = 0; y < HEIGHT; y+=2) {
    for (int x = 0; x < WIDTH; x+=2) {
      uint16_t sum = image[y][x]
                   + image[y][x+1]
                   + image[y+1][x]
                   + image[y+1][x+1];
      image_downsampled[y/2][x/2] = sum/4;
    }
  }
}

// Run an image scan
void runScan() {
  Serial.println("Taking a scan");
  int dirX = 0;
  int dirY = 1;

  digitalWrite(DIR_X, dirX);
  digitalWrite(DIR_Y, dirY);

  delay(10);
  unsigned long start = micros();
  Serial.println("Starting 240 pixel row");
  for (int j = 0; j < HEIGHT; j++){
    Serial.print("-"); Serial.print(j); Serial.println("-");

    // Pre shift
    if (shiftPixels > 0) {
      for (int i=0; i<shiftPixels; i++) { moveStepX(); }
    }

    // Scan row by iterating over columns
    for (int i = 0; i < WIDTH; i++){
      // Take measurement and store in global image array
      takeMeasurement(j,i);
      moveStepX();
    }

    // Post shift
    if (shiftPixels < 0) {
      for (int i=0; i<-shiftPixels; i++) { moveStepX(); }
    }

    // Move to next row
    moveStepY();

    // Change direction of fast pin
    dirX = (dirX+1)%2;
    digitalWrite(DIR_X, dirX);
    delayMicroseconds(200);
  }

  // Print scan statistics
  unsigned long end = micros();
  const float perPixel = (end-start)/(240*240.0);
  Serial.printf("240 rows of 240 Pixel Read Operation took %.2f seconds, or %.2f microseconds per pixel\n", (end - start)/1000000., perPixel);
  Serial.print("Min  :  ");
  Serial.println(minVal/255., 3);
  Serial.print("Max  :  ");
  Serial.println(maxVal/255., 3);
  Serial.println("");
}

// Reset position for next image
void resetAfterScan() {
  delayMicroseconds(200);
  Serial.println("Resetting position in 3...");
  delay(500);
  Serial.println("2...");
  delay(500);
  Serial.println("1...");
  delay(500);
  digitalWrite(DIR_Y, LOW);

  for(int i = 0; i < 240; i++){
    moveStepY();
  }
  Serial.println("Position reset");
}

