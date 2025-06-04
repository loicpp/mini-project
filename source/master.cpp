#include "MicroBit.h"
#include "master.h"
#include "radio.h"
#include "microbit_global.h"

void sendSerial(ManagedString s) {
    uBit.serial.send(s);
    uBit.serial.send("\n");
}

void onMasterData(MicroBitEvent) {
    ManagedString raw_message = uBit.radio.datagram.recv();
    uBit.display.print("R");
    uBit.sleep(100);
    uBit.display.clear();
    ManagedString message = encryption(raw_message);
    if (isItForMe(message)) {
        ManagedString sender = getSender(message);
        sendSerial(message);
        ManagedString response = uBit.serial.readUntil('\n', ASYNC);
        if (response.length() == 3) {
            sendRadio(sender, response);
            uBit.display.scroll(response, SCROLL_SPEED);
            uBit.sleep(100);
            uBit.display.clear();
        }
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

    uBit.serial.baud(115200);
    ManagedString str = "Init";
    sendSerial(str);
    
    while (1) {
        testFunctionMaster();
        uBit.sleep(1000);
    }
}