// ---------------------------------------------------
// -------------------PARAMETERS----------------------
// ---------------------------------------------------

// Image capture parameters
int numSamples = 20;
int shiftPixels = 26; // works for 20 samples, shift of 19 works for 50
int multiplier = 1;
int bias = 0;
int timer = 1000;

// Image min and max pixel values
int minVal = 255;
int maxVal = 0;

// LCD display parameters
bool stretch = 0;
int zoom = 1;
int x_start = 0;
int y_start = 0;
const byte displayLeftPin = 2;
const byte displayRightPin = 3;
const byte displayDownPin = 5;
const byte displayUpPin = 4;
const byte displayZoomInPin = 0;
const byte displayZoomOutPin = 1;
const unsigned long displayDebounceDelay = 100;
unsigned long displayLeftLastDebounceTime = 0;
unsigned long displayRightLastDebounceTime = 0;
unsigned long displayUpLastDebounceTime = 0;
unsigned long displayDownLastDebounceTime = 0;
unsigned long displayZoomInLastDebounceTime = 0;
unsigned long displayZoomOutLastDebounceTime = 0;
int displayLeftLastStableState = LOW;
int displayRightLastStableState = LOW;
int displayUpLastStableState = LOW;
int displayDownLastStableState = LOW;
int displayZoomInLastStableState = LOW;
int displayZoomOutLastStableState = LOW;
int nextDisplayMove = 0; //0: Do nothing, 1: LEFT, 2: RIGHT, 3: UP, 4: DOWN, 5: ZOOM IN, 6: ZOOM OUT

// States from serial commands
bool running = false;
bool probing = false;
bool autoSave = false;
bool isSaved = false;

// Stepper motor parameters
int stepsUntilContactX = 20+50;
int stepsMultiplierX = 32;
int stepsUntilContactY = 17+52;
int stepsMultiplierY = 32;
int stepSignalDelay = 45;

// Fan parameters
const byte SX1509_ADDRESS = 0x3E;
const byte pwmPin = 10;
const byte fanUpPin = 12;
const byte fanDownPin = 11;
const byte greenLedPin = 13;
const byte yellowLedPin = 14;
const byte redLedPin = 15;
int fanNextState = -1;
int fanCurrentState = 2;
const unsigned long fanDebounceDelay = 100;
unsigned long fanUpLastDebounceTime = 0;
unsigned long fanDownLastDebounceTime = 0;
int fanUpLastStableState = LOW;
int fanDownLastStableState = LOW;

//Take photo pins
const byte takePhotoPin_0 = 9;
const byte takePhotoPin_5 = 8;
const byte takePhotoPin_10 = 6;
const byte calibratePin = 7;
bool photoTaken = false;



