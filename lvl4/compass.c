#include <LPC177x_8x.h>
#include <stdint.h>

#define TxD 0
#define RxD 1	



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
int get_direction(void) {
	int meas = 0;
	LPC_UART1->THR = 0x13;

	//wait for first byte big endian from compess
	while (!(LPC_UART1->LSR & 1)){}
	meas = LPC_UART1->RBR << 8;
	
	//wait for second byte big endian from compess
	while (!(LPC_UART1->LSR & 1)){}	
	meas |= LPC_UART1->RBR;
		
	return meas;
}
