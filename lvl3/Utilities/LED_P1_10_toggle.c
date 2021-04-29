#include <stdint.h>
#include <LPC177x_8x.h>

static _Bool togglee = 0;

void toggle(void)
{
		
	
		LPC_GPIO1->SET = (togglee<<10);
		LPC_GPIO1->CLR = ((!togglee)<<10);
		togglee = !togglee;
		
	
}
