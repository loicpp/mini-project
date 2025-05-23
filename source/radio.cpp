#include "MicroBit.h"
#include "radio.h"
#include "microbit_global.h"

static ManagedString CARD_ID = "";
static ManagedString MASTER_ID = "M1";

bool isCameFromMaster(ManagedString s) {
    ManagedString output[MAX_SIZE];
    splitManagedString(s, output);
    return output[0].charAt(0) == 'M';
}

ManagedString getSender(ManagedString s) {
    ManagedString output[MAX_SIZE];
    splitManagedString(s, output);
    return output[0];
}

bool isItForMe(ManagedString s) {
    ManagedString output[MAX_SIZE];
    splitManagedString(s, output);
    return output[1] == CARD_ID;
}

ManagedString getMessage(ManagedString s) {
    ManagedString output[MAX_SIZE];
    splitManagedString(s, output);
    return output[2];
}

ManagedString initRadio(void (*onData)(MicroBitEvent), bool isMaster) {
    if (CARD_ID == "") {
        if (isMaster) {
            CARD_ID = "M1";
        } else {
            CARD_ID = "I1";
        }
    }
    // uBit.radio.setGroup(142);
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    uBit.radio.enable();
    uBit.display.scroll(CARD_ID, SCROLL_SPEED);
    return CARD_ID;
}

void sendRadio(ManagedString receiver, ManagedString message) {
    if (CARD_ID == "") {
        uBit.display.print("E");
        ManagedString str = "Error: Card number not set, you have to call initRadio() before";
        uBit.radio.datagram.send(str);
        uBit.sleep(1000);
        uBit.display.clear();
    }
    else {
        ManagedString display = ManagedString(CARD_ID) + "-" + receiver + "-" + message;
        int status = uBit.radio.datagram.send(display + "\n");
        if(status == MICROBIT_OK) {
            uBit.display.print("S");
        } else {
            uBit.display.print("E");
        }
        //uBit.display.print("S");
        uBit.sleep(100);
        uBit.display.clear();
    }
}

void sendRadioData(ManagedString temperature, ManagedString luminosity, ManagedString humidity) {
    ManagedString display = temperature + "-" + luminosity + "-" + humidity;
    sendRadio(MASTER_ID, display);
}