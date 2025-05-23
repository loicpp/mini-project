#include "MicroBit.h"
#include "master.h"
#include "radio.h"
#include "microbit_global.h"

void sendSerial(ManagedString s) {
    uBit.serial.send(s + "\n");
}

void onMasterData(MicroBitEvent) {
    ManagedString message = uBit.radio.datagram.recv();
    uBit.display.print("R");
    uBit.sleep(100);
    uBit.display.clear();
    if (isItForMe(message)) {
        ManagedString sender = getSender(message);
        sendSerial(message);
        ManagedString response = uBit.serial.readUntil('\n');
        if (response.length() > 0) {
            sendRadio(sender, response);
        }
        uBit.display.print("S");
        uBit.sleep(100);
        uBit.display.clear();
    }
}

// Function to send a message to the radio
void testFunctionMaster() {
    if (uBit.buttonA.isPressed()) {
        ManagedString message = "HTL";
        sendRadio("I1", message);
    }
    else if (uBit.buttonB.isPressed()) {
        ManagedString message = "TLH";
        sendRadio("I1",message);
    }
}

void master() {
    initRadio(onMasterData, true);
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onMasterData);
    uBit.radio.enable();

    uBit.serial.baud(115200);
    ManagedString str = "Init";
    sendSerial(str);
    
    while (1) {
        testFunctionMaster();
        uBit.sleep(1000);
    }
}