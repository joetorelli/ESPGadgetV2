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
#include "settings.h"        // The order is important!
#include "sensor_readings.h" // The order is important!
#include "bmp_functions.h"
#include <TaskScheduler.h>
#include "network_config.h"

/**********************************************
  Sub/Function Declarations
**********************************************/

/********  tasks callback functions  *********/
//update TFT
void refresh_readings_update();
void clock_update();

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

//temp sensor object
Adafruit_BME280 bme;

// BME280_ADDRESS = 0x77
// BME280_ADDRESS_ALTERNATE = 0x76

/*********************  TFT Display  *********************/
TFT_eSPI tft = TFT_eSPI(); // create object (tft), of type (TFT_eSPI), from class (TFT_eSPI())
uint16_t BackGroundColor = TFT_BLACK;
uint16_t ForeGroundColor = TFT_WHITE;

/***************  task scheduler  **************************/
Task t1_AdaIOUpdate(10000, TASK_FOREVER, &refresh_readings_update); //can not pass vars with pointer in this function
Task t2_clock(1000, TASK_FOREVER, &clock_update);
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

/*************************  Setup   ******************************/


int addr = 0;
void setup()
{

  //serial port
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);

  //init eeprom
 if (!EEPROM.begin(EEPROM_SIZE))
 {
   Serial.println("EEPROM INit Failed");
   delay(10000);
 }
 /* 
  //test for bad alue (neg number)
  if (EEPROM.readInt(0) < 0)
  {
    //IF ned then clear
    EEPROM.writeInt(0,5);
    EEPROM.commit();
    Serial.println("Inside if ");
  }
*/
for (int  i = 0; i < EEPROM_SIZE; i++)
{
  Serial.print(int(EEPROM.readInt(i)));
  Serial.print(" ");
}





  /********* file system  **********/
  if (!SPIFFS.begin())
  {
    tft.println("SPIFFS initialisation failed!");
    while (1)
      yield(); // Stay here twiddling thumbs waiting
  }

  /*************** set tft screen  *************/
  tft.init();                                         // initialize tft
  tft.setRotation(1);                                 // orientation
  tft.setTextColor(ForeGroundColor, BackGroundColor); // set text to foreground and background color
  tft.fillScreen(BackGroundColor);                    // clear screen with background color
  tft.setCursor(0, 0);                                // position cursor to top left
  tft.println("Hello");                               // print text
  tft.println("Starting IOT Gadget");                 // print text
  drawBmp("/te.bmp", 150, 160, &tft);                 //show bitmap

  /*********   init i2c  *********/
  Wire.begin(I2c_SDA, I2c_SCL);
  bool status; // connect status

  /**********  init i2c sensor  ************/

  tft.println("Init Sensor");

  status = bme.begin(BME280_ADDRESS_ALTERNATE); // get status of tft

  if (!status) // test status
  {
    tft.println("Can't find BME280");
    Serial.println("Can't find BME280, it may have fell on the floor");
    //while (1);
  }
  else
  {
    tft.println("Found BME280");
    Serial.println("Found BME280");
  }

  /*********  adafruit IO connect to wifi  ***********/

tft.print("Init WIFI");
  wifiStatus(&tft, &AdaIO);
  AdaIO.connect();
  wifiStatus(&tft, &AdaIO);
  //wait for connection
  do
  {
    tft.print(".");
    delay(500);
  } while (AdaIO.status() < AIO_CONNECTED);

  tft.println("WIFI connected");
  delay(500);
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
  t1_AdaIOUpdate.enable();
  t2_clock.enable();

  //clear tft and load bmp
  tft.fillScreen(BackGroundColor);
  drawBmp("/te.bmp", 150, 160, &tft);
  wifiStatus(&tft, &AdaIO);
}

/**************************  loop  *******************/
void loop()
{

  /***********  run tasks  **************/
 runner.execute();
/*
int val = int(random(10020));
  EEPROM.writeInt(addr, val);
  delay(100);
  Serial.print(val); Serial.print(" ");
 addr = addr + 1;
  if (addr == EEPROM_SIZE)
  {
    Serial.println();
    addr = 0;
    EEPROM.commit();
    Serial.print(EEPROM_SIZE);
    Serial.println(" bytes written on Flash . Values are:");
    for (int i = 0; i < EEPROM_SIZE; i++)
    {
      Serial.print(int(EEPROM.readInt(i))); Serial.print(" ");
      delay(100);
    }
    Serial.println(); Serial.println("----------------------------------");
  }

  delay(500);  
*/
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

  wifiStatus(&tft, &AdaIO);
}

// update clock to screen
void clock_update()
{

  refresh_clock(&tft, &CST_TimeZone);
}
