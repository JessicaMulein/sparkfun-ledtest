#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdlib.h>

#include "i2cDriverHelper.h"

#include "sparkx_alpha_led_display.h" //Click here to get the library: http://librarymanager/All#Alphanumeric_Display by SparkFun
HT16K33 display;

int main()
{
    if (display.begin() == false)
    {
        std::cout << "Device did not acknowledge! Freezing." << std::endl;
        while (1);
    }
    std::cout << "Display acknowledged." << std::endl;

    std::string disp = "Milk";
    display.print(disp.c_str());
}