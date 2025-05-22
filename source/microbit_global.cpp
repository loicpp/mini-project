#include "microbit_global.h"
#include "MicroBit.h"

MicroBit uBit;

int splitManagedString(ManagedString &input, ManagedString output[]) {
    int partCount = 0;
    int start = 0;
    int len = input.length();

    for (int i = 0; i <= len; i++) {
        if (i == len || input.charAt(i) == '-') {
            if (partCount < MAX_SIZE) {
                output[partCount] = input.substring(start, i - start);
                partCount++;
            }
            start = i + 1;
        }
    }

    return partCount;
}