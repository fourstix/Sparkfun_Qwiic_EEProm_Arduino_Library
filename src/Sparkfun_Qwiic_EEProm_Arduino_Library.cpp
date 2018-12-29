/*
  Sparkfun_Qwiic_EEProm.h - A library for Sparkfun Qwiic EEProm
  Copyright (c) 2018 by Gaston Williams.

  This is a library written for the Qwiic EEProm
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14764

  This code is public domain under the MIT License, but please
  buy me a beer if you use this and we meet someday (Beerware).

  Created by Gaston Williams
  Date: December 28, 2018

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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <util/atomic.h>
#include "Arduino.h"

#include "Sparkfun_Qwiic_EEProm_Arduino_Library.h"


//<<constructor>> setup using defaults
QwiicEEProm::QwiicEEProm(){
	//We never want this class to block, since EOF indicates no more data in EEProm
	setTimeout(0);
}

//<<destructor>>
QwiicEEProm::~QwiicEEProm(){/*nothing to destruct*/}

// Private Methods //////////////////////////////////////////////////////////////


/* This is the 3 step memory writing procedure that
we talked about. First we send the MSB of the address.
Then we send the LSB of the address. Then we send the
data that we want to store. */

void QwiicEEProm::putByte(long eeAddress, uint8_t b)
{

  Wire.beginTransmission(_i2cAddr);

  Wire.write((int)(eeAddress >> 8)); // MSB
  Wire.write((int)(eeAddress & 0xFF)); // LSB
  Wire.write(b); //Write the data

  Wire.endTransmission(); //Send stop condition
} // putByte

/* This is the 3 step memory reading procedure that
we talked about. First we send the MSB of the address.
Then we send the LSB of the address. Then we ask for the
number of bytes that we want to receive. Here, we're
going 1 byte at a time*/

uint8_t QwiicEEProm::getByte(long eeaddress)
{
  Wire.beginTransmission(_i2cAddr);

  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(_i2cAddr, (uint8_t) 1);

  uint8_t rdata = 0xFF;
  if (Wire.available()) rdata = Wire.read();
  return rdata;
} // getByte

bool QwiicEEProm::verifyHeader() {
   bool valid = false;

   // Read the byte signature and veify it spells FACE
   uint8_t f = getByte(0x00);
   uint8_t a = getByte(0x01);
   uint8_t c = getByte(0x02);
   uint8_t e = getByte(0x03);

  valid = (f == 0x0F) && (a == 0x0A) && (c == 0x0C) && (e == 0x0E);
  return valid;
} // verifyHeader

// get the size of the data stored in EEProm
long QwiicEEProm::getDataSize() {
  long dataSize = 0;
  uint8_t b = 0;

  for (int i=0; i<4; i++) {
    b = getByte(SIZE_ADDR + i);

    dataSize = dataSize << 8;  // move one byte, clearing LSB with zeros
    dataSize = dataSize | b;  // set byte with data
  } // for
return dataSize;
}  // getDataSize

// write the size of data in EEProm to the header
void QwiicEEProm::putDataSize(long dataSize) {
  uint8_t b = 0;
  //write data size backwards, LSB down to MSB
  for (int i=3; i> -1; i--) {
    b = (uint8_t) (dataSize & 0x000000FF);  // mask and truncate

    putByte(SIZE_ADDR + i, b);
    dataSize = dataSize >> 8;  // move one byte, over-writing LSB with next byte
  } //for
} // putDataSize

// set the data size in the header
void QwiicEEProm::setDataSize(void) {
  putDataSize(_eofPos - DATA_ADDR);
}

// initialize the header, if not present
void QwiicEEProm::initializeHeader() {
  long headerAddr = 0;

  //Write FACE followed by the size
  putByte(headerAddr++, 0x0F);
  putByte(headerAddr++, 0x0A);
  putByte(headerAddr++, 0x0C);
  putByte(headerAddr++, 0x0E);

  putDataSize(0);
  } //initializeHeader

// prepare the EEProm for data access
bool QwiicEEProm::open(void) {
  bool valid = check(); // validate the EEProm exists

  if(valid) {
    if (verifyHeader()) {
      _cursorPos = DATA_ADDR;
      _eofPos = getDataSize() + DATA_ADDR;
      setDataSize(); // this needs to be here, for print/write to work every time

      /* Caution! Here be dragons.  If the header is valid, the print functions
         will fail if called immediately after begin().  The pointers _eofPos and
         _cursorPos have the correct values, so I -suspect- that the Wire library
         or EEProm buffer is blocking.  For some reason doing a direct write after
         initialization clears the problem.  But I have not debugged it enough to
         understand why. So that is why setDataSize() is done in the line above.
       */
    } else {
      initializeHeader();
      _cursorPos = DATA_ADDR;
      _eofPos = _cursorPos;
    } //if-else verifyHeader

  } // if valid
  return valid;
} // open

// prepare EEProm to end
void QwiicEEProm::close(void) {
    setDataSize();
} // close

// Public Methods //////////////////////////////////////////////////////////////

bool QwiicEEProm::begin(uint8_t addr)
{
  //Start the I2C Library
  Wire.begin();
  Wire.setClock(400000);
  _i2cAddr = addr;
  return open();
} // begin

// check to see if EEProm is available
bool QwiicEEProm::check(void) {
  Wire.beginTransmission(_i2cAddr);
  return (Wire.endTransmission() == 0);
} //check

/* close wire here */
void QwiicEEProm::end()
{
  close();
  Wire.end();
}

