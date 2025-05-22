#include "MicroBit.h"
#include "iot.h"
#include "radio.h"
#include "microbit_global.h"

void onIotData(MicroBitEvent)
{
    ManagedString s = uBit.radio.datagram.recv();
    uBit.display.scroll(s);
}

void iot() {
    // Initialise the micro:bit runtime.
    initRadio(onIotData, false);

    while(1)
    {
    if (uBit.buttonA.isPressed()) {
            uBit.display.print("A");
            sendData("1");
    }
    else if (uBit.buttonB.isPressed()) {
            uBit.display.print("B");
            sendData("2");
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