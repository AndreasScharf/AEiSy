#include <LPC177x_8x.h>
#include <stdint.h>
#include <math.h>
#include "../input/compass.h"

float radumfang = 6.6f * 3.14f;

//globale Counter für Odometer
int countRight = 0;
int countLeft = 0;

typedef enum{
	STRAIGHT = 0,
	LEFT = 1,
	RIGHT = 2
} driving_states;

void set_direction(driving_states direction);



void EINT1_IRQHandler() // Interrupt Rad Rechts
{
	countRight += 1;
	LPC_SC->EXTINT |= 1<<1;
}

void EINT2_IRQHandler() // Interrupt Rad Links
{
	countLeft += 1;
	LPC_SC->EXTINT |= 1<<2;
}

int CounterRight()
{
	return countRight;
}

int CounterLeft()
{	
	return countLeft;
}

float Regler(float IstWert, float SollWert, float Kp, float Ki)
{
  float u_ges, u_p;
	static float u_i;
  static float Abweichung=0;

  Abweichung=SollWert-IstWert;

  u_p  = Kp*Abweichung;

  u_i += Ki*Abweichung;

	u_ges = u_p + u_i;

  if (u_ges > 9999) u_ges = 9999;
  else if (u_ges < -9999) u_ges = -9999;

  return u_ges;
}


void motor_init(void){
		LPC_SC->PCONP |= (1 << 6); //PC PWM 1 anschalten
		LPC_IOCON->P1_20 = (1 << 1); //PWM1[2]
		LPC_IOCON->P1_21 = (1 << 1); //PWM1[3]
		LPC_GPIO1->DIR |= (0xF << 23); //Pins auf output schalten
	
		LPC_PWM1->PCR |= (1 << 10) | (1 << 11); //PWM[2] und PWM[3] enablen
		LPC_PWM1->MR0 = 10000; //Maximum für PWM		
		LPC_PWM1->LER |= (1 << 0);//MR0 updaten
		LPC_PWM1->MCR |= (1 << 1); //Reset von TC bei MR0	
		LPC_PWM1->TCR |= 1;	//PWM Timer starten
		set_direction(STRAIGHT);//linken und rechten Motor auf 100% setzen<
}

void stop_motors(){
	LPC_PWM1->MR2 = 9999;
	LPC_PWM1->MR3 = 9999;	
	LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit
	LPC_GPIO1->CLR |= (0xF << 23);
	
}


void drive(int reverse){
		LPC_GPIO1->CLR |= (0xF << 23);	
		LPC_GPIO1->SET |= (1 << (25 + reverse));//linker Motor
		LPC_GPIO1->SET |= (1 << (23 + reverse));//rechter Motor
		
}

void drive_distance(int reverse, int distance){//Distanz in cm
		set_direction(STRAIGHT);
	
		int currentCounterRight = countRight;
		int currentCounterLeft = countLeft;		
		
		float rotations = ((float)distance)/radumfang;		
	
		LPC_GPIO1->CLR |= (0xF << 23);	
		LPC_GPIO1->SET |= (1 << (25 + reverse));//linker Motor
		LPC_GPIO1->SET |= (1 << (23 + reverse));//rechter Motor
		
		while (countRight - currentCounterRight < rotations*240.0f && countLeft - currentCounterLeft < rotations*240.0f){}		
		stop_motors();	
}

void drive_degree(int reverse, int degree){//Distanz in cm
		set_direction(STRAIGHT);
	
		int currentCounterRight = countRight;
		int currentCounterLeft = countLeft;		
		
		float rotations = ((float)degree)/radumfang;		
	
		LPC_GPIO1->CLR |= (0xF << 23);	
		LPC_GPIO1->SET |= (1 << (25 + reverse));//linker Motor
		LPC_GPIO1->SET |= (1 << (23 + reverse));//rechter Motor
		
		float soll_direction = (float)get_direction()/10.0f;
		
		while (countRight - currentCounterRight < rotations*240.0f && countLeft - currentCounterLeft < rotations*240.0f){
			//Check if Driving straight			
			float ist_direction = (float)get_direction()/10.0f;
			float difference = soll_direction - ist_direction;
			if (difference > 300){
				ist_direction += 360.0f;
			}else if(difference < -300){
				ist_direction -= 360.0f;
			}
			
			float regelung = Regler(ist_direction, soll_direction, 10, 1);
			if (regelung > 0){
				LPC_PWM1->MR2 = 9999;
				LPC_PWM1->MR3 = 9999 - abs((int)regelung);	
				
				LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit				
				
			}else if (regelung < 0){
				LPC_PWM1->MR2 = 9999 - abs((int)regelung);	
				LPC_PWM1->MR3 = 9999;	
				LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit				
				
			}else{				
				LPC_PWM1->MR2 = 9999;
				LPC_PWM1->MR3 = 9999;	
				LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit
			
			}
		}		
		stop_motors();	
}


void odometer_init(){
	
	LPC_IOCON->P2_11 = 1 | (1<<4) | (1<<5); // External Interrupt 1 + Pull Up Hysterese
	LPC_IOCON->P2_12 = 1 | (1<<4) | (1<<5); // External Interrupt 2 + Pull Up Hysterese
	
	LPC_SC->EXTMODE = (1<<1) | (1<<2);		   // Edge Sensitivity Mode1 1 und 2
	LPC_SC->EXTPOLAR = (1<<1) | (1<<2);		   // Rising Edge for Ext Interrupt 1 und 2
	
	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
	
}



void set_direction(driving_states direction){
		switch (direction){
			case STRAIGHT:
					LPC_PWM1->MR2 = 9999;
					LPC_PWM1->MR3 = 9999;		
					LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit
					break;
			case LEFT:
					LPC_PWM1->MR2 = 9999;
					LPC_PWM1->MR3 = 5000;	
					LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit
					break;
			case RIGHT:
					LPC_PWM1->MR2 = 5000;
					LPC_PWM1->MR3 = 9999;
					LPC_PWM1->LER |= (1 << 2) | (1 << 3);//MR2 und MR3 updaten zur Laufzeit
					break;				
		}
}

