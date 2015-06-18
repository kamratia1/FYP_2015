#define UART_RX0_BUFFER_SIZE  32
#define UART_BUFFER_OVERFLOW  0x0200   // receive ring buffer overflow
#define UART_NO_DATA          0x0100   // no receive data available
#define UART_RX0_BUFFER_MASK ( UART_RX0_BUFFER_SIZE - 1)
#define BAUD 115200

#include "setup.h"
#include <avr/io.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

void uart_echo(void);
char uart0_getc(void);
void uart_init(void);
void uart0_putc(unsigned char);
void uart0_puts(char *);
uint16_t uart0_available(void);
