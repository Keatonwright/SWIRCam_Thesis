// ---------------------------------------------------
// -------------------MEASUREMENT---------------------
// ---------------------------------------------------

// Read out 8 bit ADC (ADCS7476AIMFX)
uint16_t readADC_fast() {
  ADC_LOW();
  uint8_t hi = adcSPI->transfer(0x00);
  uint8_t lo = adcSPI->transfer(0x00);
  ADC_HIGH();
  return ((hi << 8) | lo) >> 4;  // 12-bit result, downsample to 8-bit
}

// Serial print live ADC value
void probe() {
  uint16_t adcValue = 0;
  for (int i=0; i<numSamples; i++) {
    adcValue += readADC_fast();
  }
  Serial.print(adcValue/numSamples/256., 3); // 8 bit adc
  Serial.println(", 0.0, 1.0");
  delay(10);
}

// Read out pixel to image array by taking a specified number of samples
void takeMeasurement(int row, int col) {
  int adcValue = 0;

  // take multiple readings and sum
  for (int i = 0; i < numSamples; i++) {
    adcValue += readADC_fast();
  }
  adcValue = adcValue / (numSamples/multiplier) - bias;  // average, multiply and subtract bias

  // clamp to uint8 range
  if (adcValue > 255) { adcValue = 255; }
  if (adcValue < 0) { adcValue = 0; }
  
  // update min and max values
  if (adcValue > maxVal) { maxVal = adcValue; }
  if (adcValue < minVal) { minVal = adcValue; }

  // Store value in image array with serpentine pattern
  if (row % 2 == 1) {
    image[239 - row][col] = adcValue;
  } else {
    image[239 - row][239 - col] = adcValue;
  }
}

