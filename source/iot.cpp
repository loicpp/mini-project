#include "MicroBit.h"
#include "iot.h"
#include "radio.h"
#include "microbit_global.h"
#include "bme280.h" 
#include "veml6070.h"
#include "tsl256x.h"
#include "ssd1306.h"
#include <stdint.h>
#include <string.h>

static ManagedString DISPLAY_SETTING = "TLH"; // Ordre d'affichage sur l'écran OLED (par défaut: : Temperature, Luminosity, Humidity)
MicroBitI2C i2c(I2C_SDA0,I2C_SCL0);

void onIotData(MicroBitEvent)
{
    ManagedString raw_message = uBit.radio.datagram.recv();
    ManagedString message = encryption(raw_message);
    if (isCameFromMaster(message) && isItForMe(message)) {
        ManagedString setting = getMessage(message);
        if (setting.length() == 3) {
            DISPLAY_SETTING = setting;
            uBit.display.scroll(DISPLAY_SETTING, SCROLL_SPEED);
        }
        else {
            uBit.display.print("E");
            uBit.sleep(100);
            uBit.display.clear();
            uBit.display.scroll(setting, SCROLL_SPEED);
        }
    }
}

void displayOn(ManagedString temperature, ManagedString luminosity, ManagedString humidity) {
    for (int i = 0; i < DISPLAY_SETTING.length(); i++) {
        char c = DISPLAY_SETTING.charAt(i);
        switch (c) {
            case 'T':
                temperature = temperature;
                // display Temperature on OLED screen
                break;
            case 'L':
                luminosity = luminosity;
                // display Luminosity on OLED screen
                break;
            case 'H':
                humidity = humidity;
                // display Humidity on OLED screen
                break;
            default:
                break;
        }
    }
}

void testFunctionIot() {
    if (uBit.buttonA.isPressed()) {
        sendRadioData("26.73", "320", "44.73");
    }
    else if (uBit.buttonB.isPressed()) {
        sendRadioData("27.92", "1197", "40.92");
    }
}

void iot() {
    // Initialise the micro:bit runtime.
    initRadio(onIotData, false);

    bme280 bme(&uBit,&i2c);
    uint32_t pressure = 0;
    int32_t temp = 0;
    uint16_t humidite = 0;

    veml6070 veml(&uBit,&i2c);
    uint16_t uv = 0;

    tsl256x tsl(&uBit,&i2c);
    uint16_t comb =0;
    uint16_t ir = 0;
    uint32_t lux = 0;

    while(true)
    {
        bme.sensor_read(&pressure, &temp, &humidite);
        int tmp = bme.compensate_temperature(temp);
        int pres = bme.compensate_pressure(pressure)/100;
        int hum = bme.compensate_humidity(humidite);

        veml.sensor_read(&uv);

        tsl.sensor_read(&comb, &ir, &lux);

        // Affichage des valeurs
        ManagedString msg = ManagedString("{id: ") + ManagedString(1) + 
        ManagedString(", temperature: ") + ManagedString(tmp/100) + "." + (tmp > 0 ? ManagedString(tmp%100): ManagedString((-tmp)%100)) + 
        ManagedString(", humidity: ") +  ManagedString(hum/100) + "." + ManagedString(tmp%100) +
        ManagedString(", pressure: ") + ManagedString(pres)+
        ManagedString(", UV: ") + ManagedString(uv) + 
        ManagedString(", lux: ") + ManagedString((int)lux) + 
        ManagedString("}\r\n");

        testFunctionIot(); // Test function to send data to the master

        // Affichage et envoi des données
        ManagedString temperatureToDisplay = ManagedString(tmp/100) + "." + (tmp > 0 ? ManagedString(tmp%100): ManagedString((-tmp)%100));
        ManagedString luminosityToDisplay = ManagedString((int)lux);
        ManagedString humidityToDisplay = ManagedString(hum/100) + "." + ManagedString(tmp%100);

        displayOn(temperatureToDisplay, luminosityToDisplay, humidityToDisplay); // Afficher les données sur l'écran OLED
        sendRadioData(temperatureToDisplay, luminosityToDisplay, humidityToDisplay); // Envoyer les données à la microbit master
        uBit.sleep(10000); // Attendre 1 minute avant de lire à nouveau
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