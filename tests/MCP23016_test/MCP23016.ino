/*  MCP23016 library
    Based on MCP23017 library for arduino by David Pye <davidmpye@gmail.com>
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
 
#include "MCP23016.h"
//#include "mbed.h"
 
union {
    uint8_t  value8[2];
    uint16_t value16;
} tmp_data;
 
/*-----------------------------------------------------------------------------
 *
 */
00029 MCP23016::MCP23016(PinName sda, PinName scl, int i2cAddress)  : _i2c(sda, scl) {
    MCP23016_i2cAddress = I2C_BASE_ADDRESS + i2cAddress;
    reset();                                  // initialise chip to power-on condition
}
 
/*-----------------------------------------------------------------------------
 * reset
 * Set configuration (IOCON) and direction(IODIR) registers to initial state
 */
00038 void MCP23016::reset() {
//
// set direction registers to inputs
//
    writeRegister(IODIR0, (unsigned short)0xFFFF);
//
// set all other registers to zero (last of 10 registers is OLAT)
//
    for (int reg_addr = OLAT0 ; reg_addr <= IPOL1 ; reg_addr+=2) {
        writeRegister(reg_addr, (unsigned short)0x0000);
    }
//
// Set the shadow registers to power-on state
//
    shadow_IODIR = 0xFFFF;
    shadow_GPIO  = 0;
    shadow_IPOL  = 0;
}
 
/*-----------------------------------------------------------------------------
 * write_bit
 * Write a 1/0 to a single bit of the 16-bit port
 */
00061 void MCP23016::write_bit(int value, int bit_number) {
    if (value == 0) {
        shadow_GPIO &= ~(1 << bit_number);
    } else {
        shadow_GPIO |= 1 << bit_number;
    }
    writeRegister(OLAT0, (unsigned short)shadow_GPIO);
}
 
/*-----------------------------------------------------------------------------
 * Write a combination of bits to the 16-bit port
 */
00073 void MCP23016::write_mask(unsigned short data, unsigned short mask) {
    shadow_GPIO = (shadow_GPIO & ~mask) | data;
    writeRegister(OLAT0, (unsigned short)shadow_GPIO);
}
 
/*-----------------------------------------------------------------------------
 * read_bit
 * Read a single bit from the 16-bit port
 */
00082 int  MCP23016::read_bit(int bit_number) {
    shadow_GPIO = readRegister(GP0);
    return  ((shadow_GPIO >> bit_number) & 0x0001);
}
 
/*-----------------------------------------------------------------------------
 * read_mask
 */
00090 int  MCP23016::read_mask(unsigned short mask) {
    shadow_GPIO = readRegister(GP0);
    return (shadow_GPIO & mask);
}
 
/*-----------------------------------------------------------------------------
 * Config
 * set direction and pull-up registers
 */
00099 void MCP23016::config(unsigned short dir_config, unsigned short polarity_config) {
    shadow_IODIR = dir_config;
    writeRegister(IODIR0, (unsigned short)shadow_IODIR);
    shadow_IPOL = polarity_config;
    writeRegister(IPOL0, (unsigned short)shadow_IPOL);
}
 
/*-----------------------------------------------------------------------------
 * writeRegister
 * write a byte
 */
void MCP23016::writeRegister(int regAddress, unsigned char data) {
    char  buffer[2];
 
    buffer[0] = regAddress;
    buffer[1] = data;
    _i2c.write(MCP23016_i2cAddress, buffer, 2);
}
 
/*----------------------------------------------------------------------------
 * write Register
 * write two bytes
 */ 
void MCP23016::writeRegister(int regAddress, unsigned short data) {
    char  buffer[3];
 
    buffer[0] = regAddress;
    tmp_data.value16 = data;
    buffer[1] = tmp_data.value8[0];
    buffer[2] = tmp_data.value8[1];
 
    _i2c.write(MCP23016_i2cAddress, buffer, 3);
}
 
/*-----------------------------------------------------------------------------
 * readRegister
 */
int MCP23016::readRegister(int regAddress) {
    char buffer[2];
 
    buffer[0] = regAddress;
    _i2c.write(MCP23016_i2cAddress, buffer, 1);
    _i2c.read(MCP23016_i2cAddress, buffer, 2);
 
    return ((int)(buffer[0] + (buffer[1]<<8)));
}
 
/*-----------------------------------------------------------------------------
 * pinMode
 */
void MCP23016::pinMode(int pin, int mode) {
    if (mode == DIR_INPUT) {
        shadow_IODIR |= 1 << pin;
    } else {
        shadow_IODIR &= ~(1 << pin);
    }
    writeRegister(IODIR0, (unsigned short)shadow_IODIR);
}
 
/*-----------------------------------------------------------------------------
 * digitalRead
 */
int MCP23016::digitalRead(int pin) {
    shadow_GPIO = readRegister(GP0);
    if ( shadow_GPIO & (1 << pin)) {
        return 1;
    } else {
        return 0;
    }
}
 
/*-----------------------------------------------------------------------------
 * digitalWrite
 */
void MCP23016::digitalWrite(int pin, int val) {
    // This will set the OUTPUT voltage
    //as appropriate.
    bool isOutput = !(shadow_IODIR & 1<<pin);
 
    if (isOutput) {
        //This is an output pin so just write the value
        if (val) shadow_GPIO |= 1 << pin;
        else shadow_GPIO &= ~(1 << pin);
        writeRegister(OLAT0, (unsigned short)shadow_GPIO);
    }
}
 
/*-----------------------------------------------------------------------------
 * digitalWordRead
 */
unsigned short MCP23016::digitalWordRead() {
    shadow_GPIO = readRegister(GP0);
    return shadow_GPIO;
}
 
/*-----------------------------------------------------------------------------
 * digitalWordWrite
 */
void MCP23016::digitalWordWrite(unsigned short w) {
    shadow_GPIO = w;
    writeRegister(OLAT0, (unsigned short)shadow_GPIO);
}
 
/*-----------------------------------------------------------------------------
 * inputPolarityMask
 */
void MCP23016::inputPolarityMask(unsigned short mask) {
    writeRegister(IPOL0, mask);
}
 
/*-----------------------------------------------------------------------------
 * inputoutputMask
 */
void MCP23016::inputOutputMask(unsigned short mask) {
    shadow_IODIR = mask;
    writeRegister(IODIR0, (unsigned short)shadow_IODIR);
}
 
