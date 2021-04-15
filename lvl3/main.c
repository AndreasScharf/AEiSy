#include <stdint.h>
#include <LPC177x_8x.h>
#include "delay.h"
#include "LED_P1_10_toggle.h"


int main(void)
{
	LPC_GPIO1->DIR |= (1<<10);
	
	while(1)
	{	
		toggle();
		delayms(500);
		
		
	}
}
