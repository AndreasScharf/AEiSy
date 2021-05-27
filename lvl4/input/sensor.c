#include <LPC177x_8x.h>
#include <stdint.h>

#define links 17
#define mitte 18
#define rechts 4
#define t10us 100

int sensoren[3];
float distances[3];



void sensor_init(void){
		//Timer für Ultraschall-Interrupts initialisieren mit 1 us
		LPC_SC->PCONP |= 2; //Timer 0 anschalten
		
		//Timer Prescale auf 2 > Frequenz von 10MHz / 1 Count = 100ns
		LPC_TIM0->PR = 2;	
		
		sensoren[0] = links;
		sensoren[1] = mitte;
		sensoren[2] = rechts;
}

float get_distance(int sensor){
		int sensorpin = sensoren[sensor+1];
		
		//Sensor als Output konfigurieren und anschalten
		LPC_GPIO1->DIR |= (1 << sensorpin); 
		LPC_GPIO1->SET |= (1 << sensorpin);

		//Timer starten: 10us warten
		LPC_TIM0->TCR = 1;	
		LPC_TIM0->TC = 0;	
		
		while (LPC_TIM0->TC < t10us){}
	
		//Sensorpin ausschalten
		LPC_GPIO1->CLR |= (1 << sensorpin);
			
		//kurz warten um auf Input umzuschalten
		LPC_TIM0->TC = 0;
			
		while (LPC_TIM0->TC < t10us *10 ){}	//warte 100us auf wegen pull up auf sensr
			
		LPC_GPIO1->DIR &= ~(1 << sensorpin);
		
		//auf steigende Flanke warten:	
		int input = 0;
			
		while(1){
				//Wert per Polling abfragen
				input = LPC_GPIO1->PIN & (1 << sensorpin);
				if (input == (1 << sensorpin)){//steigende Flanke
						break;
				}			
		}
		
		//Timer zurücksetzen
		int last_tc = LPC_TIM0->TC;
		LPC_TIM0->TC = 0;
		
		
		//auf fallende Flanke warten und Timerwert auslesen
		int measure = 0;
		
		while(1){
				//Wert per Polling abfragen
				input = LPC_GPIO1->PIN & (1 << sensorpin);
			
				if (input == 0){//fallende Flanke
						measure = LPC_TIM0->TC;
						break;
				}			
		}
		
		return (float)measure / 580.0f;;
		
		
}

