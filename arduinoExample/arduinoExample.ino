/*
 * Arduino Example of apds9130-driver
 * \author Joshua Vasquez
 * \date November 12, 2014
 */

#include "embeddedCommon.h"
#include <vl6180x-driver.h>
#include <Wire.h>


void setup()
{
    Wire.begin();
    Serial.begin(115200);
    initVL6180X();
}

void loop()
{
    float rangeData = singleShotReadRange();
    Serial.println(rangeData);
    delay(50);
}
