/*
   The MIT License (MIT)

   Copyright (c) 2016 British Broadcasting Corporation.
   This software is provided by Lancaster University by arrangement with the BBC.

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
   */

   #include "MicroBit.h"
   #include "radio.h"
   // #include "SSD1306.h"
   #include "iot.h"
   #include "master.h"
   #include "microbit_global.h"

   // TO WORK YOU HAVE TO CHANGE VALUE IN yotta_modules/microbit-dal/inc/core/MicroBitConfig.h
   // #define MICROBIT_HEAP_ENABLED 0

   // void onData(MicroBitEvent)
   // {
   //    ManagedString s = uBit.radio.datagram.recv();

   //    if (s == "1") {
   //       ManagedString str = "A";
   //       uBit.display.print("A");
   //       uBit.serial.send(str);
   //       uBit.serial.sendChar('\n');
   //    }

   //    if (s == "2") {
   //       ManagedString str = "B";
   //       uBit.display.print("B");
   //       uBit.serial.send(str);
   //       uBit.serial.sendChar('\n');
   //    }
   // }

   // void receiver() {
   //    uBit.serial.baud(115200);
   //       ManagedString str = "Init";
   //       uBit.serial.send(str);
   //       uBit.serial.sendChar('\n');
   //       //uBit.thermometer.setCalibration(uBit.thermometer.getTemperature());

   //       uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
   //       uBit.radio.enable();
         
   //       while (1) {
   //          //uBit.display.scroll(uBit.thermometer.getTemperature());
   //          //ManagedString msg = ManagedString("T: ") + ManagedString(uBit.thermometer.getTemperature()) + ManagedString("°C\r\n");
   //          //uBit.serial.send(msg);
   //          uBit.sleep(1000);
   //       }
   // }

   void configVerifier() {
      #if MICROBIT_BLUETOOTH_ENABLED != 0
         while (true) {
            uBit.display.scroll("BLUETOOTH ERROR");
            uBit.sleep(1000);
         }
      #endif

      #if MICROBIT_RADIO_MAX_PACKET_SIZE != 35
         while (true) {
            uBit.display.scroll("PACKET SIZE ERROR");
            uBit.sleep(1000);
         }
      #endif
   }

   int choose() {
      if (uBit.buttonA.isPressed()) {
         uBit.display.print("M");
         return 1;
      }
      else if (uBit.buttonB.isPressed()) {
         uBit.display.print("I");
         return 2;
      }
      return 0;
   }
   
   int main()
   {
      // Initialise the micro:bit runtime.
      configVerifier();
      int CARDNUMBER = 0;
      uBit.init();
      uBit.display.scroll("SETUP");
      while (CARDNUMBER == 0) {
         CARDNUMBER = choose();
         uBit.sleep(100);
      }
      uBit.display.clear();
      uBit.display.scroll("INIT");

      // 1 == Receiver
      if (CARDNUMBER == 1) {
         master();
      }
      // 2 == Transmitter
      else if (CARDNUMBER == 2) {
         iot();
      }

      release_fiber();
   
   }
   
   