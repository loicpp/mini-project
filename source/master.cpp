#include "MicroBit.h"
#include "master.h"
#include "radio.h"
#include "microbit_global.h"

void onMasterData(MicroBitEvent)
{
    ManagedString s = uBit.radio.datagram.recv();
    uBit.serial.send(s);
    uBit.serial.sendChar('\n');
}

void master() {
    initRadio(onMasterData, true);

    uBit.serial.baud(115200);
    ManagedString str = "Init";
    uBit.serial.send(str);
    uBit.serial.sendChar('\n');
    //uBit.thermometer.setCalibration(uBit.thermometer.getTemperature());

    
    while (1) {
        //uBit.display.scroll(uBit.thermometer.getTemperature());
        //ManagedString msg = ManagedString("T: ") + ManagedString(uBit.thermometer.getTemperature()) + ManagedString("°C\r\n");
        //uBit.serial.send(msg);
        uBit.sleep(1000);
    }
}