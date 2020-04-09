
#ifndef SENSOR_READINGS_H

#define SENSOR_READINGS_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TFT_eSPI.h>
#include "sensor_readings.h"
#define SEALEVELPRESSURE_HPA (1013.25)

// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

void refresh_readings(Adafruit_BME280 *bme,
                      TFT_eSPI *tft,
                      AdafruitIO_Feed *Temp,
                      AdafruitIO_Feed *Hum); //,
                                             //AdafruitIO_Feed *Pres,
                                             //AdafruitIO_Feed *Alt);
//void refresh_readings(Adafruit_BME280 *bme, Adafruit_SSD1306 *OLED, TFT_eSPI *tft);
//void refresh_readings(Adafruit_BME280 *bme, Adafruit_SSD1306 *OLED);
//void ScanI2C(Adafruit_SSD1306 *OLED);
void ScanI2C();

//void refresh_readings(Adafruit_BME280 *bme);
//void refresh_readings(Adafruit_BME280 *bme, TFT_eSPI *tft);
//void refresh_readings(Adafruit_BME280 *bme, Adafruit_SSD1306 *OLED); //, TFT_eSPI *tft);
//void ScanI2C(Adafruit_SSD1306 *OLED);
#endif

#include "settings.h"