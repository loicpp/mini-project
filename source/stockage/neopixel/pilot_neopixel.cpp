/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"
#include "neopixel.h"

MicroBit uBit;

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    MicroBitPin pin = uBit.io.P0;
    Neopixel neo = Neopixel(uBit.io.P0.name, 1);

    while (1) {
        neo.setColor(0, 255,0,0);
        neo.show();
        uBit.sleep(1000);
        neo.setColor(0, 0,255,0);
        neo.show();
        uBit.sleep(1000);
        neo.setColor(0, 0,0,255);
        neo.show();
        uBit.sleep(1000);
    }

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

/*
int temp;

uBit.thermometer.setCalibration(uBit.thermometer.getTemperature());

while(true) {
    temp = uBit.thermometer.getTemperature();  // This is now calibrated!
    uBit.display.print(temp);
    uBit.sleep(1000);
}
*/