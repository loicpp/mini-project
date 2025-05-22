#include "MicroBit.h"
#include "iot.h"
#include "radio.h"
#include "microbit_global.h"

static ManagedString DISPLAY_SETTING = "TLH";

void onIotData(MicroBitEvent)
{
    ManagedString s = uBit.radio.datagram.recv();

    ManagedString output[MAX_SIZE];
    splitManagedString(s, output);
    bool isMaster = output[0].charAt(0) == 'M';

    if (isMaster) {
        DISPLAY_SETTING = output[1];
    }
}

void iot() {
    // Initialise the micro:bit runtime.
    initRadio(onIotData, false);
    while(1)
    {
    if (uBit.buttonA.isPressed()) {
        uBit.display.print("A");
        sendDataSensor("18", "19", "20");
    }
    else if (uBit.buttonB.isPressed()) {
        uBit.display.print("B");
        sendDataSensor("12", "14", "15");
    }
    //displaySensor();

    uBit.sleep(100);
    uBit.display.clear();
    }
}

void displaySensor() {
    // float temperature = bme.getTemperature();
    // float humidity = bme.getHumidity();
    // float pressure = bme.getPressure();
    
    
    // // Envoyer les données via le port série
    // uBit.serial.send("Temp: ");
    // uBit.serial.sendFloat(temperature);
    // uBit.serial.send(" C, Hum: ");
    // uBit.serial.sendFloat(humidity);
    // uBit.serial.send(" %, Pres: ");
    // uBit.serial.sendFloat(pressure);
    // uBit.serial.send(" hPa\n");
    
    // oled.clearDisplay();

    // oled.setCursor(0, 0);
    // oled.print("Temp: ");
    // oled.print(temperature);
    // oled.print(" C");

    // oled.setCursor(0, 10);
    // oled.print("Hum: ");
    // oled.print(humidity);
    // oled.print(" %");

    // oled.setCursor(0, 20);
    // oled.print("Pres: ");
    // oled.print(pressure);
    // oled.print(" hPa");

    // oled.display();
}