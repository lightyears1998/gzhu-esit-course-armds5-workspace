// Example of using the Generic Timer in AArch64
//
// Copyright (C) Arm Limited, 2019 All rights reserved.
//
// The example code is provided to you as an aid to learning when working
// with Arm-based technology, including but not limited to programming tutorials.
// Arm hereby grants to you, subject to the terms and conditions of this Licence,
// a non-exclusive, non-transferable, non-sub-licensable, free-of-charge licence,
// to use and copy the Software solely for the purpose of demonstration and
// evaluation.
//
// You accept that the Software has not been tested by Arm therefore the Software
// is provided as is, without warranty of any kind, express or implied. In no
// event shall the authors or copyright holders be liable for any claim, damages
// or other liability, whether in action or contract, tort or otherwise, arising
// from, out of or in connection with the Software or the use of Software.
//
// ------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "gicv3_basic.h"
#include "generic_timer.h"
#include "system_counter.h"
#include "serial.h"
#include "spider.h"

extern uint32_t getAffinity(void);
uint32_t initGIC(void);

volatile int tick = -1;

// --------------------------------------------------------

int main(void)
{

#ifdef TEST_C
  // Test basic C function
  {
    printf("Test: sizeof(uint8_t) = %zu\n", sizeof(uint8_t));
    printf("Test: sizeof(uint16_t) = %zu\n", sizeof(uint16_t));
    printf("Test: sizeof(uint32_t) = %zu\n", sizeof(uint32_t));
    printf("Test: sizeof(uint64_t) = %zu\n", sizeof(uint64_t));
  }
#endif // TEST_C

  //
  // Configure UART
  //
  SER_Init();
  print("UART is ready.\r\n");

#ifdef TEST_UART_BLOCK_READING
  // Test UART block reading
  {
	int i = 0;
	char buffer[128], ch = 0;

	print("Test: Say something to test block reading: \r\n> ");
    while (ch != '\n') {
      ch = SER_GetChar();
      SER_PutChar(ch);
      buffer[i++] = ch;
    }
    buffer[i - 2] = '\0';

    if (strlen(buffer) > 0) {
      print("Test: I heard \"%s\", which is good enough.\r\n", buffer);
    } else {
      print("Test: I heard nothing!\r\n");
    }
  }
#endif // TEST_UART_BLOCK_READING

  uint64_t current_time;
  uint32_t rd;

  //
  // Configure the interrupt controller
  //
  rd = initGIC(); // GIC == Generic Interrupt Controller

  // Secure Physical Timer (INTID 29)
  setIntPriority(29, rd, 0);
  setIntGroup(29, rd, 0);
  enableInt(29, rd);

  // UART0, PL011 (INTID 37)
  setIntPriority(37, rd, 0);
  setIntGroup(37, rd, 0);
  enableInt(37, rd);

  //
  // Configure and enable the System Counter
  //
  setSystemCounterBaseAddr(0x2a430000);  // Address of the System Counter

  // Print timer frequency.
  {
	  char msg[64];
	  sprintf(msg, "Timer frequency: %dHz\r\n", getCNTFID(SYSTEM_COUNTER_CNTCR_FREQ0));
	  print(msg);
  }

  // Enable timer
  initSystemCounter(SYSTEM_COUNTER_CNTCR_nHDBG,
                    SYSTEM_COUNTER_CNTCR_FREQ0,
                    SYSTEM_COUNTER_CNTCR_nSCALE);

  //
  // Configure timer
  //

  // Configure the Secure Physical Timer
  // This uses the CVAL/comparator to set an absolute time for the timer to fire
  current_time = getPhysicalCount();
  setSEL1PhysicalCompValue(current_time + 10000);
  setSEL1PhysicalTimerCtrl(CNTPS_CTL_ENABLE);


  // NOTE:
  // This code assumes that the IRQ and FIQ exceptions
  // have been routed to the appropriate Exception level
  // and that the PSTATE masks are clear.  In this example
  // this is done in the startup.s file

  // Allow UART receive interrupt
  SER_Set_RxInterrupt(1);

  //
  // Spin until my CET6 pass
  //
  print("\r\n");
  while(tick < INT_MAX)
  {
	  continue;
  }
  
  printf("Main(): Test end\n");

  return 1;
}

// --------------------------------------------------------

void timerTick(void)
{
  uint64_t current_time;
  char msg[128];

  setSEL1PhysicalTimerCtrl(0);  // Disable timer to clear interrupt
  current_time = getPhysicalCount();

  ++tick;
  sprintf(msg, "[%d] %lu\r\n", tick, current_time);
  print(msg);

  setSEL1PhysicalCompValue(current_time + 100000000);
  setSEL1PhysicalTimerCtrl(CNTPS_CTL_ENABLE);
}

// --------------------------------------------------------

void handleInput(void)
{
  char ch;

  SER_Set_RxInterrupt(0);

  ch = UART0->UARTDR;
  if (!isspace(ch)) {
	printf("UART: Received '%c'\n", ch);
  } else {
	printf("UART: Received character %d\n", (int)ch);
  }

  SER_Set_RxInterrupt(1);
}

// --------------------------------------------------------

void fiqHandler(void)
{
  unsigned int ID;

  // Read the IAR to get the INTID of the interrupt taken
  ID = readIARGrp0();

  printf("FIQ: Received INTID %d\n", ID);

  switch (ID)
  {
    case 29:
      printf("FIQ: Secure Physical Timer\n");
      timerTick();
      break;
    case 37:
      printf("FIQ: UART0, PL011\n");
      handleInput();
      break;
    case 1023:
      printf("FIQ: Interrupt was spurious\n");
      return;
    default:
      printf("FIQ: Panic, unexpected INTID\n");
  }

  // Write EOIR to deactivate interrupt
  writeEOIGrp0(ID);

  return;
}

// --------------------------------------------------------

uint32_t initGIC(void)
{
  uint32_t rd;

  // Set location of GIC
  setGICAddr((void*)0x2F000000, (void*)0x2F100000);

  // Enable GIC
  enableGIC();

  // Get the ID of the Redistributor connected to this PE
  rd = getRedistID(getAffinity());

  // Mark this core as begin active
  wakeUpRedist(rd);

  // Configure the CPU interface
  // This assumes that the SRE bits are already set
  setPriorityMask(0xFF);
  enableGroup0Ints();
  enableGroup1Ints();

  return rd;
}
