/*
  A Qwiic I2C based EEProm
  By: Gaston Williams
  Date: December 28th, 2018
   
  Copyright (c) 2018 by Gaston Williams
   
  This code is public domain under the MIT License, but please buy me a beer
  if you use this and we meet someday (Beerware).

  This example puts some test data in the EEProm, truncates it, and moves the cursor around,
  It demonstrates truncate, peek and the cursor position functions like seek, rewind, setPos and tell.

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
    // Notice the cursor is at the beginning of the data
    Serial.print(F("Cursor Position: "));
    Serial.println(eeprom.tell());
    // Write a long bunch of ASCII characters
    for (char c = '@'; c <= '~'; c++) {
      eeprom.write(c);
    }
    // show it
    printData();
    //Now rewind and print a line at the beginnning
    showCursorPosition();
    Serial.println(F("Rewind"));
    eeprom.rewind(); // equivalent to seek(0, SEEK_END)
    showCursorPosition();
    showCharacterAtCursor(); //peek at the current character
    eeprom.println("This is an apple.");
    showCursorPosition();
    showCharacterAtCursor();
    showDataSize();
    // show it
    printData();
    Serial.println(F("Truncate"));
    showCursorPosition();
    eeprom.truncate();
    showDataSize();
    // show it
    printData();
    showCursorPosition();
   //Go to 9 characters from beginning of data
    eeprom.seek(9, SEEK_SET);
    showCursorPosition();
    showCharacterAtCursor();    
    //Print ' sam' at cursor
    eeprom.print(" sam");
    showCursorPosition();
    showCharacterAtCursor();    
    printData();
    //Go back 4 characters from current position
    showCursorPosition();
    showCharacterAtCursor();    
    eeprom.seek(-4, SEEK_CUR);
    showCursorPosition();
    showCharacterAtCursor();    
    //Print 'n ap' at cursor
    eeprom.print("n ap");
    showCursorPosition();
    showCharacterAtCursor();    
    printData();
    //Move cursor to the end of data
    eeprom.seek();  // equivalent to seek(0, SEEK_END);
    showCursorPosition();
    showCharacterAtCursor();     
  } // if-else
}

void loop() {
 if (confirmChoice()) {
  printData();
 }
 delay(5000); // ask every 5 seconds
} // loop

// Print all the data, but put the cursor back to where it was
void printData() {
  char dataLine[40]; // limit lines to 39 characters + NULL
  long savePos = eeprom.tell();  // save the cursor position
  
  Serial.println("-----EEProm Data------");
  //Go back to beginning of data and show it line by line
  eeprom.rewind();
  while(eeprom.available()) {
    eeprom.readln(dataLine, 40);
    Serial.println(dataLine);    
  }
  Serial.println("-----EEProm Data ------");
  eeprom.setPos(savePos); // restore the cursor to previous location 
}

//peek at the character without moving cursor
void showCharacterAtCursor() {
  int c = eeprom.peek();
  Serial.print("The cursor points to ");
  if (c == EOF) {
    Serial.println("the end of data.");
  } else {
    Serial.print("the character '");
    // cast it to char so it prints as character, not number
    Serial.print((char)c); 
    Serial.println("'.");
  } // if-else
} //showCharacterAtCursor

// show the current cursor value
void showCursorPosition() {
  Serial.print("Cursor Position: ");
  Serial.println(eeprom.tell());
}

// show the size of the data
void showDataSize() {
  Serial.print("Data Size: ");
  Serial.println(eeprom.size());
  if (eeprom.eof()) {
    Serial.println("(End of File)");
  } // if eof
}

// confirm the user's choice printing EEProm
boolean confirmChoice() {
    bool result = false;
    Serial.println("Would you like to print the eeprom data (y/n)?");
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
    Serial.println("Nevermind.");
  }  
  return result;
}  // confirmChoice
