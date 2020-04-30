#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdlib.h>

#include "i2cDriverHelper.h"

#include "sparkx_alpha_led_display.h" //Click here to get the library: http://librarymanager/All#Alphanumeric_Display by SparkFun
HT16K33 display;

int main()
{
    Aperture::USB::I2CDriverHelper i2CPort;
    if (!display.begin(i2CPort))
    {
        std::cerr << "Device did not acknowledge! Freezing." << std::endl;
        while (1);
    }
    std::cerr << "Display acknowledged." << std::endl;

    const char dispCstr[5] = "Milk";
    display.print(dispCstr);
}