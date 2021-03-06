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
#include <math.h>

void init(void);
void presentation(void);

typedef enum{
	MENU = 0,
	LEVEL4 = 1,
	LEVEL5 = 3,
	LEVEL6 = 5,
	LEVEL7 = 7,
	LEVEL8 = 9
} presentation_slide;

presentation_slide presentation_index;

int main(void)
{

	init();
	
	clear_display();
	
	
	presentation();
	/*write_text("Hallo",0,0);
	write_text("Sensor links: ",2,0);
	write_text("Sensor mitte: ",4,0);
	write_text("Sensor rechts:",6,0);
	write_text("Himmelsrichtung:",10,0);
	write_text("deg",10,25);
	
	//draw_pixel(0,0);
	//draw_pixel(1,0);
	
	//draw_circle(100,100,15);
	//fill_circle(200,37,35);
	//draw_pixel(8,0);
	
	//fill_circle(230,120,15);


	while((LPC_GPIO2->PIN & (1<<7)) == (1<<7)){}
		
	//while((LPC_GPIO2->PIN & (1<<8)) == (1<<8)){}
	
	
	while (1){
		float sl = get_distance(-1);
		float sm = get_distance(0);
		float sr = get_distance(1);
		//wenn float rumspinnt vielleicht auf integer wechseln, zwar weniger genau, aber Aufl?sung in cm sollte eigentlich auch reichen
				
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
		*/
	
	
}



void init(void){
	//LED-Dir setzen und System Timer Control enablen
	LPC_GPIO1->DIR |= (1<<10);
	
	//Buttons f?r Steuerung initialisieren
	LPC_GPIO2->DIR &= ~((1<<7) | (1<<8));
	
	
	input_init();
	output_init();
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = 0x98967F; //250 ms Intervall
	
	
}

