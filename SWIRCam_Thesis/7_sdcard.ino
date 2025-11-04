//----------------------------------------------------
//-----------------------SD CARD----------------------
//----------------------------------------------------

// Main photo saving method. Saves as image_XXX.pgm where XXX
// is next smallest number up to 999
void savePhoto() {
  Serial.println("SAVING IMAGE TO SD CARD");
  String filename = getNextFilename();
  if (filename.length() > 0) {
    savePGM(filename.c_str());
  } else {
    Serial.println("Could not find free filename");
  }
}

// Method for saving a 240x240 array as a PGM file
void savePGM(const char* filename) {
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing.");
    return;
  }

  // Write the binary PGM header
  file.print("P5\n");
  file.print(String(WIDTH) + " " + String(HEIGHT) + "\n");
  file.print("255\n");

  // Write image data row by row
  for (int y = 0; y < HEIGHT; y++) {
    file.write(image[y], WIDTH);  // Write one row (240 bytes)
  }

  file.close();
  Serial.println(String("Image saved as ") + filename);
}

// Method for retrieving the next available file name
String getNextFilename() {
  int index = 1;
  String filename;

  while (true) {
    filename = "/image" + String(index) + ".pgm";
    if (!SD.exists(filename)) {
      return filename;
    }
    index++;
    if (index > 999) {  // Safety stop to prevent infinite loop
      return "";
    }
  }
}

