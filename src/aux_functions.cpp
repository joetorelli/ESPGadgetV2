#include "aux_functions.h"
//uint16_t BackGroundColor = TFT_BLACK;
//uint16_t ForeGroundColor = TFT_WHITE;
//number of times AdaIO updated
int DisplayAdaIOCounter = 0;

void wifiStatusStart(TFT_eSPI *tft, AdafruitIO_WiFi *AdaIO)
{
    tft->println(wl_status_to_string(WiFi.status()));
    //tft->print(AdaIO->networkStatus());
}

void wifiStatus(TFT_eSPI *tft, AdafruitIO_WiFi *AdaIO)
{
    tft->loadFont("NotoSansBold15");
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft->fillRect(5, 220, 150, 20, TFT_BLACK);
    tft->setCursor(5, 220);
    tft->print(wl_status_to_string(WiFi.status()));
    //tft->print(AdaIO->networkStatus());
}

const char *wl_status_to_string(wl_status_t status)
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
    //read from EEPROM
    int TotalPosts = EEPROM.readInt(0);
    DEBUGPRINT("TotalPosts= ");
    DEBUGPRINTLN(TotalPosts);

    //inc current countercounter
    DisplayAdaIOCounter++;
    DEBUGPRINT("DisplayAdaIOCounter= ");
    DEBUGPRINTLN(DisplayAdaIOCounter);

    // inc total count and write to EEPROM
    TotalPosts++;
    EEPROM.writeInt(0, TotalPosts);
    EEPROM.commit();

    DEBUGPRINT("TotalPosts= ");
    DEBUGPRINTLN(TotalPosts);
    DEBUGPRINT("EEPROM= ");
    DEBUGPRINTLN(EEPROM.readInt(0));

    //display on tft
    tft->loadFont("NotoSansBold15");
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft->fillRect(220, 0, 90, 20, TFT_BLACK);
    tft->setCursor(220, 0);
    tft->print(DisplayAdaIOCounter);
    tft->print("/");
    tft->print(EEPROM.readInt(0));
}

void calibrate_touch_screen(TFT_eSPI *tft)
{
    uint16_t calibrationData[5];
    uint8_t calDataOK = 0;

    DEBUGPRINTLN("starting touch calibration");

    // tft->init();

    tft->setRotation(1); // instead of three
    tft->fillScreen((0xFFFF));

    tft->setCursor(20, 0, 2);
    tft->setTextColor(TFT_BLACK, TFT_WHITE);
    tft->setTextSize(1);
    tft->println("calibration run");

    // check file system
    if (!SPIFFS.begin())
    {
        DEBUGPRINTLN("formating file system");

        SPIFFS.format();
        SPIFFS.begin();
    }

    // check if calibration file exists
    if (SPIFFS.exists(CALIBRATION_FILE))
    {
        fs::File f = SPIFFS.open(CALIBRATION_FILE, "r"); // Using the fs namespace with :: because class File is inside the fs namespace. See FS.h
        if (f)
        {
            if (f.readBytes((char *)calibrationData, 14) == 14)
                calDataOK = 1;
            f.close();
        }
    }
    if (calDataOK)
    {
        // calibration data valid
        tft->setTouch(calibrationData); // To be able to use this function, you must set and uncomment the #define TOUCH_CS 5 in User_Setup.h in TFT_eSPI library
                                        // In PlatformIO, this is stored in .piolibdeps.
    }
    else
    {
        // data not valid. recalibrate
        tft->calibrateTouch(calibrationData, TFT_WHITE, TFT_RED, 15);
        // store data
        fs::File f = SPIFFS.open(CALIBRATION_FILE, "w");
        if (f)
        {
            f.write((const unsigned char *)calibrationData, 14);
            f.close();
        }
    }

    tft->fillScreen((0xFFFF));
}

void readTouch(TFT_eSPI *tft)
{

    uint16_t x, y;

    if (tft->getTouch(&x, &y)) //touch coord
    {
        if (x < 320 && y < 240) //cover the screen
        {
            //print position on screen
            tft->fillRect(220, 150, 70, 40, TFT_BLACK);
            tft->setCursor(220, 150, 2);
            tft->printf("x: %i    ", x);
            tft->setCursor(220, 165, 2);
            tft->printf("y: %i    ", y);

            //tft->drawPixel(x, y, TFT_YELLOW);     //leave a turd on touch point

            //if you touch position numbers turn on led
            if ((x > 220 && x < 280) && (y > 140 && y < 190))
            {
                digitalWrite(IFTTT_PIN, HIGH);
            }
            // if you touch logo clear coor and led
            else if ((x > 225 && x < 320) && (y > 200 && 240))
            {
                digitalWrite(IFTTT_PIN, LOW);
                tft->fillRect(220, 150, 70, 40, TFT_BLACK);
            }
        }
    }
}
