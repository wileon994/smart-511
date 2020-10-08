#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0 PEout(8)	// DS0
void LED_Init(void);
void LED_on(void);
void LED_off(void);
#endif
