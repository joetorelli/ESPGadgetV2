
#ifndef SETTINGS_H

#define SETTINGS_H
#define LED_BUILTIN 2 // This is valid for my devkit
#include "TFT_eSPI.h" // ESP32 Hardware-specific library
#include "AdafruitIO_WiFi.h"
#include "network_config.h"
#include "ezTime.h" //ntp and other functions
#include "aux_functions.h"
#include "clock.h" //clock settings
#include "EEPROM.h"

#define LED_PIN 17
#define IFTTT 16
#define EEPROM_SIZE 4

//used for oled display testing
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#endif

#ifdef DEBUGLOG
#define DEBUGPRINT(x) Serial.print(x)
#define DEBUGPRINTDEC(x) Serial.print(x, DEC)
#define DEBUGPRINTLN(x) Serial.println(x)
#else
#define DEBUGPRINT(x)
#define DEBUGPRINTDEC(x)
#define DEBUGPRINTLN(x)
#endif
