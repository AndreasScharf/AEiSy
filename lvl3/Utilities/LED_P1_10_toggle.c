#include <stdint.h>
#include <LPC177x_8x.h>

static _Bool togglee = 0;
static _Bool toggleee = 0;

void toggle_LED(void){
		
	
		LPC_GPIO1->SET = (togglee<<10);
		LPC_GPIO1->CLR = ((!togglee)<<10);
		togglee = !togglee;
			
}

void SysTick_Handler(){
	
	if (toggleee){
		toggle_LED();
	}
	toggleee = !toggleee;
}
