/*
  Sparkfun_Qwiic_EEProm.h - A library for Sparkfun Qwiic EEProm
  Copyright (c) 2018 by Gaston Williams

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

#ifndef SPARKFUN_QWIIC_EEPROM_H
#define SPARKFUN_QWIIC_EEPROM_H

#include <inttypes.h>

#include "Stream.h"

#include "Wire.h"

// Define contstants
#define EOF		 	(-1)
#define SIZE_ADDR   0x04
#define DATA_ADDR   0x08

//#define MAX_I2C_WRITE 16
#define MAX_DATA_SIZE 0x010000

//constants for seek cursor origin
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/*This address is determined by the way your address pins are wired.
In the diagram from earlier, we connected A0 and A1 to Ground and
A2 to 5V. To get the address, we start with the control code from
the datasheet (1010) and add the logic state for each address pin
in the order A2, A1, A0 (100) which gives us 0b1010100, or in
Hexadecimal, 0x54*/

#define EEPROM_ADDR 0x50

class QwiicEEProm : public Stream
{
  //protected:
    // Has any byte been written since begin or flush
  //  bool _written;
  private:
    uint8_t _i2cAddr = EEPROM_ADDR;
    // uint8_t buffer[MAX_I2C_WRITE];
    long _cursorPos = 0;
    long _eofPos = 0;
    void putByte(long eeAddress, uint8_t b);
    uint8_t getByte(long eeaddress);
    bool verifyHeader(void);
    long getDataSize(void);
    void putDataSize(long dataSize);
    void initializeHeader(void);
    bool open(void);
    void close(void);

  public:
  	QwiicEEProm();
	~QwiicEEProm();
    bool begin() { return begin(EEPROM_ADDR); }
    bool begin(uint8_t);
    void end();
    void setDataSize(void);
    bool check(void);
    long count(void);
    bool full(void);
    long freespace(void);
    long size(void);
    long tell(void);
    bool validPos(long pos);
    bool setPos(long pos);
    void rewind(void);
    void seek(void);
    bool seek(long offset, int origin);
    bool eof(void);
    void truncate(void);
    void remove(void);
    size_t readln(char *buffer, size_t max);
    size_t appendln(char *str);
    size_t appendln(const __FlashStringHelper *);
    size_t appendln(void);
    size_t append(uint8_t);
    size_t append(uint8_t *buffer, size_t size);
    size_t append(char *buffer, size_t size);
    size_t append(char *str);
    size_t append(const __FlashStringHelper *);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int availableForWrite(void);
    // virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
};


#endif
