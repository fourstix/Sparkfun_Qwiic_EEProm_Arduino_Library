/*
  A Qwiic I2C based EEProm Library
  By: Gaston Williams
  Date: December 28th, 2018
    
  Copyright (c) 2018 by Gaston Williams
      
  This code is public domain under the MIT License, but please buy me a beer
  if you use this and we meet someday (Beerware).

  This example adds a few lines from A.E. Houseman's A Shropshire Lad poem XIII to the Qwiic EEProm.
  It then asks the user if it should print the EEprom data contents.  If you run it immediately after
  Example 2, the Qwiic EEProm will contain the entire first verse of Poem XIII from A Shropshire Lad.
  
  It demonstrates append and readln functions of the library.

  To Use:
    Attach Qwiic EEProm to a RedBoard or Uno with a Qwiic cable
    Load this sketch onto the RedBoard
    Open a terminal window to see the Serial.print statements
    Type y into the serial terminal to see the EEProm data printed
    
  Disclaimers:
    References to Sparkfun products, programs or services do not imply
    that they will be available in all countries in which Sparkfun operates.
  
    Sparkfun, the Sparkfun logo, and other Sparkfun products and services are
    trademarks of the Sparkfun Electronics Corporation, in the United States,
    other countries or both. Other company, product, or services names may be
    trademarks or services marks of others.
    
    A Shropshire Lad by A.E. Houseman was published in 1896, and is available to the public domain
    through project Gutenberg.  http://www.gutenberg.org/ebooks/5720
    
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
  
  if (eeprom.begin()) {
    Serial.println(F("Sparkfun Qwiic EEProm is ready!"));
    // print some information about the EEProm data
    // Notice the cursor is at the beginning of the data.
    Serial.print(F("Cursor Position: "));
    Serial.println(eeprom.tell());
    // The append functions always add data to the end.
    //Append functions work like print, but always append at the end
    eeprom.appendln("Give pearls away and rubies");
    eeprom.append("But keep your fancy ");
    eeprom.appendln("free.'");
    // You can use the flash string macro to save program space
    eeprom.append(F("But I was one-and-twenty,"));
    eeprom.appendln(); // end the line
    eeprom.appendln(F("No use to talk to me."));

    

    //Show how much data we wrote
    Serial.print(eeprom.size());
    Serial.println(" characters written to EEProm.");
  } // if-else
}

void loop() {
 if (confirmChoice()) {
  char dataLine[40]; // limit lines to 39 characters + NULL
  
  Serial.println("------------------");
  //Go back to beginning of data and show it line by line
  eeprom.rewind();
  while(eeprom.available()) {
    //readln function reads up to n-1 characters or newline with terminating null
    eeprom.readln(dataLine, 40); 
    Serial.println(dataLine);    
  }
   Serial.println("------------------");
 }
 delay(5000);
} // loop


// confirm the user's choice printing EEProm
boolean confirmChoice() {
    bool result = false;
    Serial.println(F("Would you like to print the eeprom data (y/n)?"));
    while (true) {
      while (! Serial.available());  // wait for characters
      char c = Serial.read();  // read a character 
      if (isAlpha(c)) { 
          if (c == 'y' || c == 'Y') {
            result = true;
          } // if c == y
          break;  // break out of reading loop
      }  // if isAlpha
    } // while reading characters
  if (!result) {
    Serial.println(F("Nevermind."));
  }  
  return result;
}  // confirmChoice
