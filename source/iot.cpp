#include "MicroBit.h"
#include "iot.h"
#include "radio.h"
#include "microbit_global.h"
#include "bme280.h" 
#include "veml6070.h"
#include "tsl256x.h"
#include "ssd1306.h"
// #include <string>

static ManagedString DISPLAY_SETTING = "TLH"; // Ordre d'affichage sur l'écran OLED (par défaut: : Temperature, Luminosity, Humidity)
MicroBitI2C i2c(I2C_SDA0,I2C_SCL0);
// MicroBitPin P8(MICROBIT_ID_IO_P8, MICROBIT_PIN_P8, PIN_CAPABILITY_DIGITAL_OUT);
// ssd1306 screen(&uBit, &i2c, &P8);

void onIotData(MicroBitEvent)
{
    ManagedString s = uBit.radio.datagram.recv();
    
    if (isCameFromMaster(s) && isItForMe(s)) {
        ManagedString message = getMessage(s);
        if (message.length() == 3) {
            DISPLAY_SETTING = message;
            uBit.display.scroll(DISPLAY_SETTING, SCROLL_SPEED);
        }
        else {
            uBit.display.print("E");
            uBit.sleep(100);
            uBit.display.clear();
            uBit.display.scroll(message, SCROLL_SPEED);
        }
    }
}

// void displayOn(ManagedString temperature, ManagedString luminosity, ManagedString humidity) {
//     for (int i = 0; i < DISPLAY_SETTING.length(); i++) {
//         char c = DISPLAY_SETTING.charAt(i);
//         switch (c) {
//             case 'T':
//                 temperature = temperature;
//                 // display Temperature on OLED screen
//                 break;
//             case 'L':
//                 luminosity = luminosity;
//                 // display Luminosity on OLED screen
//                 break;
//             case 'H':
//                 humidity = humidity;
//                 // display Humidity on OLED screen
//                 break;
//             default:
//                 break;
//         }
//     }
// }

void testFunctionIot() {
    if (uBit.buttonA.isPressed()) {
        sendRadioData("26.73", "320", "44.73");
    }
    else if (uBit.buttonB.isPressed()) {
        sendRadioData("27.92", "1197", "40.92");
    }
}

// // Fonction ordre d'affichage sur l'écran
// void displayOrder(ssd1306 &screen, int tmp, int hum, int pres, int uv, uint32_t lux)
// {
//     screen.update_screen();
//     screen.display_line(1, 0, (ManagedString("tmp : ") + ManagedString(tmp/100) + "." + (tmp > 0 ? ManagedString(tmp%100): ManagedString((-tmp)%100))).toCharArray());
//     screen.display_line(2, 0, (ManagedString("hum : ") + ManagedString(hum/100) + "." + ManagedString(tmp%100)).toCharArray());
//     screen.display_line(3, 0, (ManagedString("pres : ") + ManagedString(pres)).toCharArray());
//     screen.display_line(4, 0, (ManagedString("uv : ") + ManagedString(uv)).toCharArray());
//     screen.display_line(5, 0, (ManagedString("lum : ") + ManagedString((int)lux)).toCharArray());
// }

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

        // displayOn(temperatureToDisplay, luminosityToDisplay, humidityToDisplay); // Afficher les données sur l'écran OLED
        sendRadioData(temperatureToDisplay, luminosityToDisplay, humidityToDisplay); // Envoyer les données à la microbit master
        uBit.sleep(10000); // Attendre 1 minute avant de lire à nouveau
        // displayOrder(screen, tmp, hum, pres, uv, lux);
    }
}

// // Fonction principale
// void iot()
// {
//     // Initialise the micro:bit runtime.
//     uBit.init();
//     uBit.serial.baud(115200); // Configure la vitesse de la sortie série

//     bme280 bme(&uBit,&i2c);
//     uint32_t pressure = 0;
//     int32_t temp = 0;
//     uint16_t humidite = 0;

//     veml6070 veml(&uBit,&i2c);
//     uint16_t uv = 0;

//     tsl256x tsl(&uBit,&i2c);
//     uint16_t comb =0;
//     uint16_t ir = 0;
//     uint32_t lux = 0;


//         while(true)
//     {
//         bme.sensor_read(&pressure, &temp, &humidite);
//         int tmp = bme.compensate_temperature(temp);
//         int pres = bme.compensate_pressure(pressure)/100;
//         int hum = bme.compensate_humidity(humidite);

//         veml.sensor_read(&uv);

//         tsl.sensor_read(&comb, &ir, &lux);

//         // Affichage des valeurs
//         ManagedString msg = ManagedString("{id: ") + ManagedString(1) + 
//         ManagedString(", temperature: ") + ManagedString(tmp/100) + "." + (tmp > 0 ? ManagedString(tmp%100): ManagedString((-tmp)%100)) + 
//         ManagedString(", humidity: ") +  ManagedString(hum/100) + "." + ManagedString(tmp%100) +
//         ManagedString(", pressure: ") + ManagedString(pres)+
//         ManagedString(", UV: ") + ManagedString(uv) + 
//         ManagedString(", lux: ") + ManagedString((int)lux) + 
//         ManagedString("}\r\n");

//         // displayOrder(screen, tmp, hum, pres, uv, lux);

//        uBit.serial.send(msg);
//        uBit.sleep(500);
//     }

//     release_fiber();
// }

