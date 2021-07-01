#include <LPC177x_8x.h>
#include <stdint.h>

#define TxD 0
#define RxD 1	
int heading = 0;
int8_t pitch =0;
int8_t roll =0;



void compass_init(void){
	//Uart1 im PCONP anschalten
	LPC_SC->PCONP |= (0x01 << 4);
	
	LPC_IOCON->P2_0 |= 2; //TX von UART 1
	LPC_IOCON->P2_1 |= 2; //RX von UART 1
	
	LPC_UART1->LCR |= 0x3; //8 Daten bit
	LPC_UART1->LCR &= ~(1 << 2); //stop bit
	LPC_UART1->LCR &= ~(1 << 3); //no parity bit
	
	LPC_UART1->LCR |= (1 << 7); // access to baudrate
	LPC_UART1->DLL = 195; //Baudrate to 9600 
	LPC_UART1->DLM = 0; //Baudrate to 9600
	LPC_UART1->LCR &= ~(1 << 7); //restrict access to baudrate settings

}
int get_heading(void)
{return heading;}

int8_t get_pitch(void)
{return pitch;}

int8_t get_roll(void)
{return roll;}

int get_orientation(void) {
	LPC_UART1->THR = 0x23;
	
	//wait for first byte big endian from compess
	while (!(LPC_UART1->LSR & 1)){}
	heading = LPC_UART1->RBR << 8;
	
	//wait for second byte big endian from compess
	while (!(LPC_UART1->LSR & 1)){}	
	heading |= LPC_UART1->RBR;
		
	//wait for pitch
	while (!(LPC_UART1->LSR & 1)){}
	pitch = LPC_UART1->RBR;
	
		
	//wait for roll	
	while (!(LPC_UART1->LSR & 1)){}
	roll = LPC_UART1->RBR;
	
	return 0;
}
