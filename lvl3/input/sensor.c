#include <LPC177x_8x.h>
#include <stdint.h>
#define links 17
#define mitte 18
#define rechts 4

int sensoren[3];
float distances[3];

void TIMER0_IRQHandler(){
		
		
		static int state = 0; //init_pulse 0, wait_rising 1, wait_falling/counting 2, change_sensor 3
		static int counter = 0;
		static int sensor = 0;
		static int old_GPIO = 0;
		int current_sensor = sensoren[sensor];
		
		switch (state){
			case 0:
				if (!(LPC_GPIO1->DIR & (1<<current_sensor))){				//beim ersten Aufruf init_pulse senden	
						LPC_GPIO1->DIR |= (1<<current_sensor);				
						LPC_GPIO1->SET |= (1<<current_sensor);
				}
					
				if (counter >= 9){//nach 10 us abschalten -> nächster State
					state++;
					counter = 0;
				}
				
				break;
			case 1:
				
				if ((LPC_GPIO1->DIR & (1<<current_sensor)))
						LPC_GPIO1->DIR &= ~(1<<current_sensor);
				
				//rising edge detektieren
				if (old_GPIO != (LPC_GPIO1->PIN & (1<<current_sensor))){
					counter = 0;//Zeitmessung startet
					state ++;
				}				
				old_GPIO = (LPC_GPIO1->PIN & (1<<current_sensor));
				
				//Timeout
				if (counter > 30000){//30ms vergangen ohne Antwort -> Timeout/Sensor wechseln
						distances[sensor] = 400;	
						state = 3;
				}
				break;	
			case 2:
				//falling edge detektieren
				if (old_GPIO != (LPC_GPIO1->PIN & (1<<current_sensor))){
					distances[sensor] = counter/58;		
					state++;
				}				
				old_GPIO = (LPC_GPIO1->PIN & (1<<current_sensor));
				break;
			case 3:
				sensor++;
			
				if (sensor == 3)
					sensor = 0;
				
				
				counter = -1;
				state = 0;
				break;
		}
		counter++;
		LPC_TIM0->IR = 1; //Interrupt(-Register) zurücksetzen
}
	


void sensor_init(void){
		
	
		//Timer für Ultraschall-Interrupts initialisieren mit 1 us
		
		
		LPC_TIM0->MCR = 3;//ersten beiden Bits setzen -> Aufruf bei Match und Reset des Counters
		LPC_TIM0->MR0 = 30;//bei 30 MHz -> Aufruf alle 1 us
		LPC_TIM0->TCR = 1;//Timer0 starten
	
		NVIC->ISER[0] |= (1<<1);//Timer0 Interrupt Enable
	
		sensoren[0] = links;
		sensoren[1] = mitte;
		sensoren[2] = rechts;
}


float get_distance(int number){//-1 links; 0 mitte; 1 rechts
	
	
	//Interrupt disablen
	//NVIC->ICER[0] |= (1<<1);//Timer0 Interrupt Clear
	
	//Lesezugriff
	float distance = distances[number+1];
	
	//Interrupt wieder enablen
	//NVIC->ISER[0] |= (1<<1);//Timer0 Interrupt Enable
	
	
	
	return distance;
}