// check to see if cursor is at the end of the file
bool QwiicEEProm::eof(void) {
  return (_cursorPos == _eofPos);
} // eof

// check to see if data has filled the EEProm (used before writing more)
bool QwiicEEProm::full(void) {
  //if we reach the end of the EEProm, it is full
  return (_cursorPos == MAX_DATA_SIZE);
} // full

// return count of bytes from cursor until end of data
long QwiicEEProm::count(void) {
  return (_eofPos - _cursorPos);
} // count

// space remaining after data
long QwiicEEProm::freespace(void) {
  return (MAX_DATA_SIZE - _eofPos);
} // freespace

// size of data in the EEprom
long QwiicEEProm::size(void) {
  return (_eofPos - DATA_ADDR);
} // size

//current cursor location within data
long QwiicEEProm::tell(void) {
  return (_cursorPos - DATA_ADDR);
} // tell

// true if position located within data stream (from beginning to eof)
bool QwiicEEProm::validPos(long pos) {
	return (pos >= 0) && (pos <= (_eofPos - DATA_ADDR));
}

// set the cursor to a new position (EOF for end of file)
bool QwiicEEProm::setPos(long pos) {
  bool result = false;
  // EOF is allowed to change to end of file
  if (pos == EOF) {
    _cursorPos = _eofPos;
    result = true;
  } else {
	// only change if position in valid range
	if (validPos(pos)) {
      _cursorPos = (pos + DATA_ADDR);
      result = true;
    } // if validPos
  } // if-else
  return result;
} // setPos

// move the cursor n characters forwards (n > 0) or backwards (n < 0)
// based on the origin:
//
// SEEK_SET => seek from the beginning (n should be positive)
// SEEK_CUR => from the current cursor position
// SEEK_END => from end of file (n should be negative)

bool QwiicEEProm::seek(long n, int origin) {
	bool result = false;
	if (origin == SEEK_SET) {
		// cannot seek before beginning of file, so n should be positive
		// need to check here, since -1 is also the EOF value allowed in setPos()
		if (n >= 0) {
		  result = setPos(n);
	    } // if
	} else if (origin == SEEK_CUR) {
		long newPos = tell() + n;
		result = setPos(newPos);
	} else if (origin == SEEK_END) {
		long newPos = getDataSize() + n; //n should be negative
		result = setPos(newPos);
	}
	return result;
} // seek

// set the cursor to the begining of data, equivalent to seek(0, SEEK_SET)
void QwiicEEProm::rewind(void) {
  _cursorPos = DATA_ADDR;
} // rewind

// set the cursor after the end of the data, equivalent to seek(0, SEEK_END)
void QwiicEEProm::seek(void) {
  _cursorPos = _eofPos;
} // seek

// truncate the data at the cursor position
void QwiicEEProm::truncate() {
  _eofPos = _cursorPos;
  setDataSize();
} // truncate

// reset the EEProm back to the empty state to remove all previous data
void QwiicEEProm::remove(void) {
  _cursorPos = DATA_ADDR;
  _eofPos = _cursorPos;
  setDataSize();
} // remove

// append byte
size_t QwiicEEProm::append(uint8_t b) {
	seek();
	return write(b);
} // append uint8_t

// append byte array
size_t QwiicEEProm::append(uint8_t *buffer, size_t size) {
	seek();
	return write(buffer, size);
} // append byte array

//append characters
size_t QwiicEEProm::append(char *buffer, size_t size) {
	seek();
	return write(buffer, size);
} // append char array

//append string

//append string
size_t QwiicEEProm::append(char *str) {
	seek();
	return print(str);
} // append string

//append flash string
size_t QwiicEEProm::append(const __FlashStringHelper *ifsh) {
	seek();
	return print(ifsh);
} // append string

//append string
size_t QwiicEEProm::appendln(char *str) {
	seek();
	return println(str);
} // append string

//append string
size_t QwiicEEProm::appendln() {
	seek();
	return println();
} // append string

//append flash string
size_t QwiicEEProm::appendln(const __FlashStringHelper *ifsh) {
	seek();
	return println(ifsh);
} // append string


// read upto max-1 bytes into a null-terminated string
// read terminates with \n, EOF or max characters read.
// The newline is not included in the string.
size_t QwiicEEProm::readln(char *buffer, size_t max) {
	size_t size = readBytesUntil('\n', buffer, max-1);
	//check to see if the last character is carriage return, if so, remove it
	if ((size > 0) && (buffer[size-1] == '\r')) {
		size--;
	}
	//always terminate with a NULL
	buffer[size] = '\0';
	return size;
} // readLine

/* -- methods required by Stream and Print classes -- */
int QwiicEEProm::available(void)
{
  return eof() ? 0 : 1;
} // available

int QwiicEEProm::availableForWrite(void)
{
  return full() ? 0 : 1;
} // availableForWrite

int QwiicEEProm::peek(void)
{
if (eof()) return EOF;
  else return getByte(_cursorPos);
} // peek

int QwiicEEProm::read(void)
{
  // if the cursor is at the end of file, we don't have any characters
  char c = EOF;
  if (available() > 0) {
    c = getByte(_cursorPos);
    _cursorPos++;
  } // if available
  return c;
} // read

// write byte to the EEProm
size_t QwiicEEProm::write(uint8_t b)
{
  if (!full()) {
    putByte(_cursorPos, b);
    _cursorPos++;
    //if we wrote past the previous end of file, adjust the pointer
    if (_cursorPos > _eofPos) {
      _eofPos = _cursorPos;
      setDataSize();
    } // if
  } // if !full
} // write

