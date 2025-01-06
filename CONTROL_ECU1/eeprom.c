/*
 * EEPROM.c
 * Description: Source file of the DC motor
 */

#include"twi.h"
#include"eeprom.h"

uint8 EEPROM_writeByte(uint16 address,uint8 data){
	/* First : we send the start bit */
	TWI_start();
	/*Check of sending the start bit*/
	if(TWI_getStatus() != TWI_START){
		return ERROR;
	}

	TWI_writeByte((uint8)(0xA0 | ((address & 0x0700)>>7)));
	/*Check of sending the slave address and responded with ACK*/
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK){
		return ERROR;
	}
	/*sending the first 8-bits in the location address*/
	TWI_writeByte((uint8)(address));
	/*Check of sending the location address and responded with ACK*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK){
		return ERROR;
	}
	/*sending the data in the location address*/
	TWI_writeByte(data);
	/*Check of sending the data and responded with ACK*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK){
		return ERROR;
	}
	TWI_stop();
	/*return success operation */
	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 address,uint8 *u8data){
	/* First : we send the start bit
	 * */
	TWI_start();
	/*Check of sending the start bit*/
	if(TWI_getStatus() != TWI_START){
		return ERROR;
	}
		TWI_writeByte((uint8)((0xA0) | ((address & 0x0700)>>7)));
	/*Check of sending the slave address and responded with ACK*/
	if(TWI_getStatus() != TWI_MT_SLA_W_ACK){
		return ERROR;
	}
	/*sending the first 8-bits in the location address*/
	TWI_writeByte((uint8)(address));
	/*Check of sending the location address and responded with ACK*/
	if(TWI_getStatus() != TWI_MT_DATA_ACK){
		return ERROR;
	}
	/*Sending the repeated start bit*/
	TWI_start();
	if(TWI_getStatus() != TWI_REPEATED_START){
		return ERROR;
	}
	/*Here, we need to send read control bit so we will make or operation with 1*/
	TWI_writeByte((uint8)((0xA0) | ((address & 0x0700)>>7) | 1));
	/*Check of sending the slave address and responded with ACK*/
	if(TWI_getStatus() != TWI_MT_SLA_R_ACK){
		return ERROR;
	}
	/*reading the byte*/
	*u8data=TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK){
        return ERROR;
       }
    TWI_stop();
	return SUCCESS;
}
