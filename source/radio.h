#ifndef RADIO_H
#define RADIO_H

bool isCameFromMaster(ManagedString s);

ManagedString getSender(ManagedString s);

bool isItForMe(ManagedString s);

ManagedString getMessage(ManagedString s);

ManagedString initRadio(void (*onData)(MicroBitEvent), bool isMaster = false);

void sendRadio(ManagedString receiver, ManagedString message);

void sendRadioData(ManagedString temperature, ManagedString luminosity, ManagedString humidity);

#endif