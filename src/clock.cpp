#include "clock.h"

void refresh_clock(TFT_eSPI *tft, Timezone *timezone)
{

    // ezTime options https://github.com/ropg/ezTime
    // tft colors https:github.com/Bodmer/TFT

    tft->loadFont("NotoSansBold15");
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);

    //day of week
    tft->setCursor(220, 50);
    tft->fillRect(220, 50, 135, 70, TFT_BLACK); //clear date area
    tft->print(timezone->dateTime("l"));

    //month
    tft->setCursor(220, 65);
    //tft->fillRect(220,50,135,70,TFT_BLACK);         //clear date area
    tft->print(timezone->dateTime("F"));

    //day
    tft->print(" ");
    tft->print(timezone->dateTime("j"));

    //year
    tft->setCursor(220, 80);
    tft->print(timezone->dateTime("Y"));

    //time
    tft->setCursor(220, 95);
    tft->print(timezone->dateTime("H:i:s"));
}