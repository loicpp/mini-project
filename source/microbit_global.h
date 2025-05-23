#pragma once
#include "MicroBit.h"

#define MAX_SIZE 10
#define SCROLL_SPEED 50

int splitManagedString(ManagedString &input, ManagedString output[]);

extern MicroBit uBit;