#include <stdint.h>
#include <stdio.h>
#include <LPC177x_8x.h>
#include "Utilities/delay.h"
#include "Utilities/LED_P1_10_toggle.h"
#include "Input/input.h"
#include "Output/output.h"
#include "Output/lcd_display.h"

void init(void);




int main(void)
{
	init();
	write_text("Hallo",0,0);
	write_text("Sensor links: ",2,0);
	write_text("Sensor mitte: ",4,0);
	write_text("Sensor rechts:",6,0);
	
	
	while (1){
		float sl = get_distance(-1);
		float sm = get_distance(0);
		float sr = get_distance(1);
		
		char s1[7];
		char s2[7];
		char s3[7];
		
		sprintf(s1, "%f", sl);
		sprintf(s2, "%f", sm);
		sprintf(s3, "%f", sr);
		
		write_text(s1, 2, 15);
		write_text(s2, 4, 15);
		write_text(s3, 6, 15);
		
		
		delayms(100);
	}
	
}



void init(void){
	//LED-Dir setzen und System Timer Control enablen
	LPC_GPIO1->DIR |= (1<<10);

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = 0x98967F; //250 ms Intervall
	
	input_init();
	output_init();
	
	
}


