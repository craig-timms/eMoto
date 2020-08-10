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
 
#ifndef     MBED_MCP23016_H
#define     MBED_MCP23016_H
 
//#include    "mbed.h"
 
#define GP0     0x00    // Access to GP0
#define GP1     0x01    // Access to GP1
#define OLAT0   0x02    // Access to OLAT0
#define OLAT1   0x03    // Access to OLAT1
#define IPOL0   0x04    // Access to IPOL0
#define IPOL1   0x05    // Access to IPOL1
#define IODIR0  0x06    // Access to IODIR0
#define IODIR1  0x07    // Access to IODIR1
#define INTCAP0 0x08    // Access to INTCAP0 (Read-Only)
#define INTCAP1 0x09    // Access to INTCAP1 (Read-Only)
#define IOCON0  0x0A    // Access to IOCON0
#define IOCON1  0x0B    // Access to IOCON1
 
#define     I2C_BASE_ADDRESS    0x40
 
#define     DIR_OUTPUT      0
#define     DIR_INPUT       1
 
/** MCP23016 class
 *
 * Allow access to an I2C connected MCP23016 16-bit I/O extender chip
 * Example:
 * @code
 *      MCP23016     *par_port; 
 * @endcode
 *
 */
00050 class MCP23016 {
public:
    /** Constructor for the MCP23016 connected to specified I2C pins at a specific address
     *
     * 16-bit I/O expander with I2C interface
     *
     * @param   sda         I2C data pin
     * @param   scl         I2C clock pin
     * @param   i2cAddress  I2C address
     */
    MCP23016(PinName sda, PinName scl, int i2cAddress);
 
    /** Reset MCP23016 device to its power-on state
     */    
    void reset(void);
 
    /** Write a 0/1 value to an output bit
     *
     * @param   value         0 or 1
     * @param   bit_number    bit number range 0 --> 15
     */   
    void write_bit(int value, int bit_number);
      
    /** Write a masked 16-bit value to the device
     *
     * @param   data    16-bit data value
     * @param   mask    16-bit mask value
     */       
    void write_mask(unsigned short data, unsigned short mask);
 
    /** Read a 0/1 value from an input bit
     *
     * @param   bit_number    bit number range 0 --> 15
     * @return                0/1 value read
     */       
    int  read_bit(int bit_number);
    
    /** Read a 16-bit value from the device and apply mask
     *
     * @param   mask    16-bit mask value
     * @return          16-bit data with mask applied
     */     
    int  read_mask(unsigned short mask);
 
    /** Configure an MCP23016 device
     *
     * @param   dir_config         data direction value (1 = input, 0 = output)
     * @param   polarity_config    polarity value (1 = flip, 0 = normal)
     */           
    void config(unsigned short dir_config, unsigned short polarity_config);
 
    void writeRegister(int regAddress, unsigned char  val);
    void writeRegister(int regAddress, unsigned short val);
    int  readRegister(int regAddress);
 
/*----------------------------------------------------------------------------- 
 * pinmode
 * Set units to sequential, bank0 mode
 */  
    void pinMode(int pin, int mode); 
    void digitalWrite(int pin, int val);
    int  digitalRead(int pin);
 
// These provide a more advanced mapping of the chip functionality
// See the data sheet for more information on what they do
 
//Returns a word with the current pin states (ie contents of the GPIO register)
    unsigned short digitalWordRead();
// Allows you to write a word to the GPIO register
    void digitalWordWrite(unsigned short w);
// Sets up the polarity mask that the MCP23016 supports
// if set to 1, it will flip the actual pin value.
    void inputPolarityMask(unsigned short mask);
//Sets which pins are inputs or outputs (1 = input, 0 = output) NB Opposite to arduino's
//definition for these
    void inputOutputMask(unsigned short mask);
    int read(void);
    void write(int data);
 
protected:
    I2C     _i2c;
    int     MCP23016_i2cAddress;                        // physical I2C address
    unsigned short   shadow_GPIO, shadow_IODIR, shadow_IPOL;     // Cached copies of the register values
    
};
 
#endif
