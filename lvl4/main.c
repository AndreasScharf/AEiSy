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
	
	clear_display();
	/*write_text("Hallo",0,0);
	write_text("Sensor links: ",2,0);
	write_text("Sensor mitte: ",4,0);
	write_text("Sensor rechts:",6,0);
	write_text("Himmelsrichtung:",10,0);
	write_text("deg",10,25);
	*/
	draw_pixel(0,0);
	draw_pixel(1,0);
	
	draw_circle(100,100,15);
	fill_circle(200,37,35);
	//draw_pixel(8,0);
	
	fill_circle(230,120,15);


	while((LPC_GPIO2->PIN & (1<<7)) == (1<<7)){}
		
	//while((LPC_GPIO2->PIN & (1<<8)) == (1<<8)){}
	
	
	while (1){
		float sl = get_distance(-1);
		float sm = get_distance(0);
		float sr = get_distance(1);
		//wenn float rumspinnt vielleicht auf integer wechseln, zwar weniger genau, aber Aufl�sung in cm sollte eigentlich auch reichen
				
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
		
		//drive(0);		
		drive_distance(1,20);
		
		
		delayms(4000);
				
				
		drive_degree(0,720);
		
		
		delayms(4000);
		
		//set_direction(RIGHT);		
		
		//stop_motors();
		//set_direction(STRAIGHT);
		
		//delayms(4000);
	}
	
}



void init(void){
	//LED-Dir setzen und System Timer Control enablen
	LPC_GPIO1->DIR |= (1<<10);
	
	//Buttons f�r Steuerung initialisieren
	LPC_GPIO2->DIR &= ~((1<<7) | (1<<8));
	
	
	input_init();
	output_init();
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = 0x98967F; //250 ms Intervall
	
	
}


