#ifndef SERIAL_H_
#define SERIAL_H_

// The following type definition is no longer needed,
// because `stdint.h` of ArmClang has defined them.
// typedef unsigned          char uint8_t;
// typedef unsigned short     int uint16_t;
// typedef unsigned           int uint32_t;
// typedef unsigned       __int64 uint64_t;
#include <stdint.h>

#define   __RO     volatile const
#define   __WO     volatile
#define   __RW     volatile

/* UART - Register Layout Typedef */
typedef struct {
  __RW uint32_t UARTDR;             /*0x000 Data Register*/
  __RW uint32_t UARTRSR_UARTECR;    /*0x004 Receive Status Register/Error Clear Register */
  uint32_t RESERVED_0[4];           /*0x008 - 0x014*/
  __RO uint32_t UARTFR;             /*0x018 Flag Register*/
  uint32_t RESERVED_1;              /*0x01c*/
  __RW uint32_t UARTILPR;           /*0x020 IrDA Low-Power Counter Register*/
  __RW uint32_t UARTIBRD;           /*0x024 Integer Baud Rate Register */
  __RW uint32_t UARTFBRD;           /*0x028 Fractional Baud Rate Register*/
  __RW uint32_t UARTLCR_H;          /*0x02c Line Control Register */
  __RW uint32_t UARTCR;             /*0x030 Control Register*/
  __RW uint32_t UARTIFLS;           /*0x034 Interrupt FIFO Level Select Register*/
  __RW uint32_t UARTIMSC;           /*0x038 Interrupt Mask Set/Clear Register*/
  __RO uint32_t UARTRIS;            /*0x03c Raw Interrupt Status Register*/
  __RO uint32_t UARTMIS;            /*0x040 Masked Interrupt Status Register*/
  __WO uint32_t UARTICR;            /*0x044 Interrupt Clear Register*/
  __RW uint32_t UARTDMACR;          /*0x048 DMA Control Register*/
  uint32_t RESERVED_2[13];          /*0x04c - 0x07c*/
  uint32_t RESERVED_T[4];           /*0x080 - 0x08c*/
  uint32_t RESERVED_3[976];         /*0x090 - 0xfcc*/
  uint32_t RESERVED_4[4];           /*0xfd0 - 0xfdc*/
  __RO uint32_t UARTPeriphID0;      /*0xfe0 UARTPeriphID0 Register*/
  __RO uint32_t UARTPeriphID1;      /*0xfe4 UARTPeriphID1 Register*/
  __RO uint32_t UARTPeriphID2;      /*0xfe8 UARTPeriphID2 Register*/
  __RO uint32_t UARTPeriphID3;      /*0xfec UARTPeriphID3 Register*/
  __RO uint32_t UARTCellID0;        /*0xff0 UARTCellID0 Register*/
  __RO uint32_t UARTCellID1;        /*0xff4 UARTCellID1 Register*/
  __RO uint32_t UARTCellID2;        /*0xff8 UARTCellID2 Register*/
  __RO uint32_t UARTCellID3;        /*0xffc UARTCellID3 Register*/
} UART_Type;


/* Line control register */
#define UART_LCRH_SPS             (1 << 7)
#define UART_LCRH_WLEN_8          (3 << 5)
#define UART_LCRH_WLEN_7          (2 << 5)
#define UART_LCRH_WLEN_6          (1 << 5)
#define UART_LCRH_WLEN_5          (0 << 5)
#define UART_LCRH_FEN             (1 << 4)
#define UART_LCRH_STP2            (1 << 3)
#define UART_LCRH_EPS             (1 << 2)
#define UART_LCRH_PEN             (1 << 1)
#define UART_LCRH_BRK             (1 << 0)


/* Control Register */
#define UART_CR_CTSEN             (1 << 15)
#define UART_CR_RTSEN             (1 << 14)
#define UART_CR_OUT2              (1 << 13)
#define UART_CR_OUT1              (1 << 12)
#define UART_CR_RTS               (1 << 11)
#define UART_CR_DTR               (1 << 10)
#define UART_CR_RXE               (1 << 9)
#define UART_CR_TXE               (1 << 8)
#define UART_CR_LPE               (1 << 7)
#define UART_CR_IIRLP             (1 << 2)
#define UART_CR_SIREN             (1 << 1)
#define UART_CR_UARTEN            (1 << 0)


/* Interrupt Mask Set/Clear Register */
#define UART_IMSC_OEIM            (1 << 10)
#define UART_IMSC_BEIM            (1 << 9)
#define UART_IMSC_PEIM            (1 << 8)
#define UART_IMSC_FEIM            (1 << 7)
#define UART_IMSC_RTIM            (1 << 6)
#define UART_IMSC_TXIM            (1 << 5)
#define UART_IMSC_RXIM            (1 << 4)
#define UART_IMSC_DSRMIM          (1 << 3)
#define UART_IMSC_DCDMIM          (1 << 2)
#define UART_IMSC_CTSMIM          (1 << 1)
#define UART_IMSC_RIMIM           (1 << 0)

#define UART0_BASE      (0x1C090000u)  /* CS3 0x1C000000 + 0x090000 (VE ARM Cortex-A Series memory map) */
#define UART0           ((UART_Type *)UART0_BASE)

#define UART0_CLK       24000000 // 24MHz

extern void SER_Init(void);
extern void SER_Enable(void);
extern void SER_Disable(void);
extern char SER_GetChar (void);
extern void SER_PutChar(char c);
extern void SER_Set_baud_rate(uint32_t baud_rate);
extern void SER_Set_RxInterrupt(int flag);

#endif /* SERIAL_H_ */
