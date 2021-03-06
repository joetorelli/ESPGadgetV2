
#include <Arduino.h>
#include "sensor_readings.h"
#include "mqttController.h"

//void refresh_readings(Adafruit_BME280 *bme, TFT_eSPI *tft)
void refresh_readings(Adafruit_BME280 *bme,
                      TFT_eSPI *tft,
                      AdafruitIO_Feed *Temp,
                      AdafruitIO_Feed *Hum,
                      AdafruitIO_Feed *Pres,
                      AdafruitIO_Feed *Alt)
//AdafruitIO_Feed *LEDControl)

{

  uint16_t BackGroundColor = TFT_BLACK;
  uint16_t ForeGroundColor = TFT_WHITE;

  //DEBUGPRINTln(" DEFAULT BME Reading //********/");
  float f_temperature = 0;
  float f_humidity = 0;
  float f_pressure = 0;
  float f_altitude = 0;

  digitalWrite(UpdateLED, HIGH);

  //read sensor ad load vars
  f_temperature = bme->readTemperature();
  f_humidity = bme->readHumidity();
  f_pressure = bme->readPressure() / 100.0F;
  f_altitude = bme->readAltitude(SEALEVELPRESSURE_HPA);

  DEBUGPRINTLN("TFT = Right Now...");
  //tft->setTextColor(ForeGroundColor, BackGroundColor);
  //tft->setRotation(1);
  tft->setCursor(5, 5);
  tft->println("Right Now...");
  tft->fillRect(5, 5, 100, 30, BackGroundColor); //clear out old text

  tft->setTextColor(TFT_YELLOW, BackGroundColor);
  tft->loadFont("NotoSansBold15");
  tft->setTextColor(ForeGroundColor, BackGroundColor);
  tft->setRotation(1);
  tft->setCursor(5, 5);
  tft->println("Right Now...");
  tft->setTextColor(TFT_YELLOW, BackGroundColor);

  // Temperature
  //print to serial port
  DEBUGPRINT(f_temperature);
  DEBUGPRINTLN(" °C");

  tft->fillRect(5, 50, 140, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 50);
  tft->print(f_temperature);
  tft->println(" °C");

  // Humidity
  //print to serial port
  DEBUGPRINT(f_humidity);
  DEBUGPRINTLN(" %");

  tft->fillRect(5, 90, 130, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 90);
  tft->print(f_humidity);
  tft->print(" %");

  // Pressure
  //print to serial port
  DEBUGPRINT(f_pressure);
  DEBUGPRINTLN(" hPa");

  tft->fillRect(5, 130, 130, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 130);
  tft->print(f_pressure);
  tft->println(" hpa");

  // Appx altitude
  //print to serial port
  DEBUGPRINT(f_altitude);
  DEBUGPRINTLN(" m");

  tft->fillRect(5, 170, 130, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 170);
  tft->print(f_altitude);
  tft->println(" m");

  /******  Send Data to AdaIO   ******/
  Temp->save(f_temperature);
  Hum->save(f_humidity);
  //LEDControl->save(IFTTT_Flag);
  //Pres->save(f_pressure);
  //Alt->save(f_altitude);

  //update AdaIO count
  DisplayTheCount(tft);

  digitalWrite(UpdateLED, LOW);
  //print to serial port
  DEBUGPRINTLN("-----v2----");
}
