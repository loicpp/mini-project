#ifndef RADIO_H
#define RADIO_H

bool isMasterCard();

ManagedString initRadio(void (*onData)(MicroBitEvent), bool isMaster = false);

void sendData(ManagedString receiver, ManagedString message);

void sendDataSensor(ManagedString temperature, ManagedString luminosity, ManagedString humidity);

#endif