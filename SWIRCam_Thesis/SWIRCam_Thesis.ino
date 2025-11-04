// SWIR Scanning Camera Code
#include <TFT_ILI9163C.h>
#include <SPI.h>
#include <SD.h>
#include "driver/gpio.h"
#include <Wire.h>
#include <SparkFunSX1509.h>
#include "bitmaps.h"
#include <pgmspace.h>

// SD Card SPI pin assignments
#define SD_CS   26
#define SD_CLK  18
#define SD_MOSI 23
#define SD_MISO 19

// LCD SPI pin assignments
#define LCD_CS   5
#define LCD_DC   27
#define LCD_RST  26
#define LEDC_CHANNEL     0       // PWM channel 0
#define LEDC_TIMER_BIT   8       // 8-bit resolution (0-255)
#define LEDC_BASE_FREQ   5000    // 5 kHz PWM frequency
#define LED_PIN          2       // GPIO2 for LED

// ADC SPI pins and fast CS pin writes
#define ADC_CS 14
#define ADC_CLK 0
#define ADC_MISO 4
#define ADC_LOW()   REG_WRITE(GPIO_OUT_W1TC_REG, (1 << ADC_CS))
#define ADC_HIGH()  REG_WRITE(GPIO_OUT_W1TS_REG, (1 << ADC_CS))

// Stepper Motor Pins
#define STEP_X 17
#define STEP_Y 25
#define SAVE_BUTTON 35
#define SWITCH_XY 15
#define DIR_X 16
#define DIR_Y 33
#define ENABLE_XY 32

// Create up TFT, ADC and I/O Expander objects
TFT_ILI9163C tft = TFT_ILI9163C(LCD_CS, LCD_DC, -1);
SPIClass *adcSPI = new SPIClass(HSPI);
SX1509 io;

// Captured image and downsampled display image global variables
#define WIDTH 240
#define HEIGHT 240
uint8_t image[HEIGHT][WIDTH];
uint8_t image_downsampled[HEIGHT / 2][WIDTH / 2];

//Stepper motor microstep pin inputs
//MS2,MS1:
//00: 1/8
//01: 1/32
//10: 1/64
//11: 1/16

