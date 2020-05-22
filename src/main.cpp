/******************************************************************
  
 Project     :  ESP32 Unleashed
 Libraries   :  Wire
                FS
                SD(esp32)
                Adafruit Unified Sensor
                Adafruit BME280 Library
                TFT_eSPI
                TaskScheduler
                Adafruit IO Arduino
                ezTime
                EEPROM
                RTC

 Author      :  Joet using VSCode with PlatformIO

 Description :  course:  esp32 Unleashed by tech explorations  

 Harware     :  ESP32 Dev Kit v3 
                BME280 sensor board
                2.8 inch, 240x320 pixels, SPI TFT  IL9341
                ESP32 development kit breakout board, v1.4

 Version     :  Pointers passing vars and objects
                multi file
                messed up git in section 10 so started over with version 2 and new git.


  TFT_eSPI  User_Setup.h pin settings:
  // For ESP32 Dev board (only tested with ILI9341 display)

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 15 // Chip select control pin
#define TFT_DC 0  // Data Command control pin
#define TFT_RST 2 // Reset pin (could connect to RST pin)
#define TOUCH_CS 5 // Chip select pin (T_CS) of touch screen              
******************************************************************/

/**********************************************************
Includes
**********************************************************/

#include <Arduino.h>
#include "settings.h"        // The order is important!
#include "sensor_readings.h" // The order is important!
#include "bmp_functions.h"
#include <TaskScheduler.h>
#include "network_config.h"
#include "clock.h"
#include <Wire.h>
#include <RtcDS3231.h>

/**********************************************
  Sub/Function Declarations
**********************************************/

/********  tasks callback functions  *********/
//update TFT
void refresh_readings_update();
void clock_update();
void SD_Update();

/**********************************************
  Pin Definitions
**********************************************/

// assign i2c pin numbers
#define I2c_SDA 21 //27
#define I2c_SCL 22 //26

/********************* Objects and vars  *******************/

/************************   BME  ***************************/

//temp sensor object
Adafruit_BME280 bme;

// BME280_ADDRESS = 0x77
// BME280_ADDRESS_ALTERNATE = 0x76
boolean IFTTT_Flag = LOW;

/********************  WDT   ******************************/
hw_timer_t *WDTimer = NULL;
const int WDTimeout = 10000;

// WDT timeout function
void IRAM_ATTR ResetGadget()
{
  ets_printf("Rebooting...\n");
  DEBUGPRINTLN("WDTimer Timeout");
  esp_restart();
}

/*********************  TFT Display  *********************/
TFT_eSPI tft = TFT_eSPI(); // create object (tft), of type (TFT_eSPI), from class (TFT_eSPI())
uint16_t BackGroundColor = TFT_BLACK;
uint16_t ForeGroundColor = TFT_WHITE;

/***************  task scheduler  **************************/
Task t1_AdaIOUpdate(10000, TASK_FOREVER, &refresh_readings_update); //can not pass vars with pointer in this function
Task t2_clock(1000, TASK_FOREVER, &clock_update);
Task t3_SDCard(15000, TASK_FOREVER, &SD_Update);
//Task t5_indicators(2000, TASK_FOREVER, &indicators);
Scheduler runner;

/**************  clock  ********************/
Timezone CST_TimeZone; //object for time zone

/**************  adafruit.io feeds  ************************/
AdafruitIO_WiFi AdaIO(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASSWORD);
AdafruitIO_Feed *Temperature = AdaIO.feed("iogadget.temperature");
AdafruitIO_Feed *Humidity = AdaIO.feed("iogadget.humidity");
AdafruitIO_Feed *Pressure = AdaIO.feed("iogadget.pressure");
AdafruitIO_Feed *Altitude = AdaIO.feed("iogadget.altitude");
AdafruitIO_Feed *LEDControl = AdaIO.feed("iogadget.led");

/*************************  Setup   ******************************/

