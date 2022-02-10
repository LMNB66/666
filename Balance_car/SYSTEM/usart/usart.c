#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"	  
	 

#if SYSTEM_SUPPORT_OS
#include "includes.h"  
#endif

  
#if 1
#pragma import(__use_no_semihosting)             
                
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
   
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){};  
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX
 	
u8 USART_RX_BUF[USART_REC_LEN];

u16 USART_RX_STA=0;  
  
void uart_init(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);



	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

}

//////////////////////////////////////////////////////////////////////////////////////////////////

#define  STR_LIMIT  str[i]!='K' && str[i]!=':' && str[i]!='p' && str[i]!='\0' && str[i]!='i' && str[i]!='d' && str[i]!='t' && str[i]!='m' && str[i]!='X' && str[i]!='Y'

float pid_control[pid_num]={0};		//串口调参


u8 Str_sta[50]={0};					//保存字符串

//====================================================
//接收字符串
//====================================================
int Usart_str(u8 Res, u8 *str)
{
	static u8 i=0;
	str[i]=Res;
	i++;
	if(Res=='#')
	{
		i=0;
		return 1;
	}
	return 0;
}

//====================================================
//解码字符串
//格式如  Kp:0,Ki:0#   以#为结尾
//====================================================
void Str_dec(u8 *str, float *fnum)
{
	u8 i, j=0, k=0, tmp[10]={0};
	for(i=0;i<strlen((char *)str);i++)
	{
		if(STR_LIMIT)
		{
			
			if(str[i]==',' || str[i]=='#')
			{
				fnum[k++]=strtod((char *)tmp, NULL);
				memset((char *)tmp, 0, sizeof tmp);
				j=0;
			}
			else
			{				
				tmp[j]=str[i];
				j++;
			}
		}
	}
}

void USART1_IRQHandler(void)
{
	u8 Res;
#if SYSTEM_SUPPORT_OS
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		{
		Res =USART_ReceiveData(USART1);
		
/////////////////////////////////////////////////////////////////////////		

		//接收字符串并解码
		if(Usart_str(Res, Str_sta))
		{
			Str_dec(Str_sta, pid_control);
			printf("%s\r\n", Str_sta);
		}
		
/////////////////////////////////////////////////////////////////////////
		
		if((USART_RX_STA&0x8000)==0)
			{
			if(USART_RX_STA&0x4000)
				{
				if(Res!=0x0a)USART_RX_STA=0;
				else USART_RX_STA|=0x8000;
				}
			else
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS
	OSIntExit();  											 
#endif
} 
#endif	
