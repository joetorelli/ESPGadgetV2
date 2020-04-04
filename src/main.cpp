/******************************************************************
  
 Project     :  ESP32U leashed
 Libraries   :  TFT_eSPI
 Author      :  VSCode 
 Description : course:  esp32 Unleashed by peter  section 8
 
 
 Version     : Pointers passing vars and objects
               multi file
              messed up git in section 10 so started over with version 2 and new git.
******************************************************************/

/**********************************************************
Includes
**********************************************************/

#include <Arduino.h>
#include "sensor_readings.h"
//#include "TFT_eSPI.h" // ESP32 Hardware-specific library
#include "settings.h" // The order is important!
#include "bmp_functions.h"

/**********************************************
  Pin Definitions
**********************************************/

// assign i2c pin numbers
#define I2c_SDA 21 //27
#define I2c_SCL 22 //26

/********************* Objects and vars  *******************/

/**************************************   BME  ****************************/
// used for calcs
#define SEALEVEL_HPA (1013.25)

Adafruit_BME280 bme;

// BME280_ADDRESS = 0x77
// BME280_ADDRESS_ALTERNATE = 0x76

/*********************  TFT Display  *********************/
TFT_eSPI tft = TFT_eSPI(); // create object (tft), of type (TFT_eSPI), from class (TFT_eSPI())
uint16_t BackGroundColor = TFT_BLACK;
uint16_t ForeGroundColor = TFT_WHITE;
/**********************************************
  Sub/Function Declarations
**********************************************/

/*************************  Setup   ******************************/

void setup()
{

  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    tft.println("SPIFFS initialisation failed!");
    while (1)
      yield(); // Stay here twiddling thumbs waiting
  }

  Wire.begin(I2c_SDA, I2c_SCL);
  bool status; // connect TFT status

  status = bme.begin(BME280_ADDRESS_ALTERNATE); // get status of tft

  if (!status) // test status
  {

    Serial.println("Can't find BME280, it may have fell on the floor");
    //while (1);
  }
  else
  {
    Serial.println("Found BME280");
  }

  tft.init();                                         // initialize tft
  tft.setRotation(1);                                 // orientation
  tft.setTextColor(ForeGroundColor, BackGroundColor); // set text to foreground and background color
  tft.fillScreen(BackGroundColor);                    // clear screen with background color
  tft.setCursor(0, 0);                                // position cursor to top left
  tft.println("Hello");                               // print text
  tft.println("Starting BME sensor");                 // print text
  delay(1000);
  tft.fillScreen(BackGroundColor);
  drawBmp("/te.bmp", 150, 160, &tft);

  delay(1000);
}

/**************************  loop  *******************/
void loop()
{

  refresh_readings(&bme, &tft);

  delay(2000);
}

/**********************************************
  Sub/Function Definitions
**********************************************/
