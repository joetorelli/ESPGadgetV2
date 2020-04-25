
#include "mqttController.h"

//read on/off string from Google assistant on control LED on IFTTT pin (7)
void LEDMessage(AdafruitIO_Data *data)
{

    DEBUG_PRINT("received value: ");
    DEBUG_PRINTLN(data->value());

    if (!strcmp(data->value(), "off"))
    {
        digitalWrite(IFTTTLED, LOW);
        //IFTTT_Flag = LOW;
    }

    if (!strcmp(data->value(), "on"))
    {
        digitalWrite(IFTTTLED, HIGH);
        //IFTTT_Flag = HIGH;
    }
}