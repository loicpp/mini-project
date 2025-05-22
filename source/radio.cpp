#include "MicroBit.h"
#include "radio.h"
#include "microbit_global.h"

static ManagedString CARD_ID = "";
static ManagedString MASTER_ID = "M1";
static bool isMaster = false;

bool isMasterCard() {
    return isMaster;
}

ManagedString initRadio(void (*onData)(MicroBitEvent), bool isMaster) {
    if (CARD_ID == "") {
        if (isMaster) {
            CARD_ID = "M1";
        } else {
            CARD_ID = "I1";
        }
    }
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    uBit.radio.enable();

    return CARD_ID;
}

void sendData(ManagedString receiver, ManagedString message) {
    if (CARD_ID == "") {
        uBit.display.print("E");
        ManagedString str = "Error: Card number not set, you have to call initRadio() before";
        uBit.radio.datagram.send(str);
        uBit.sleep(1000);
        uBit.display.clear();
    }
    else {
        ManagedString display = ManagedString(CARD_ID) + "-" + receiver + "-" + message;
        uBit.radio.datagram.send(display);
    }
}

sendDataSensor(ManagedString temperature, ManagedString luminosity, ManagedString humidity) {
    ManagedString display = temperature + "-" + luminosity + "-" + humidity;
    sendData(MASTER_ID, display);
}