/*
 * File Name: uart.c
 * ATTiny 841 UART0 driver
 * Created: 09/05/2015 11:34:36
 * Author: Kishan Amratia
 * UART Receive code adapted from: https://github.com/andygock/avr-uart/blob/master/uart.c
 */ 

#include "uart.h"

static volatile uint8_t UART_RxBuf[UART_RX0_BUFFER_SIZE];
static volatile uint8_t UART_RxHead;
static volatile uint8_t UART_RxTail;
static volatile uint8_t UART_LastRxError;

void uart_init(void) {
	PRR &= ~(1<<PRUSART0);	// disable Power Reduction USART0 bit
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	#if USE_2X
	UCSR0A |= (1<<U2X0);
	#else
	UCSR0A &= ~(1<<U2X0);
	#endif
	
	// Parity Mode disabled by default (Page 184 of data sheet)
	// 1 stop-bit by default
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00); // 8-bit data
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);   // Enable RX and TX
	
	UCSR0B |= (1<<RXCIE0);	// Enable RX Complete Interrupt
	
}

void uart0_putc( unsigned char data )
{
	while ( !( UCSR0A & (1<<UDRE0)) );       // Wait for empty transmit buffer
	UCSR0A |= (1<<TXC0);                     // clear TXC (Transmit Complete) flag
	UDR0 = data;                             // Put data into buffer, sends the data
	loop_until_bit_is_set(UCSR0A, TXC0);    // Wait while TXC0 is set i.e. the buffer is shifted out
}

void uart0_puts( char *str )
{
	while (*str) {
		uart0_putc(*str);
		str++;
	}
}

void uart_echo(void)
{	// Echo the received byte back i.e. transmit what you receive
	char c;
	c = UDR0; // Fetch the received byte value into the variable "ByteReceived"
	uart0_putc(c);
}

uint16_t uart0_available(void)
{	// Determine number of bytes waiting in the receive buffer
	return (UART_RX0_BUFFER_SIZE + UART_RxHead - UART_RxTail) & UART_RX0_BUFFER_MASK;
}

char uart0_getc(void)
{
	uint16_t tmptail;
	uint8_t data;

	if ( UART_RxHead == UART_RxTail ) {
		return UART_NO_DATA;   /* no data available */
	}

	/* calculate /store buffer index */
	tmptail = (UART_RxTail + 1) & UART_RX0_BUFFER_MASK;
	UART_RxTail = tmptail;

	/* get data from receive buffer */
	data = UART_RxBuf[tmptail];

	return data;
}

ISR(USART0_RX_vect)
{
	// Interrupt Handler when character is received on UART0
	// uart_echo();
	uint16_t tmphead;
	uint8_t data;
	uint8_t usr;
	uint8_t lastRxError;
	
	/* read UART status register and UART data register */
	usr  = UCSR0A;
	data = UDR0;
	
	lastRxError = (usr & (_BV(FE0)|_BV(DOR0)) );
	
	/* calculate buffer index */
	tmphead = ( UART_RxHead + 1) & UART_RX0_BUFFER_MASK;
	
	if ( tmphead == UART_RxTail ) {
		/* error: receive buffer overflow */
		lastRxError = UART_BUFFER_OVERFLOW >> 8;
		} else {
		/* store new index */
		UART_RxHead = tmphead;
		/* store received data in buffer */
		UART_RxBuf[tmphead] = data;
	}
	UART_LastRxError = lastRxError;
}


