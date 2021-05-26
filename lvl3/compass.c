#include <LPC177x_8x.h>
#include <stdint.h>

#define TxD 0
#define RxD 1	



void compass_init(void){
	//Uart1 im PCONP anschalten
	LPC_SC->PCONP |= (0x01 << 4);
	
	
	

}
