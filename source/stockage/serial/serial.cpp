#include "MicroBit.h"
#include <string>

MicroBit uBit;

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.serial.baud(115200);
    ManagedString str = "Init";
    uBit.serial.send(str);
    uBit.serial.sendChar('\n');
    uBit.thermometer.setCalibration(uBit.thermometer.getTemperature());
    
    while (1) {
        uBit.display.scroll(uBit.thermometer.getTemperature());
        ManagedString msg = ManagedString("T: ") + ManagedString(uBit.thermometer.getTemperature()) + ManagedString("°C\r\n");
        uBit.serial.send(msg);
        uBit.sleep(1000);
    }

    release_fiber();
}