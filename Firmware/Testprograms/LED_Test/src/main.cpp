/******************************************************************************
* file    main.cpp
*******************************************************************************
* brief   Main Program
*******************************************************************************
* author  Florian Baumgartner
* version 1.0
* date    2022-08-02
*******************************************************************************
* MIT License
*
* Copyright (c) 2022 Crelin - Florian Baumgartner
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell          
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#include <Arduino.h>
#include "console.h"
#include "utils.h"

#include <Adafruit_NeoPixel.h>


#define USER_BTN          0

Adafruit_NeoPixel strip(144*2, 21, NEO_RGBW + NEO_KHZ800);


void setup()
{
  console.begin();
  if(!utils.begin("DRIVE"))
  {
    console.error.println("[MAIN] Could not initialize utilities");
  }

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)

  for(int i = 0; i < strip.numPixels(); i++)
  {
    strip.fill(0);
    strip.setPixelColor(i, 255, 0, 0, 0);
    strip.show();
    delay(10);
  }
  delay(1000);

  strip.fill(strip.Color(255, 255, 255, 255));
  strip.show();
  delay(4000);
}

int i = 0;

void rainbow(int wait);

void loop()
{
  //strip.setPixelColor(i, 0, 0, 0, 255);
  //strip.setPixelColor(i, grün, rot, blau, weiss);
  //strip.setPixelColor(i, strip.Color(255, 0, 0, 0));
  //strip.setPixelColor(i, strip.Color(255, 0, 0, 0));
  rainbow(100);
  //strip.rainbow(10);
  //strip.fill(strip.Color(255, 255, 255, 255));


  //strip.fill(strip.Color(0, 0, 0, 0));
  //strip.show();
  //delay(20); // ms
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}