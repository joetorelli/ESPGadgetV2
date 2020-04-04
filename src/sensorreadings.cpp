
#include <Arduino.h>
#include "sensor_readings.h"

void refresh_readings(Adafruit_BME280 *bme, TFT_eSPI *tft)
{

  uint16_t BackGroundColor = TFT_BLACK;
  uint16_t ForeGroundColor = TFT_WHITE;

  // OLED->clearDisplay();
  // OLED->setTextSize(1);
  // OLED->setCursor(0,0);

  //Serial.println(" DEFAULT BME Reading //********/");
  float f_temperature = 10;
  float f_humidity = 20;
  float f_pressure = 30;
  float f_altitude = 40;

  //digitalWrite(LED_BUILTIN, HIGH);

  //read sensor ad load vars
  f_temperature = bme->readTemperature();
  f_humidity = bme->readHumidity();
  // f_pressure = bme->readPressure() / 100.0F;
  // f_altitude = bme->readAltitude(SEALEVELPRESSURE_HPA);

  Serial.println("TFT = Right Now...");
  //tft->setTextColor(ForeGroundColor, BackGroundColor);
  //tft->setRotation(1);
  tft->setCursor(5, 5);
  tft->println("Right Now...");
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

  //digitalWrite(LED_BUILTIN, LOW);
  //print to serial port
  Serial.println("-----v2----");
}

/************  ScanI2C  ***************/
void ScanI2C(Adafruit_SSD1306 *OLED)
//void ScanI2C()
{

  OLED->clearDisplay();
  OLED->setTextSize(1);
  OLED->setCursor(0, 0);

  byte DeviceError, DeviceAddress;
  int NumberOfDevices;

  Serial.println("Scanning...");
  // OLED->println("Scanning I2C...");
  // OLED->display();

  NumberOfDevices = 0;

  for (DeviceAddress = 1; DeviceAddress < 127; DeviceAddress++)
  {

    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the DeviceAddress.
    Wire.beginTransmission(DeviceAddress);
    DeviceError = Wire.endTransmission();

    if (DeviceError == 0)
    {

      Serial.print("I2C device found at DeviceAddress 0x");
      Serial.print("0");
      Serial.println(DeviceAddress, HEX);

      switch (DeviceAddress)
      {
      case 0x3C:
        OLED->print("OLED @ DeviceAdr 0x");
        OLED->println(String(DeviceAddress, HEX));
        break;

      case 0x76:
        OLED->print("BME  @ DeviceAdr 0x");
        OLED->println(String(DeviceAddress, HEX));
        break;

      case 0x77:
        OLED->print("BME  @ DeviceAdr 0x");
        OLED->println(String(DeviceAddress, HEX));
        break;

      default:
        OLED->print("I2C  @ DeviceAdr 0x");
        OLED->println(String(DeviceAddress, HEX));
        break;
      }

      NumberOfDevices++;
      OLED->display();
    }

    else if (DeviceError == 4)
    {

      Serial.print("Unknow DeviceError at DeviceAddress 0x");
      OLED->print("Unknow DeviceError at DeviceAddress 0x");
      //Serial.print("0");
      Serial.println(DeviceAddress, HEX);
      OLED->println(String(DeviceAddress, HEX));
      OLED->display();
    }
  }

  if (NumberOfDevices == 0)
  {

    Serial.println("No I2C devices found\n");
    OLED->print("No I2C devices found");
  }
  else
  {

    Serial.println("done\n");

    // OLED->print("done");
  }

  OLED->display();
}
