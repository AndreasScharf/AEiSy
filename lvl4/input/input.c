
#include "sensor.h"
#include "../compass.h"


void input_init(void){
	sensor_init();
	compass_init();
}
