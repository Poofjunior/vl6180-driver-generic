/*
 * Arduino Example of apds9130-driver
 * \author Joshua Vasquez
 * \date October 25, 2014
 */
#include "embeddedCommon.h"
#include "vl6180-driver.h"
#include <Wire.h>


void setup()
{
    Wire.begin();
    Serial.begin(115200);
    initVL6180();
}

void loop()
{
    float rangeData = singleShotReadRange();
    Serial.println(rangeData);
    delay(50);
}
