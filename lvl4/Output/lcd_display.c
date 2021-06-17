#include <LPC177x_8x.h>


#define data ((uint8_t *) 0x80004000)
#define command ((uint8_t *) 0x80004004)
#define read_address ((uint8_t *) 0x80004004)


_Bool check_status(void){
		uint8_t status = *read_address;
		status &= 0x3;
		if (status == 3){
			return 1;
		}else{
			return 0;
		}
}

void send_command_0(uint8_t code){
		while (!check_status()){}
		*command = code;
}
void send_command_1(uint8_t code, uint8_t Data){
		while (!check_status()){}	
		*data = Data;	
		while (!check_status()){}
		*command = code;
}
void send_command_2(uint8_t code, uint16_t Data){
		uint8_t D1 = Data & 0x00FF;
		uint8_t D2 = Data >> 0x8;
		while (!check_status()){}	
		*data = D1;
		while (!check_status()){}	
		*data = D2;	
		while (!check_status()){}
		*command = code;
}



void write_text(char  string[], int line, int column)
	{ 
	uint16_t address = line*30 + column;
	
  // Set Address Pointer 
	send_command_2(0x24, address);		
		
	while(*string)
	{ 
		char c = (*string++);
		send_command_1(0xc0,c-32);
	}
}

void clear_display(){
	//Text Speicher clearen
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 30; j++){
			write_text(" ", i, j);
		}
	}
	//Graphical Speicher clearen
	
}

void display_init(){
	//EMC Init
	//Pins Konfigurieren
	LPC_IOCON->P3_0 |= 1; //Data Bus	0
	LPC_IOCON->P3_1 |= 1; //Data Bus	1
	LPC_IOCON->P3_2 |= 1; //Data Bus	2
	LPC_IOCON->P3_3 |= 1; //Data Bus	3
	LPC_IOCON->P3_4 |= 1; //Data Bus	4
	LPC_IOCON->P3_5 |= 1; //Data Bus	5
	LPC_IOCON->P3_6 |= 1; //Data Bus	6
	LPC_IOCON->P3_7 |= 1; //Data Bus	7
	
	LPC_IOCON->P4_0 |= 1; //Address Bus	0
	LPC_IOCON->P4_1 |= 1; //Address Bus	1
	LPC_IOCON->P4_2 |= 1; //Address Bus	2
	LPC_IOCON->P4_3 |= 1; //Address Bus	3
	LPC_IOCON->P4_4 |= 1; //Address Bus	4
	LPC_IOCON->P4_5 |= 1; //Address Bus	5
	LPC_IOCON->P4_6 |= 1; //Address Bus	6
	LPC_IOCON->P4_7 |= 1; //Address Bus	7
	LPC_IOCON->P4_8 |= 1; //Address Bus	8
	LPC_IOCON->P4_9 |= 1; //Address Bus	9
	LPC_IOCON->P4_10 |= 1; //Address Bus	10
	LPC_IOCON->P4_11 |= 1; //Address Bus	11
	LPC_IOCON->P4_12 |= 1; //Address Bus	12
	LPC_IOCON->P4_13 |= 1; //Address Bus	13
	LPC_IOCON->P4_14 |= 1; //Address Bus	14
	LPC_IOCON->P4_15 |= 1; //Address Bus	15
	
	LPC_IOCON->P4_24 |= 1; //Output Enable
	LPC_IOCON->P4_25 |= 1; //Write Enable
	LPC_IOCON->P4_30 |= 1; //Chip Select	

	//Power Supply für EMC aktivieren
	LPC_SC->PCONP |= (1<<11); //External Memory Controler - Power On
	
	//Konfiguratioin des EMC
	LPC_EMC->Control = 1; //EMC Enablen
	LPC_EMC->StaticConfig0 = 0x80; //Static Memory Configuration EMC
	
	//LCD-Controller Init
	//Set Home Address
	send_command_2(0x40, 0x000);
	//Set Text Area to 30
	send_command_2(0x41, 30);
	//Set Graphic Home Address
	send_command_2(0x42, 0x200);
	//Set Graphic Area to 30
	send_command_2(0x43, 30);
	
	//Activate EXOR Mode
	send_command_0(0x81);
	//Set Display Mode for Text and Graphics to ON
	send_command_0(0x94); //9F
	
	clear_display();
}