void presentation(){
	presentation_index = MENU;
	int pfeil_index = 1;
	int slide = 1;
	
	float sleft;
	float smid;
	float sright;
	
	while(1){
		switch (presentation_index){
			case MENU:
				write_text("LC Display Control", 1, 3);
				write_text("Ultraschallsensoren", 3, 3);
				write_text("Kompass + UART", 5, 3);
				write_text("Motor Control", 7, 3);
				write_text("Odometer / Radencoder", 9, 3);
				
				
			
				while((LPC_GPIO2->PIN & (1<<7)) == (1<<7)){//Enter noch nicht gedr?ckt
					if ((LPC_GPIO2->PIN & (1<<8)) != (1<<8)){
						write_text(" ", pfeil_index, 1);
						pfeil_index ++;
						pfeil_index %= 10;
						
						pfeil_index ++;						
						delayms(400);
					}				
					if (get_togglee()){
						write_text(">", pfeil_index, 1);
					}else{
						write_text(" ", pfeil_index, 1);
					}						
				}
				//presentation_index anpassen			
				presentation_index = pfeil_index;
				slide = 1;
				clear_display();
				delayms(400);
			break;
			case LEVEL4:				
				if(slide == 1){
					//Slide 1: String
					write_text("Ausgabe eines String:", 1, 1);
					delayms(500);					
					write_text("Hello Raab", 6, 10);	
					
				}else if(slide == 2){
					write_text("Ausgabe eines Bitmaps:", 1, 1);
					delayms(500);	
					draw_hs_logo();
					
					
				}else if(slide == 3){
					write_text("Animation:", 1, 1);
					delayms(500);
					play_animation();
				}
				else if(slide == 4){
					write_text("Pixel setzen und aufheben:", 1, 1);
					delayms(500);
					draw_pixel(100,100); delayms(100); draw_pixel(101,101); delayms(100); draw_pixel(102,102); 
					delayms(1000);
					clear_pixel(100,100); delayms(100); clear_pixel(101,101); delayms(100); clear_pixel(102,102); 					
				}
				else if(slide == 5){
					write_text("Rechtecke zeichen:", 1, 1);
					delayms(500);
					draw_rectangle(20, 50, 100, 50)	; delayms(100);
					fill_rectangle(140, 15, 50, 100);
				}
				else if(slide == 6){
					write_text("Kreise und Ellipsen zeichen:", 1, 1);
					delayms(500);
					draw_circle(20, 70, 30); delayms(100); draw_ellipse(120, 80, 200, 60, 100);
					delayms(1000);
					fill_circle(20, 70, 30); delayms(100); fill_ellipse(120, 80, 200, 60, 100);
				}	
				while((LPC_GPIO2->PIN & (1<<7)) == (1<<7) && (LPC_GPIO2->PIN & (1<<8)) == (1<<8)){}//warte auf einen der Taster
					if ((LPC_GPIO2->PIN & (1<<7)) != (1<<7)){//enter wurde gedr?ckt
						slide++;
						if (slide > 6){//6 Slides bei Level 4
							presentation_index = MENU;
							pfeil_index = 3;
						}
				}
				clear_display();	
				delayms(200);
			break;
				
			case LEVEL5:
					clear_display();	
					write_text("Darstellung der Distanzen:", 1, 1);
				
					while(1){
						sleft = get_distance(-1);
						smid = get_distance(0);
						sright = get_distance(1);
				
						char s1[5];
						char s2[5];
						char s3[5];
						
						sprintf(s1, "%3.2f", sleft);
						write_text(s1, 5, 2);
						
						sprintf(s2, "%3.2f", smid);
						write_text(s2, 5, 11);
						
						sprintf(s3, "%3.2f", sright);
						write_text(s3, 5, 23);
					
						plot_distance(190, 50, ((int)(sleft/2.0f))); // linker Sensor
						plot_distance(100, 50, ((int)(smid/2.0f))); // mittlerer Sensor
						plot_distance(20, 50, ((int)(sright/2.0f))); // rechter Sensor
						
						
						if ((LPC_GPIO2->PIN & (1<<7)) != (1<<7)){//Enter-Taste gedr?ckt
							break;
						}
						
					}
			
					presentation_index = MENU;
					pfeil_index = 5;
					clear_display();	
					delayms(200);						
			break;
			case LEVEL6:
				
			
			
					clear_display();	
					write_text("Darstellung der Orientierung:", 1, 1);

					while(1){
						
						
						get_orientation();
						float heading= (float)get_heading();
						int pitch= (int)get_pitch();
						int roll= (int)get_roll();
						
						

						char head_string[6];
						char pitch_string[5];
						char roll_string[5];
						
						sprintf(head_string, "%1.1f",heading);
						write_text(head_string, 5, 11);
						
						sprintf(pitch_string, "%d", pitch);
						write_text(pitch_string, 7, 11);
						
						sprintf(roll_string, "%d", roll);
						write_text(roll_string, 9, 11);
					
						
						
						if ((LPC_GPIO2->PIN & (1<<7)) != (1<<7)){//Enter-Taste gedr?ckt
							break;
						}
						
					}
			
					presentation_index = MENU;
					pfeil_index = 7;
					clear_display();	
					delayms(200);						
		
			break;
			case LEVEL7:
				if(slide == 1){
					write_text("Drive forward", 1, 1);
					delayms(500);					
					drive(0);					
				}				
				else if(slide == 2){
					write_text("Drive backward", 1, 1);
					delayms(500);					
					drive(1);					
				}
				else if(slide == 3){
					write_text("Turn centered clockwise", 1, 1);
					delayms(500);					
					turn_centered(1);				
				}
				else if(slide == 4){
					write_text("Turn centered counterclockwise", 1, 1);
					delayms(500);					
					turn_centered(0);				
				}
				else if(slide == 5){
					write_text("Turn off-centered clockwise", 1, 1);
					delayms(500);					
					turn_off_centered(1);				
				}
				else if(slide == 6){
					write_text("Turn off-entered counterclockwise", 1, 1);
					delayms(500);					
					turn_off_centered(0);				
				}
				
				while((LPC_GPIO2->PIN & (1<<7)) == (1<<7) && (LPC_GPIO2->PIN & (1<<8)) == (1<<8)){}//warte auf einen der Taster
					if ((LPC_GPIO2->PIN & (1<<7)) != (1<<7)){//enter wurde gedr?ckt
						slide++;
						if (slide > 6){//6 Slides bei Level 7
							presentation_index = MENU;
							pfeil_index = 9;
						}
				}
				clear_display();	
				stop_motors();
				delayms(200);
			break;
			case LEVEL8:
				
				if(slide == 1){
					write_text("Drive forward(Distance) 30cm", 1, 1);
					delayms(500);					
					drive_distance(0, 30);			
				}
				else if(slide == 2){
					write_text("Drive backward(Distance) 30cm", 1, 1);
					delayms(500);					
					drive_distance(1, 30);			
				}
				else if(slide == 3){
					write_text("Drive forward(Degree) 360deg", 1, 1);
					delayms(500);					
					drive_degree(0, 360 * 200);					
				}
				else if(slide == 4){
					write_text("Drive backward(Degree) 360deg", 1, 1);
					delayms(500);					
					drive_degree(1, 360);				
				}
				
				while((LPC_GPIO2->PIN & (1<<7)) == (1<<7) && (LPC_GPIO2->PIN & (1<<8)) == (1<<8)){}//warte auf einen der Taster
					if ((LPC_GPIO2->PIN & (1<<7)) != (1<<7)){//enter wurde gedr?ckt
						slide++;
						if (slide > 4){//4 Slides bei Level 8 abbruch function
							presentation_index = MENU;
							pfeil_index = 9;
						}
				}
				clear_display();	
				stop_motors();
				delayms(200);
			
				
				
			break;
	
	
	
	
	
		}
	}
}


