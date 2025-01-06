/*
 * uart.h
 * Description: Header of the uart driver
 */

#ifndef UART_H_
#define UART_H_
#include"std_types.h"

typedef enum
{
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, NINE_BITS = 7
}UART_NumberOfBits;

typedef enum
{
	ONE_STOP_BIT, TWO_STOP_BIT
}UART_StopBits;

typedef enum
{
	DISABLED, RESERVED, EVEN_PARITY, ODD_PARITY
}UART_ParityBitType;

typedef struct
{
	uint32 baud_rate;
	UART_NumberOfBits number_of_bits;
	UART_StopBits number_of_stop_bits;
	UART_ParityBitType parity_bit_type;
}UART_ConfigType;

/*
 * Description :
 * Function responsible for sending the byte
 */
void UART_sendByte(uint8 data);
/*
 * Description :
 * Function responsible for receiving a byte
 */
uint8 UART_recieveByte(void);
/*
 * Description :
 * Function responsible for initialize the uart device
 */
void UART_init(const UART_ConfigType * Config_Ptr);
/*
 * Description :
 * Function responsible for sending a string
 */
void UART_sendString(const uint8 *Str);
/*
 * Description :
 * Function responsible for receiving a string based on the special character '#'
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
