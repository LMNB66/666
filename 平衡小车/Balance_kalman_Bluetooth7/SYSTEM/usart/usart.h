#ifndef __USART_H
#define __USART_H
	
#include "sys.h" 

#define USART_REC_LEN  			200
#define EN_USART1_RX 			1
  	
extern u8  USART_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;	


#define pid_num 2						//调参为4，遥控为2

extern float pid_control[pid_num];		//调参或遥控保存数据

void uart_init(u32 bound);

#endif
