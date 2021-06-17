#include <stdint.h>
#include <stdio.h>
#include <LPC177x_8x.h>
#include "Utilities/delay.h"
#include "Utilities/LED_P1_10_toggle.h"
#include "Input/input.h"
#include "Output/output.h"
#include "Output/lcd_display.h"
#include "Input/sensor.h"
#include "Input/compass.h"
#include "Output/Motor.h"

void init(void);


int main(void)
{
	init();
	write_text("Hallo",0,0);
	write_text("Sensor links: ",2,0);
	write_text("Sensor mitte: ",4,0);
	write_text("Sensor rechts:",6,0);
	write_text("Himmelsrichtung:",10,0);
	write_text("deg",10,25);
	
	while (1){
		float sl = get_distance(-1);
		float sm = get_distance(0);
		float sr = get_distance(1);
		//wenn float rumspinnt vielleicht auf integer wechseln, zwar weniger genau, aber Auflösung in cm sollte eigentlich auch reichen
				
		char s1[7];
		char s2[7];
		char s3[7];
		char s4[5];
		
		sprintf(s1, "%f", sl);
		write_text(s1, 2, 20);
		
		sprintf(s2, "%f", sm);
		write_text(s2, 4, 20);
		
		sprintf(s3, "%f", sr);
		write_text(s3, 6, 20);
		
		float dirc = (float)get_direction()/10.0f;
		sprintf(s4, "%f", dirc);
		write_text(s4, 10, 20);
		
		drive(0);		
		
		delayms(4000);
		
		set_direction(RIGHT);
		
		delayms(4000);
		
		stop_motors();
		set_direction(STRAIGHT);
		
		delayms(4000);
	}
	
}



void init(void){
	//LED-Dir setzen und System Timer Control enablen
	LPC_GPIO1->DIR |= (1<<10);

	
	
	input_init();
	output_init();
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = 0x98967F; //250 ms Intervall
	
	
}


