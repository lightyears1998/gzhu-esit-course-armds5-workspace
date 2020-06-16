#include "serial.h"

void SER_Init(void)
{
    SER_Disable();  // Disable UART
    SER_Set_baud_rate( 38400 );
    // UART0->UARTLCR_H = UART_LCRH_WLEN_8 | UART_LCRH_FEN;  // 8 bits, 1 stop bit, no parity, FIFO enabled
    UART0->UARTLCR_H = UART_LCRH_WLEN_8;  // 8 bits, 1 stop bit, no parity and FIFO disabled
    SER_Enable();  // Enable UART and enable TX/RX
}

/*----------------------------------------------------------------------------
  Enable Serial Port
 *----------------------------------------------------------------------------*/
void SER_Enable(void)
{
    UART0->UARTCR = UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE;
}

/*----------------------------------------------------------------------------
  Disable Serial Port
 *----------------------------------------------------------------------------*/
void SER_Disable(void)
{
    UART0->UARTCR = 0x0;
}

/*----------------------------------------------------------------------------
  Set Receive Interrupt
 *----------------------------------------------------------------------------*/
void SER_Set_RxInterrupt(int flag)
{
	// write 1 to enable while write 0 to clear.
	UART0->UARTIMSC = (flag & 1) << 4;
}

/*----------------------------------------------------------------------------
  Set baud rate
 *----------------------------------------------------------------------------*/
void SER_Set_baud_rate(uint32_t baud_rate)
{
    uint32_t divider;
    uint32_t mod;
    uint32_t fraction;

    /*
     * Set baud rate
     *
     * IBRD = UART_CLK / (16 * BAUD_RATE)
     * FBRD = ROUND((64 * MOD(UART_CLK,(16 * BAUD_RATE))) / (16 * BAUD_RATE))
     */
    divider   = UART0_CLK / (16 * baud_rate);
    mod       = UART0_CLK % (16 * baud_rate);
    fraction  = (((8 * mod) / baud_rate) >> 1) + (((8 * mod) / baud_rate) & 1);

    UART0->UARTIBRD = divider;
    UART0->UARTFBRD = fraction;
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_PutChar(char c)
{
    while (UART0->UARTFR & 0x20);   // Wait for UART TX to become free
    //if (c == '\n')
    //{
    //  UART0->UARTDR = '\r';
    //  while (UART0->UARTFR & 0x20);
    //}
    UART0->UARTDR = c;
}

/*----------------------------------------------------------------------------
  Read character from Serial Port (blocking read)
 *----------------------------------------------------------------------------*/
char SER_GetChar (void)
{
    while (UART0->UARTFR & 0x10);  // Wait for a character to arrive
    return UART0->UARTDR;
}

