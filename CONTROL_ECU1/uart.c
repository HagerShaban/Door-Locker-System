/*
 * uart.C
 * Description: Source of the uart driver
 */

#include"uart.h"
#include"avr/io.h"
#include"common_macros.h"

void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint16 ubrr_value = 0;
	/* for double transmission speed*/
	UCSRA = (1<<U2X);
	/* to enbale the transmitter and receiver*/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	UCSRC = (1<<URSEL) ;
	/*insert required parity mode in (UCSRC) register */
	UCSRC = (UCSRC & 0xCF) | ( ( Config_Ptr->parity_bit_type ) << 4 );
	/*insert required stop bits in (UCSRC) register */
	UCSRC = (UCSRC & 0xF7)  | ( ( Config_Ptr->number_of_stop_bits ) << 3 );


	if (Config_Ptr->number_of_bits == 9)
	{
		UCSRC |= (1<<UCSZ0) | (1<<UCSZ1);
		UCSRB |= (1<<UCSZ2);
	}
	else
	{
		UCSRC |= ( ( Config_Ptr->number_of_bits ) << 1 );
	}
	/*To calculate the ubrr */
	ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;
}


uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}


void UART_receiveString(uint8 *Str)
	{
		uint8 i = 0;

		/* Receive the first byte */
		Str[i] = UART_recieveByte();

		/* Receive the whole string until the '#' */
		while(Str[i] != '#')
		{
			i++;
			Str[i] = UART_recieveByte();
		}

		/* After receiving the whole string plus the '#', replace the '#' with '\0' */
		Str[i] = '\0';
	}
