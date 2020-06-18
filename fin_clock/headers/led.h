#ifndef LED_H
#define LED_H

#include <stdint.h>

#define LED_BASE (0x1C010008u)
#define LED      ((uint8_t *)LED_BASE)

void ignite(uint8_t);

#endif // LED_H