int addr = 0;
void setup()
{

  //serial port
  Serial.begin(115200);
  pinMode(UpdateLED, OUTPUT); //AdaIOUpdate Task Indicator
  pinMode(TouchLED, OUTPUT);  //GLCD Touch Indicator
  pinMode(IFTTTLED, OUTPUT);  // IFTTT indicator

  DEBUGPRINTLN("DEBUG Print Enabled");

  // WDT setup, cause system reboot
  //use timer 0 ,divide clock by 80, count up
  WDTimer = timerBegin(0, 80, true);
  //create interrupt, function to call,
  timerAttachInterrupt(WDTimer, &ResetGadget, true);
  // length of time out
  timerAlarmWrite(WDTimer, WDTimeout * 20000, false);
  // enable interrupt
  timerAlarmEnable(WDTimer);

  //init eeprom
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    DEBUGPRINTLN("EEPROM INIT Failed");
    delay(10000);
  }
  else
  {
    DEBUGPRINTLN("EEPROM INIT OK");
  }

  //test for bad alue (neg number)
  if (EEPROM.readInt(0) < 0)
  {
    //IF neg then clear
    EEPROM.writeInt(0, 0);
    EEPROM.commit();
    //Serial.println("Inside if ");
  }

  /*************** set tft screen  *************/
  tft.init(); // initialize tft
              // Calibrate the touch interface
  // This only needs to be done once
  // The calibration data will be stored in the SPIFS
  // It is important to calibrate the touch interface after the display is rotated
  calibrate_touch_screen(&tft);

  tft.setRotation(1);                                 // orientation
  tft.setTextColor(ForeGroundColor, BackGroundColor); // set text to foreground and background color
  tft.fillScreen(BackGroundColor);                    // clear screen with background color
  tft.setCursor(0, 0);                                // position cursor to top left
  tft.println("Hello");                               // print text
  tft.println("Starting IOT Gadget");                 // print text
  DEBUGPRINTLN("TFT INIT OK");

  /********* file system  **********/
  if (!SPIFFS.begin())
  {
    tft.println("SPIFFS initialisation failed!");
    while (1)
      yield(); // Stay here twiddling thumbs waiting
  }
  else
  {
    DEBUGPRINTLN("SPIFFS INIT OK");
  }

  // show bmp on display
  drawBmp("/V4.bmp", 225, 190, &tft);
  //drawBmp("/te2.bmp", 150, 160, &tft);   //150, 160, &tft);             //show bitmap

  /*********   init i2c  *********/
  Wire.begin(I2c_SDA, I2c_SCL);
  bool status; // connect status
  DEBUGPRINTLN("I2C INIT OK");

  /**********  init i2c sensor  ************/

  tft.println("Init Sensor");

  status = bme.begin(BME280_ADDRESS_ALTERNATE); // get status of tft

  if (!status) // test status
  {
    tft.println("Can't find BME280");
    DEBUGPRINTLN("Can't find BME280, it may have fell on the floor");
    //while (1);
  }
  else
  {
    tft.println("Found BME280");
    DEBUGPRINTLN("Found BME280");
  }
  /**********************  init rtc  **************************/

  RtcDS3231<TwoWire> Rtc(Wire);
  Rtc.Begin();

  // WDT reset
  timerWrite(WDTimer, 0);
  DEBUGPRINTLN("WDT INIT OK");

  /*********  adafruit IO connect to wifi  ***********/

  tft.println("Init WIFI");
  DEBUGPRINTLN("Start WiFi INIT");
  wifiStatusStart(&tft, &AdaIO);
  DEBUG_PRINTLN("AdaIO Connect");
  AdaIO.connect();

  wifiStatusStart(&tft, &AdaIO);
  //wait for connection
  do
  {
    tft.print(".");
    delay(500);
  } while (AdaIO.status() < AIO_CONNECTED);

  //callback for MQTT
  //whena msg is received from Adafruit
  DEBUG_PRINTLN("MQTT Controller...");
  LEDControl->onMessage(LEDMessage);

  //tft.setCursor(5, 50);
  tft.println("WIFI connected");

  // WDT reset
  timerWrite(WDTimer, 0);

  /******* set up clock ************/
  tft.print("Getting Time. Please Wait");

  do
  {
    tft.print(".");
  } while (!waitForSync(1));

  CST_TimeZone.setLocation("America/Chicago");

  /************* set up task runner  *************/
  runner.init();
  runner.addTask(t1_AdaIOUpdate);
  runner.addTask(t2_clock);
  runner.addTask(t3_SDCard);
  t1_AdaIOUpdate.enable();
  t2_clock.enable();
  t3_SDCard.enable();

  //clear tft and load bmp
  tft.fillScreen(BackGroundColor);
  drawBmp("/V4.bmp", 225, 190, &tft);
  //drawBmp("/te2.bmp", 150, 160, &tft);   //150, 160, &tft);

  wifiStatus(&tft, &AdaIO);

  /*********************  SD Card  *************************/
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS))
  {
    Serial.println("SD Card failed");
    return;
  }

  uint8_t CardType = SD.cardType();

  if (CardType == CARD_NONE)
  {
    Serial.println("No SD Card Found");
    return;
  }

  Serial.print("SD Card Type: ");

  if (CardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (CardType == CARD_SD)
  {
    Serial.println("SCSC");
  }
  else if (CardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("Unkown Type");
  }
  String Tempp;
  uint64_t CardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", CardSize);
  Serial.printf("Total Space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used Space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  listDir(SD, "/", 0);
}

/**************************  loop  *******************/
void loop()
{

  // WDT reset
  timerWrite(WDTimer, 0);

  /***********  run tasks  **************/
  runner.execute();
  //connect to AdaIOfor MQTT
  AdaIO.run();
  readTouch(&tft);
}

/**********************************************
  Sub/Function Definitions
**********************************************/
// use these function as a wrapper to pass the vars
// these are called from Task Scheduler

//update temp to screen
void refresh_readings_update()
{
  refresh_readings(&bme,
                   &tft,
                   Temperature,
                   Humidity,
                   Pressure,
                   Altitude);
  //LEDControl);

  wifiStatus(&tft, &AdaIO);
}

// update clock to screen
void clock_update()
{

  refresh_clock(&tft, &CST_TimeZone);
}

void SD_Update()
{

  Refresh_SD(&CST_TimeZone, &bme);
}
