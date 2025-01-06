/*
 * EEPROM.h
 * Description: Header file of the DC motor
 */
#ifndef EEPROM_H_
#define EEPROM_H_

#include"std_types.h"
#define SUCCESS (1)
#define ERROR   (0)
#define TWI_ADDRESS 0b0000001

/*
 * Description :
 * Function responsible for writing only one byte in the EEPROM
 */
uint8 EEPROM_writeByte(uint16 address,uint8 data);
/*
 * Description :
 * Function responsible for reading only one byte from the EEPROM
 */
uint8 EEPROM_readByte(uint16 address,uint8 *data);

#endif /* EEPROM_H_ */
