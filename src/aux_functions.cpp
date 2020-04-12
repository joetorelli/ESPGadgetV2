#include "aux_functions.h"
  //uint16_t BackGroundColor = TFT_BLACK;
  //uint16_t ForeGroundColor = TFT_WHITE;
//number of times AdaIO updated
int DisplayAdaIOCounter = 0;

void wifiStatus(TFT_eSPI *tft,
                AdafruitIO_WiFi *AdaIO)
{

    tft->loadFont("NotoSansBold15");
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft->fillRect(5, 220, 150, 20, TFT_BLACK);
    tft->setCursor(5, 220);
    tft->print(wl_status_to_string(WiFi.status()));
    //tft->print(AdaIO->networkStatus());
}

const char  *wl_status_to_string(wl_status_t status)
{

    switch (status)
    {
        case WL_NO_SHIELD:
            return "WL_NO_SHIELD";
            break;
        case WL_IDLE_STATUS:
            return "WL_IDLE_STATUS";
            break;
        case WL_NO_SSID_AVAIL:
            return "WL_NO_SSID_AVAIL";
            break;
        case WL_SCAN_COMPLETED:
            return "WL_SCAN_COMPLETED";
            break;
        case WL_CONNECTED:
            return "CONNECTED";
            break;
        case WL_CONNECT_FAILED:
            return "CONNECT_FAILED";
            break;
        case WL_CONNECTION_LOST:
            return "WL_CONNECTION_LOST";
            break;
        case WL_DISCONNECTED:
            return "DISCONNECTED";
            break;

        default:
            break;
    }
}

void DisplayTheCount(TFT_eSPI *tft)
{

    int TotalPosts = EEPROM.readInt(0);
Serial.print("TotalPosts= ");
Serial.println(TotalPosts);

   DisplayAdaIOCounter++; 
Serial.print("DisplayAdaIOCounter= ");
Serial.println(DisplayAdaIOCounter);


    if(DisplayAdaIOCounter > TotalPosts)
    {
        TotalPosts = DisplayAdaIOCounter;  //   + TotalPosts;
        EEPROM.writeInt(0,TotalPosts);
        EEPROM.commit();
    }
Serial.print("TotalPosts= ");
Serial.println(TotalPosts);
  Serial.print("EEPROM= ");
  Serial.println(EEPROM.readInt(0));


    tft->loadFont("NotoSansBold15");
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK );
    tft->fillRect(220,0,90,20,TFT_BLACK);
    tft->setCursor(220,0);
    tft->print(DisplayAdaIOCounter);
    tft->print("/");
    tft->print(EEPROM.readInt(0));


}