#include <led.h>

void ignite(uint8_t code)
{
	*LED = 1 << (code % 8);
}
