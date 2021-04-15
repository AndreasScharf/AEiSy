#include <stdint.h>
#include <LPC177x_8x.h>
#include "delay.h"


void toggle(void)
{
	static _Bool toggle = 0;	
	
		LPC_GPIO1->SET = (toggle<<10);
		LPC_GPIO1->CLR = (!toggle<<10);
		toggle = !toggle;
		
	
}
