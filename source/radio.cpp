#include "MicroBit.h"
#include "radio.h"
#include "microbit_global.h"

static ManagedString CARD_ID = "";
static ManagedString MASTER_ID = "M1";
static ManagedString ENCRYPTION_KEY = "o5U4mwP34wGzSRV9Y";

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

ManagedString encryption(ManagedString &input) {
    ManagedString output = "";
    uint8_t input_len = input.length();
    uint8_t key_len = ENCRYPTION_KEY.length();

    for (uint8_t i = 0; i < input_len; i++) {
        char c = input.charAt(i);
        char k = ENCRYPTION_KEY.charAt(i % key_len);
        output = output + (char)(c ^ k);
    }

    return output;
}

ManagedString initRadio(void (*onData)(MicroBitEvent), bool isMaster) {
    if (CARD_ID == "") {
        if (isMaster) {
            CARD_ID = "M1";
        } else {
            CARD_ID = "I1";
        }
    }
    uBit.radio.setGroup(142);
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
        ManagedString encrypted = encryption(display);
        int status = uBit.radio.datagram.send(encrypted + "\n");
        if(status == MICROBIT_OK) {
            uBit.display.print("S");
        } else {
            uBit.display.print("E");
        }
        uBit.sleep(100);
        uBit.display.clear();
    }
}

void sendRadioData(ManagedString temperature, ManagedString luminosity, ManagedString humidity) {
    ManagedString display = temperature + "-" + luminosity + "-" + humidity;
    sendRadio(MASTER_ID, display);
}