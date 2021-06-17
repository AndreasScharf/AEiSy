#include "lcd_display.h"
#include "Motor.h"


void output_init(void){
	display_init();
	motor_init();
	odometer_init();
}
