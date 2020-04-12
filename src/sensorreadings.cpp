
#include <Arduino.h>
#include "sensor_readings.h"

//void refresh_readings(Adafruit_BME280 *bme, TFT_eSPI *tft)
void refresh_readings(Adafruit_BME280 *bme,
                      TFT_eSPI *tft,
                      AdafruitIO_Feed *Temp,
                      AdafruitIO_Feed *Hum,
                      AdafruitIO_Feed *Pres,
                      AdafruitIO_Feed *Alt)
{

  uint16_t BackGroundColor = TFT_BLACK;
  uint16_t ForeGroundColor = TFT_WHITE;


  //Serial.println(" DEFAULT BME Reading //********/");
  float f_temperature = 10;
  float f_humidity = 20;
  float f_pressure = 30;
  float f_altitude = 40;

  digitalWrite(LED_PIN, HIGH);

  //read sensor ad load vars
  f_temperature = bme->readTemperature();
  f_humidity = bme->readHumidity();
  f_pressure = bme->readPressure() / 100.0F;
  f_altitude = bme->readAltitude(SEALEVELPRESSURE_HPA);

  Serial.println("TFT = Right Now...");
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
  Serial.print(f_temperature);
  Serial.println(" °C");

  tft->fillRect(5, 50, 140, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 50);
  tft->print(f_temperature);
  tft->println(" °C");

  // Humidity
  //print to serial port
  Serial.print(f_humidity);
  Serial.println(" %");

  tft->fillRect(5, 90, 130, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 90);
  tft->print(f_humidity);
  tft->print(" %");

  // Pressure
  //print to serial port
  Serial.print(f_pressure);
  Serial.println(" hPa");

  tft->fillRect(5, 130, 130, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 130);
  tft->print(f_pressure);
  tft->println(" hpa");

  // Appx altitude
  //print to serial port
  Serial.print(f_altitude);
  Serial.println(" m");

  tft->fillRect(5, 170, 130, 30, BackGroundColor); //clear out old text
  tft->setCursor(5, 170);
  tft->print(f_altitude);
  tft->println(" m");

  /******  Send Data to AdaIO   ******/
  Temp->save(f_temperature);
  Hum->save(f_humidity);
  //Pres->save(f_pressure);
  //Alt->save(f_altitude);

  //update AdaIO count
  DisplayTheCount(tft);
  digitalWrite(LED_PIN, LOW);
  //print to serial port
  Serial.println("-----v2----");
}
