SparkFun Qwiic EEProm Library
===========================================================

[*SparkX Qwiic EEProm ( SPX-14764)*](https://www.sparkfun.com/products/14764)

This library makes using the Qwiic EEProm easy. It creates a Stream class so that you can read and write
to QwiicEEProm eeprom using arrays and strings instead of raw bytes.  It supports C-like cursor functions.

For example to add Hello, World! to the Qwiic EEProm with this library all one has to do is:
    
    eeprom.begin()
    eeprom.appendln("Hello, World!);
    eeprom.end();

And your data will be recorded to the EEProm for later review. Please see the examples to get started.

Available commands:

* boolean **begin**() - Start comminication with a given Qwiic EEProm, initializing the EEProm if necessary
* boolean **end**() - Stop comminication with a given Qwiic EEProm
* boolean **check**() - Check to see of EEProm is responding
* boolean **eof**() - Check to see if cursor is at the end of data
* boolean **full**() - Check to see if EEProm is full (no room for more data)
* long    **count**() - Return the count of bytes from cursor to EOF
* long    **freespace**() - Return the amount of bytes remaining in the EEProm
* long    **size**() - Return the amount of bytes written to the EEProm
* boolean **tell**() - Tell the current position of the cursor
* boolean **validPos**(long pos) - Check to see of a position is valid (between 0 and end of data)
* boolean **setPos**(long pos) - Set the position of the cursor, returns true if cursor set.
* void    **rewind**() - Set the cursor to the begining of the EEProm data, equivalent to seek(0, SEEK_SET)
* void    **seek**() - Set cursor to the end of the EEProm data, , equivalent to seek(0, SEEK_END)
* bool    **seek**(long n, int origin) - move the cursor n characters forwards (n > 0) or backwards (n < 0)
*                       based on the origin, returns true if cursor moved. 
*	  Use one of these defined constants for origin:
* 		SEEK_SET - seek from the beginning (n should be positive or zero)
* 		SEEK_CUR - seek from the current cursor position (n can be positive or negative)
*		SEEK_END - seek from end of file (n should be negative or zero)
* void    **truncate**() - truncate data by setting end of data at the current cursor position
* void    **remove**() - remove data from EEProm by setting end of data to the beginning (zero) position
* size_t  **append**(uint8_t b) - append a byte of data at the end, returns number of bytes written
* size_t  **append**(uint8_t *buffer, size_t size) - append size bytes of data from a buffer, returns number of bytes written
* size_t  **append**(char *buffer, size_t size) - append size characters of data from a buffer, returns number of characters written
* size_t  **append**(char *str)- append a string of characters, returns number of characters written
* size_t  **appendln**(char *str)- append a string of characters with a newline, returns number of characters written
* void    **readln**(char *buffer, size_t max) - read up to max-1 charaters into a null-terminated string.
*               The read terminates with \n, EOF or max characters read, and the newline is not included in the string.
* The following virtual functions are defined as follows:
* int     **peek**() - returns the next character at the cursor, without advancing the cursor or EOF if at the end of data
* int     **read**() - returns the character at the cursor and advances the cursor. returns EOF if at the end of data
* size_t  ** write**(uint8_t) - writes one byte of data, advancing the cursor
* int     **available**() - returns 1 to indicate one data byte can be read, unless eof() is true
* int     **availableForWrite**() - returns 1 to indicate one data byte can be written, unless full() is true
*
* Note: The append and appendln functions support the flash string macro F("My String"). 
*	All of the functions available in Stream and Print (read, write, println, find, etc.) are supported.
*       Characters are streamed one by one to the EEProm

	
Library written by Gaston Williams).

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation
--------------

* **[Installing an Arduino Library Guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)** - Basic information on how to install an Arduino library.
* **[Product Repository](https://github.com/tbd/tbd)** - Main repository (including hardware files)

License Information
-------------------

This product is _**open source**_! 

Various bits of the code have different licenses applied. Anything SparkFun wrote is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release any derivative under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
- Gaston Williams
