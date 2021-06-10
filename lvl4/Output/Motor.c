#include <LPC177x_8x.h>
#include <stdint.h>



typedef enum{
	FORWARD = 0,
	LEFT = 1,
	RIGHT = 2
} driving_states;

void set_direction(driving_states direction);

void motor_init(void){
		LPC_SC->PCONP |= (1 << 6); //PC PWM 1 anschalten
		LPC_IOCON->P1_20 = (1 << 1); //PWM1[2]
		LPC_IOCON->P1_21 = (1 << 1); //PWM1[3]
		LPC_GPIO1->DIR |= (0xF << 23); //Pins auf output schalten
	
		LPC_PWM1->PCR |= (1 << 10) | (1 << 11); //PWM[2] und PWM[3] enablen
		LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit
		LPC_PWM1->MR0 = 100; //Maximum für PWM
		LPC_PWM1->MCR |= (1 << 1); //Reset von TC bei MR0
	
		LPC_PWM1->TCR |= 1;	//PWM Timer starten
		set_direction(FORWARD);//linken und rechten Motor auf 100% setzen<
}


void drive(int reverse){
		LPC_GPIO1->CLR &= ~(0xF << 23); 	
		LPC_GPIO1->SET |= (1 << (25 + reverse));//linker Motor
		LPC_GPIO1->SET |= (1 << (23 + reverse));//rechter Motor
}

void stop_motors(){
	LPC_PWM1->MR2 = 100;
	LPC_PWM1->MR3 = 100;	
	LPC_GPIO1->CLR &= ~(0xF << 23);
	
}

void set_direction(driving_states direction){
		switch (direction){
			case FORWARD:
					LPC_PWM1->MR2 = 100;
					LPC_PWM1->MR3 = 100;			
					break;
			case LEFT:
					LPC_PWM1->MR2 = 100;
					LPC_PWM1->MR3 = 50;			
					break;
			case RIGHT:
					LPC_PWM1->MR2 = 50;
					LPC_PWM1->MR3 = 100;			
					break;				
		}
}