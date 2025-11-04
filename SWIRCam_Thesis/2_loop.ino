// ---------------------------------------------------
// -------------------LOOP----------------------------
// ---------------------------------------------------

void loop() {
  // Parse serial commands
  if (Serial.available()){
    command(Serial.readStringUntil('\n'));
  }

  // Probe and print adc values (use the serial plotter!)
  if (probing) {
    probe();
  }

  // Calibrate if button pressed
  if (io.digitalRead(calibratePin) == HIGH) {
    centre();
  }

  // Save if button pressed, photo has been taken, and has not been saved yet
  if (digitalRead(SAVE_BUTTON) == HIGH && photoTaken == true && isSaved == false) {
    savePhoto();
    isSaved = true;
  }

  // Run scan if button pressed or command entered
  if (io.digitalRead(takePhotoPin_0) == HIGH or running) {
    running = 0;
    zoom = 1;
    photoTaken = true;
    displayBitmapToRegion(digits[0]);
    takePhoto();
    displayPhoto();
    
    //Save if autosave on
    if (autoSave == true && isSaved == false){
      savePhoto();
      isSaved = true;
    }
  }

  if (io.digitalRead(takePhotoPin_5) == HIGH or running) {
    running = 0;
    zoom = 1;
    photoTaken = true;
    Serial.println("Taking photo in 5...");
    displayBitmapToRegion(digits[5]);
    for (int i = 4; i >= 1; i--) {
      delay(800);
      Serial.println(String(i) + "...");
      displayBitmapToRegion(digits[i]);
    }
    delay(800);
    displayBitmapToRegion(digits[0]);
    takePhoto();
    displayPhoto();

    //Save if autosave on
    if (autoSave == true && isSaved == false){
      savePhoto();
      isSaved = true;
    }
  }

  if (io.digitalRead(takePhotoPin_10) == HIGH or running) {
    running = 0;
    zoom = 1;
    photoTaken = true;
    Serial.println("Taking photo in 10...");
    displayBitmapToRegion(digits[10]);
    for (int i = 9; i >= 1; i--) {
      delay(800);
      Serial.println(String(i) + "...");
      displayBitmapToRegion(digits[i]);
    }
    delay(800);
    displayBitmapToRegion(digits[0]);
    takePhoto();
    displayPhoto();

    //Save if autosave on
    if (autoSave == true && isSaved == false){
      savePhoto();
      isSaved = true;
    }
  }

  //Fan loop
  fanLoop();

  //Display Loop
  displayLoop();
}

