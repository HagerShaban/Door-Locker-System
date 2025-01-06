/*
 * I2C.h
 * Description: Header file for two wire interface (I2C)
 */

#include"std_types.h"

#define TWI_START 0x08
#define TWI_REPEATED_START 0x10
#define TWI_MT_SLA_W_ACK 0x18
#define TWI_MT_SLA_R_ACK 0x40
#define TWI_MT_DATA_ACK 0x28
#define TWI_MR_DATA_ACK 0x50
#define TWI_MR_DATA_NACK  0x58


/*enum is used to differentiate between the normal mode and fast mode*/
typedef enum{
	NORMAL_MODE_100K=100000,FAST_MODE_400K=400000
}TWI_mode;


typedef enum
{
   Prescaler_1, Prescaler_4, Prescaler_16, Prescaler_64
}TWI_Prescaler;


typedef struct{
	TWI_mode mode;
	TWI_Prescaler prescaler;
	uint8 address;
}TWI_configType;


/*
 * Description :
 * Function responsible for initializing the I2C based on the structure given
 */
void TWI_init(TWI_configType *Config_Ptr);
/*
 * Description :
 * Function responsible for sending a start bit
 */
void TWI_start(void);
/*
 * Description :
 * Function responsible for sending a stop bit
 */
void TWI_stop(void);
/*
 * Description :
 * Function responsible for writing a byte to the SDA bus
 */
void TWI_writeByte(uint8 data);
/*
 * Description :
 * Function responsible for reading a byte to the SDA bus and respond with the ACK
 */
uint8 TWI_readByteWithACK(void);
/*
 * Description :
 * Function responsible for reading a byte to the SDA bus and not respond with the ACK
 */
uint8 TWI_readByteWithNACK(void);

/*
 * Description :
 * Function responsible for getting the status of the TWI
 */
uint8 TWI_getStatus(void);
