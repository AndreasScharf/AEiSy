#include <stdint.h>
#include <LPC177x_8x.h>
#include "../Output/motor.h"

float dummfang = 6.6f * 3.14f;
static _Bool togglee = 0;
static _Bool toggleee = 0;

static int old_counter_right = 0;
static int old_counter_left = 0;
static int current_counter_right = 0;
static int current_counter_left = 0;
static float speed = 0;

void toggle_LED(void){
		
	
		LPC_GPIO1->SET = (togglee<<10);
		LPC_GPIO1->CLR = ((!togglee)<<10);
		togglee = !togglee;
			
}

float getSpeed(){
	return speed;
}

void SysTick_Handler(){
	
	if (toggleee){
		toggle_LED();
	}
	toggleee = !toggleee;
	
	current_counter_right = CounterRight();
	current_counter_left = CounterLeft();
	
	speed = ((float)((current_counter_right - old_counter_right)+(current_counter_left - old_counter_left))/240.0f)*dummfang;// geteilt durch 2(für Mittelwert) und durch 0.5(sek)
	
	
	old_counter_right = current_counter_right;
	old_counter_left = current_counter_left;
		
}
