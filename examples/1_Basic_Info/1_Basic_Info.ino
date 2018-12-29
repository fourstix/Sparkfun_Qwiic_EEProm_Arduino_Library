/*
  A Qwiic I2C based EEProm Library
  By: Gaston Williams
  Date: December 28th, 2018
  
  Copyright (c) 2018 by Gaston Williams
  
  This code is public domain under the MIT License, but please buy me a beer
  if you use this and we meet someday (Beerware).

  This example shows how to begin communication with the  Qwiic EEProm,
  and how to querry some basic information like data size, etc.

  To Use:
    Attach Qwiic EEProm to a RedBoard, BlackBoard or Uno with a Qwiic cable
    Load this sketch onto the Arduino
    Open a terminal window to see the Serial.print statements
    
  Disclaimers:
    References to Sparkfun products, programs or services do not imply
    that they will be available in all countries in which Sparkfun operates.
  
    Sparkfun, the Sparkfun logo, and other Sparkfun products and services are
    trademarks of the Sparkfun Electronics Corporation, in the United States,
    other countries or both. Other company, product, or services names may be
    trademarks or services marks of others.
    
  License:
    This code, firmware, and software is released under the MIT License
    (http://opensource.org/licenses/MIT).

    The MIT License (MIT)

    Copyright (c) 2018 by Gaston Williams

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.    
      
*/
#include <Sparkfun_Qwiic_EEProm_Arduino_Library.h>

QwiicEEProm eeprom;


void setup() {
  Serial.begin(9600);

  // Start communication with the EEprom with default I2C address 
  // Begin will initialize the EEProm with a header if necessary
  
  if (eeprom.begin()) {
    Serial.println("Sparkfun Qwiic EEProm is ready!");
    // print some information about the EEProm data
    Serial.print("Data Size: ");
    Serial.println(eeprom.size());
    Serial.print("Cursor Position: ");
    Serial.println(eeprom.tell());
    if (eeprom.eof()) {
      Serial.println("(End of data)");
    }
    Serial.print("Available EEProm free capacity: ");
    Serial.println(eeprom.freespace());
  } else {
    Serial.println("EEProm is not responding.");
  } // if-else
}

void loop() {
  // uncomment the following lines to check repeatedly:
  
//  if (eeprom.check()) {
//    Serial.println("Sparkfun Qwiic EEProm is still ok!");
//  } else {
//    Serial.println("EEProm is not responding.");
//  } // if-else  
//  delay(5000); // wait 5 seconds and check again  
}
