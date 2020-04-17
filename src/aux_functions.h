#include <Arduino.h>
#include "settings.h"

void wifiStatusStart(TFT_eSPI *tft, AdafruitIO_WiFi *AdaIO);
void wifiStatus(TFT_eSPI *tft, AdafruitIO_WiFi *AdaIO);

const char *wl_status_to_string(wl_status_t status);

//display the count when AdaIO updated
void DisplayTheCount(TFT_eSPI *tft);
