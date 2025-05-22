#include "MicroBit.h"
#include "iot.h"
#include "radio.h"
#include "microbit_global.h"
#include "bme280.h" 
#include "veml6070.h"
#include "tsl256x.h"
#include "ssd1306.h"

static ManagedString DISPLAY_SETTING = "TLH";
MicroBitI2C i2c(I2C_SDA0,I2C_SCL0);

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
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.serial.baud(115200); // Configure la vitesse de la sortie série

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

       uBit.serial.send(msg);
    }

    release_fiber();
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