// ---------------------------------------------------
// -------------------SETUP---------------------------
// ---------------------------------------------------

void setup() {
  Serial.begin(115200);

  // LCD Setup
  ledcSetup(LEDC_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL);
  ledcWrite(LEDC_CHANNEL, 96);  // 50% brightness
  // Startup LCD and fill with red
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(tft.Color565(255, 0, 0));
  Serial.println("LCD initialized");

  // IO Expander Setup
  Wire.begin();
  if (!io.begin(SX1509_ADDRESS)) {
    Serial.println("SX1509 not detected. Check wiring.");
    while (1);
  }
  Serial.println("I/O EXPANDER initialized");


  // Setup buttons
  pinMode(SAVE_BUTTON, INPUT_PULLDOWN);
  pinMode(SWITCH_XY, INPUT_PULLDOWN);
  io.pinMode(takePhotoPin_0, INPUT_PULLDOWN);
  io.pinMode(takePhotoPin_5, INPUT_PULLDOWN);
  io.pinMode(takePhotoPin_10, INPUT_PULLDOWN);
  io.pinMode(calibratePin, INPUT_PULLDOWN);

  // Setup x and y stepper motors (enable active-low)
  pinMode(STEP_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  pinMode(STEP_Y, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(ENABLE_XY, OUTPUT);
  digitalWrite(STEP_X, LOW);
  digitalWrite(STEP_Y, LOW);
  digitalWrite(ENABLE_XY, HIGH);
  Serial.println("STEPPER MOTORS initialized");

  // Configure FAN
  io.pinMode(pwmPin, ANALOG_OUTPUT);  // Configure as LED driver PWM
  io.analogWrite(pwmPin, 0);          // Start with 255 (off)
  io.pinMode(fanUpPin, INPUT_PULLDOWN);
  io.pinMode(fanDownPin, INPUT_PULLDOWN);
  io.pinMode(greenLedPin, ANALOG_OUTPUT);
  io.pinMode(yellowLedPin, ANALOG_OUTPUT);
  io.pinMode(redLedPin, ANALOG_OUTPUT);
  io.analogWrite(greenLedPin, 0);
  io.analogWrite(yellowLedPin, 0);
  io.analogWrite(redLedPin, 255);
  Serial.println("FAN initialized");
  
  // Configure ADC
  pinMode(ADC_CS, OUTPUT);
  digitalWrite(ADC_CS, HIGH);  // idle high
  adcSPI->begin(ADC_CLK, ADC_MISO, -1, ADC_CS);  // SCLK, MISO, MOSI, CS (MOSI unused) new: SCLK 0, MISO 4
  adcSPI->beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));  // 20 MHz SPI
  Serial.println("ADC initialized");

  // Configure SD card reader
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SPI)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized");

  // If all setup is successful, fill LCD with GREEN
  tft.fillScreen(tft.Color565(0, 255, 0));
}

