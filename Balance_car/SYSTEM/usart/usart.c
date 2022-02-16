#include "usart.h"	
#include "stdio.h"
#include "stdlib.h"
#include "string.h" 
#include "pid.h" 
	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"	//ucos 使用	  
#endif	  

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting) 
           
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout; 
      
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
}
 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){};	//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
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
 
#if EN_USART1_RX   //如果使能了接收

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       			//接收状态标记	  
  
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	USART_Init(USART1, &USART_InitStructure); 		//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    	//使能串口1 
}

void USART1_IRQHandler(void)	//串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  				//接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);									//读取接收到的数据
		
		/////////////////////
		USART_control(Res);//中断运行
		/////////////////////
		
		if((USART_RX_STA&0x8000)==0)									//接收未完成
		{
			if(USART_RX_STA&0x4000)										//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;							//接收错误,重新开始
				else USART_RX_STA|=0x8000;								//接收完成了 
			}
			else 														//还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;	//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
	} 

#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
 
#endif		

//====================================================
//串口接收控制，接收字符串，格式以p\i\d\t\x\y开头+数字
//最后以#结尾，只能收发一组，例如  p125#，得到125
//====================================================
void USART_control(u8 Res)
{
	static u8 sta=7, i=0;
	static char str[10];	//保存接收到字符串里面的数字
	if(sta==7)
	{
		
		switch(Res)
		{
			case 'p' : sta=0; break;	//Kp
			case 'i' : sta=1; break;	//Ki
			case 'd' : sta=2; break;	//Kd
			case 't' : sta=3; break;	//Ktp
			case 'o' : sta=4; break;	//theta_offset
			case 'x' : sta=5; break;	//mX
			case 'y' : sta=6; break;	//mY
			default  : sta=7; break;
		}
	}
	else
	{
		if(Res!='#')
		{
			str[i++]=Res;
		}
		else
		{
			//串口调参，遥控
			switch (sta)
			{
				case 0:			Kp	= atof(str);break;
				case 1:			Ki	= atof(str);break;
				case 2:			Kd	= atof(str);break;
				case 3:			Ktp	= atof(str);break;
				case 4:theta_offset	= atof(str);break;
				case 5:			mX	= atof(str);break;
				case 6:			mY	= atof(str);break;
			}
			printf("%s\r\n", str);
			memset(str, 0, sizeof str);				//清空字符串
			sta=7;
			i=0;
		}
	}
}
