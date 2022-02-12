#include "oled.h"
#include "stdlib.h"
#include "string.h" 
#include "stdio.h"	 
#include "delay.h"
#include "oledfont.h"


/*****************************************************************************
 * @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
 * @date       :2018-08-27 
 * @function   :Write a byte of data using STM32's hardware SPI
 * @parameters :SPIx: SPI type,x for 1,2,3
                Byte:Data to be written
 * @retvalue   :Data received by the bus
******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET); 
	SPIx->DR=Byte; 
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET); 
	return SPIx->DR;		
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
 * @date       :2018-08-09 
 * @function   :Set hardware SPI Speed
 * @parameters :SPIx: SPI type,x for 1,2,3
                SpeedSet:0-high speed
												 1-low speed
 * @retvalue   :None
******************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
	SPIx->CR1&=0XFFC7;
	if(SpeedSet==1)
	{
		SPIx->CR1|=SPI_BaudRatePrescaler_2;//Fsck=Fpclk/2	
	}
	else
	{
		SPIx->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	}
	SPIx->CR1|=1<<6;
} 

/*****************************************************************************
 * @name       :void SPI1_Init(void)	
 * @date       :2018-08-09 
 * @function   :Initialize the STM32 hardware SPI1
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void SPI1_Init(void)	
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
	   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);   
}
	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/	

static unsigned char OLED_buffer[1024] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*******************************************************************
 * @name       :void OLED_WR_Byte(unsigned dat,unsigned cmd)
 * @date       :2018-08-27
 * @function   :向OLED屏幕上写入一个字节的内容
 * @parameters :dat:Content to be written
                cmd:0-write command
								    1-write data
 * @retvalue   :None
********************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		OLED_DC_Set();
	}
	else
	{
		OLED_DC_Clr();
	}
	OLED_CS_Clr();
	SPI_WriteByte(SPI1,dat);
	OLED_CS_Set();
}

/*******************************************************************
 * @name       :void OLED_Set_Pos(unsigned char x, unsigned char y) 
 * @date       :2018-08-27
 * @function   :在OLED屏幕中设置坐标
 * @parameters :x:x coordinates
                y:y coordinates
 * @retvalue   :None
********************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y/PAGE_SIZE,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  

/*******************************************************************
 * @name       :void OLED_Display_On(void) 
 * @date       :2018-08-27
 * @function   :打开OLED显示器
 * @parameters :None
 * @retvalue   :None
********************************************************************/ 	  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/*******************************************************************
 * @name       :void OLED_Display_Off(void)
 * @date       :2018-08-27
 * @function   :关闭OLED显示器
 * @parameters :None
 * @retvalue   :None
********************************************************************/    
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

/*******************************************************************
 * @name       :void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
 * @date       :2018-08-27
 * @function   :将像素的值设置为RAM
 * @parameters :x:the x coordinates of pixel
                y:the y coordinates of pixel
								color:the color value of the point
								      1-white
											0-black
 * @retvalue   :None
********************************************************************/ 
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
{
	if(color)
	{
		OLED_buffer[(y/PAGE_SIZE)*WIDTH+x]|= (1<<(y%PAGE_SIZE))&0xff;
	}
	else
	{
		OLED_buffer[(y/PAGE_SIZE)*WIDTH+x]&= ~((1<<(y%PAGE_SIZE))&0xff);
	}
}		   			 

/*******************************************************************
 * @name       :void OLED_Display(void)
 * @date       :2018-08-27
 * @function   :在OLED屏幕中显示
 * @parameters :None
 * @retvalue   :None
********************************************************************/  
void OLED_Display(void)
{
	u8 i,n;		    
	for(i=0;i<PAGE_SIZE;i++)  
	{  
		OLED_WR_Byte (YLevel+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (XLevelL,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (XLevelH,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<WIDTH;n++)
		{
			OLED_WR_Byte(OLED_buffer[i*WIDTH+n],OLED_DATA); 
		}
	}   //更新显示
}

/*******************************************************************
 * @name       :void OLED_Clear(unsigned dat)  
 * @date       :2018-08-27
 * @function   :清除OLED屏幕
 * @parameters :dat:0-Display full black
                    1-Display full white
 * @retvalue   :None
********************************************************************/ 
void OLED_Clear(unsigned dat)  
{  
	if(dat)
	{
		memset(OLED_buffer,0xff,sizeof(OLED_buffer));
	}
	else
	{
		memset(OLED_buffer,0,sizeof(OLED_buffer));
	}
	OLED_Display();
}

/*******************************************************************
 * @name       :void OLED_Reset(void) 
 * @date       :2018-08-27
 * @function   :重置OLED屏幕
 * @parameters :dat:0-Display full black
                    1-Display full white
 * @retvalue   :None
********************************************************************/ 
void OLED_Reset(void)
{
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
}	
	
/*******************************************************************
 * @name       :void OLED_Init_GPIO(void)
 * @date       :2018-08-27
 * @function   :初始化gpio
 * @parameters :None
 * @retvalue   :None
********************************************************************/ 
void OLED_Init_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_1;	//GPIOB10,11,1 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB10、11、1
 	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_1);	
}

/*******************************************************************
 * @name       :void OLED_Init(void)
 * @date       :2018-08-27
 * @function   :初始化OLEDSH1106控制集成电路
 * @parameters :None
 * @retvalue   :None
********************************************************************/ 				    
void OLED_Init(void)
{
	SPI1_Init();  //初始化SPI1
 	OLED_Init_GPIO(); //初始化GPIO
 	delay_ms(200);
	OLED_Reset();     //复位OLED

/**************初始化SSD1306*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0xFF,OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0XF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); /*set charge pump disable*/ 
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
} 

/*******************************************************************
 * @name       :void GUI_DrawPoint(u8 x,u8 y,u8 color)
 * @date       :2018-08-27
 * @function   :在液晶显示器屏幕上画一个点
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
								color:the color value of the point
								      1-white
											0-black
 * @retvalue   :None
********************************************************************/
void GUI_DrawPoint(u8 x,u8 y,u8 color)
{
	OLED_Set_Pixel(x,y,color);
	OLED_Display();
}

/*******************************************************************
 * @name       :void GUI_Fill(u8 sx,u8 sy,u8 ex,u8 ey,u8 color)
 * @date       :2018-08-27 
 * @function   :填充指定区域
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the color value of the the specified area
								      1-white
											0-black
 * @retvalue   :None
********************************************************************/
void GUI_Fill(u8 sx,u8 sy,u8 ex,u8 ey,u8 color)
{  	
	u8 i,j;			
	u8 width=ex-sx+1; 		//得到填充的宽度
	u8 height=ey-sy+1;		//高度
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
				OLED_Set_Pixel(sx+j, sy+i,color);
		}		
	}
	OLED_Display();
}

/*******************************************************************
 * @name       :void GUI_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2,u8 color)
 * @date       :2018-08-27 
 * @function   :在两点之间画出一条线
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
								color:the color value of the line
								      1-white
											0-black
 * @retvalue   :None
********************************************************************/
void GUI_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2,u8 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{ 
		OLED_Set_Pixel(uRow,uCol,color);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
	OLED_Display();
} 

/*****************************************************************************
 * @name       :void GUI_DrawRectangle(u8 x1, u8 y1, u8 x2, u8 y2,u8 color)
 * @date       :2018-08-27 
 * @function   :绘制矩形
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
								color:the color value of the rectangle
								      1-white
											0-black							  
 * @retvalue   :None
******************************************************************************/
void GUI_DrawRectangle(u8 x1, u8 y1, u8 x2, u8 y2,u8 color)
{
	GUI_DrawLine(x1,y1,x2,y1,color);
	GUI_DrawLine(x1,y1,x1,y2,color);
	GUI_DrawLine(x1,y2,x2,y2,color);
	GUI_DrawLine(x2,y1,x2,y2,color);
}  

/*****************************************************************************
 * @name       :void GUI_FillRectangle(u8 x1, u8 y1, u8 x2, u8 y2,u8 color)
 * @date       :2018-08-27
 * @function   :填充矩形
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
								x2:the ending x coordinate of the filled rectangle
								y2:the ending y coordinate of the filled rectangle
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
******************************************************************************/  
void GUI_FillRectangle(u8 x1, u8 y1, u8 x2, u8 y2,u8 color)
{
	GUI_Fill(x1,y1,x2,y2,color);
}
 
/*****************************************************************************
 * @name       :static void _draw_circle_8(u8 xc, u8 yc, u8 x, u8 y, u8 color)
 * @date       :2018-08-27 
 * @function   :8个对称圆圈绘制算法（内部调用）
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								x:the x coordinate relative to the Circular center 
								y:the y coordinate relative to the Circular center 
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
******************************************************************************/  
static void _draw_circle_8(u8 xc, u8 yc, u8 x, u8 y, u8 color)
{
	OLED_Set_Pixel(xc + x, yc + y, color);
	OLED_Set_Pixel(xc - x, yc + y, color);
	OLED_Set_Pixel(xc + x, yc - y, color);
	OLED_Set_Pixel(xc - x, yc - y, color);
	OLED_Set_Pixel(xc + y, yc + x, color);
	OLED_Set_Pixel(xc - y, yc + x, color);
	OLED_Set_Pixel(xc + y, yc - x, color);
	OLED_Set_Pixel(xc - y, yc - x, color);
}

/*****************************************************************************
 * @name       :void GUI_DrawCircle(u8 xc, u8 yc, u8 color, u8 r)
 * @date       :2018-08-27
 * @function   :在指定的位置上绘制一个指定大小的圆圈
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
******************************************************************************/  
void GUI_DrawCircle(u8 xc, u8 yc, u8 color, u8 r)
{
	int x = 0, y = r,d;
	d = 3 - 2 * r;
	while (x <= y) 
	{
		_draw_circle_8(xc, yc, x, y, color);
		if (d < 0) 
		{
				d = d + 4 * x + 6;
		}
		else 
		{
				d = d + 4 * (x - y) + 10;
				y--;
		}
		x++;
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_FillCircle(u8 xc, u8 yc, u8 color, u8 r)
 * @date       :2018-08-27
 * @function   :在指定的位置填充一个指定大小的圆圈
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
******************************************************************************/  
void GUI_FillCircle(u8 xc, u8 yc, u8 color, u8 r)
{
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	while (x <= y) 
	{
			for (yi = x; yi <= y; yi++)
			{
				_draw_circle_8(xc, yc, x, yi, color);
			}
			if (d < 0) 
			{
				d = d + 4 * x + 6;
			} 
			else 
			{
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
	}
	OLED_Display();
}

/**********************************************************************************
 * @name       :void GUI_DrawTriangel(u8 x0,u8 y0,u8 x1,u8 y1,u8 x2,u8 y2,u8 color)
 * @date       :2018-08-27 
 * @function   :在指定的位置上绘制一个三角形
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
***********************************************************************************/ 
void GUI_DrawTriangel(u8 x0,u8 y0,u8 x1,u8 y1,u8 x2,u8 y2,u8 color)
{
	GUI_DrawLine(x0,y0,x1,y1,color);
	GUI_DrawLine(x1,y1,x2,y2,color);
	GUI_DrawLine(x2,y2,x0,y0,color);
}

/*****************************************************************************
 * @name       :static void _swap(u8 *a, u8 *b)
 * @date       :2018-08-27
 * @function   :交换两个号码（内部呼叫）
 * @parameters :a:the address of the first number 
                b:the address of the second number
 * @retvalue   :None
******************************************************************************/  
static void _swap(u8 *a, u8 *b)
{
	u16 tmp;
  tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :static void _draw_h_line(u8 x0,u8 x1,u8 y,u8 color)
 * @date       :2018-08-27
 * @function   :在RAM中绘制一条水平线（内部呼叫）
 * @parameters :x0:the bebinning x coordinate of the horizontal line
                x1:the ending x coordinate of the horizontal line
								y:the y coordinate of the horizontal line
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
******************************************************************************/
static void _draw_h_line(u8 x0,u8 x1,u8 y,u8 color)
{
	u8 i=0;
	for(i=x0;i<=x1;i++)
	{
		OLED_Set_Pixel(i, y, color);
	}
}

/*****************************************************************************
 * @name       :void GUI_FillTriangel(u8 x0,u8 y0,u8 x1,u8 y1,u8 x2,u8 y2,u8 color)
 * @date       :2018-08-27 
 * @function   :在指定的位置填充一个三角形
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
								color:the color value of the rectangle
								      1-white
											0-black	
 * @retvalue   :None
******************************************************************************/ 
void GUI_FillTriangel(u8 x0,u8 y0,u8 x1,u8 y1,u8 x2,u8 y2,u8 color)
{
	u8 a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) 
	{
    _swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
  }
	if(y0 == y2) 
	{ 
		a = b = x0;
		if(x1 < a)
    {
			a = x1;
    }
    else if(x1 > b)
    {
			b = x1;
    }
    if(x2 < a)
    {
			a = x2;
    }
		else if(x2 > b)
    {
			b = x2;
    }
		_draw_h_line(a,b,y0,color);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2)
	{
		last = y1; 
	}
  else
	{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) 
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b)
    {
			_swap(&a,&b);
		}
		_draw_h_line(a,b,y,color);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		_draw_h_line(a,b,y,color);
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示一个英语字符
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
								chr:the ascii code of display character(0~94)
								Char_Size:the size of display character(8,16)
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/ 
void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 mode)
{      	
	  unsigned char c=0,i=0,tmp,j=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>WIDTH-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			for(i=0;i<16;i++)
			{
			  if(mode)
				{
					tmp = F8X16[c*16+i];
				}
				else
				{
					tmp = ~(F8X16[c*16+i]);
				}
				for(j=0;j<8;j++)
				{
					if(tmp&(0x80>>j))
					{
						OLED_Set_Pixel(x+j, y+i,1);
					}
					else
					{
						OLED_Set_Pixel(x+j, y+i,0);
					}
				}
			}
		}
		else if(Char_Size==8)
		{	
				for(i=0;i<8;i++)
				{
				  if(mode)
					{
						tmp = F6x8[c][i];
					}
					else
					{
						tmp = ~(F6x8[c][i]);
					}
					for(j=0;j<8;j++)
					{
						if(tmp&(0x80>>j))
						{
							OLED_Set_Pixel(x+j, y+i,1);
						}
						else
						{
							OLED_Set_Pixel(x+j, y+i,0);
						}
					}
			}
	}
	else
	{
		return;
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示英文字符串
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
								chr:the start address of the English string
								Char_Size:the size of display character
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/   	  
void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 mode)
{
	unsigned char j=0,csize;
	if(Char_Size == 16)
  {
	  csize = Char_Size/2;
	}
  else if(Char_Size == 8)
  {
	  csize = Char_Size/2+2;
	}
	else
	{
		return;
	}
	while (chr[j]!='\0')
	{		
		GUI_ShowChar(x,y,chr[j],Char_Size,mode);
		x+=csize;
		if(x>120)
		{
			x=0;
			y+=Char_Size;
		}
		j++;
	}
}

/*****************************************************************************
 * @name       :u32 mypow(u8 m,u8 n)
 * @date       :2018-08-27 
 * @function   :获得m的第n次幂（内部呼叫）
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
******************************************************************************/ 
static u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/*****************************************************************************
 * @name       :void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示编号
 * @parameters :x:the bebinning x coordinate of the number
                y:the bebinning y coordinate of the number
								num:the number(0~4294967295)
								len:the length of the display number
								Size:the size of display number
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/  			 
void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0,csize;
  if(Size == 16)
  {
	  csize = Size/2;
	}
  else if(Size == 8)
  {
	  csize = Size/2+2;
	} 	
	else
	{
		return;
	}
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				GUI_ShowChar(x+csize*t,y,' ',Size,mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	GUI_ShowChar(x+csize*t,y,temp+'0',Size,mode); 
	}
} 

//====================================================
//显示浮点数
//====================================================
void GUI_ShowFloat(u8 x,u8 y,float num,u8 len,u8 Size,u8 mode)
{
	char str[25], sp[6];
	sprintf(sp, "%%0.%df", len);
	sprintf(str, sp, num);
	GUI_ShowString(x, y, (u8 *)str, Size, mode);
} 

/*****************************************************************************
 * @name       :void GUI_ShowFont16(u8 x,u8 y,u8 *s,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示一个16x16的汉字
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
								s:the start address of the Chinese character
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/ 
void GUI_ShowFont16(u8 x,u8 y,u8 *s,u8 mode)
{
	u8 i,j,k,tmp;
	u16 num;
	num = sizeof(cfont16)/sizeof(typFNT_GB16);
  for(i=0;i<num;i++)
	{
		if((cfont16[i].Index[0]==*s)&&(cfont16[i].Index[1]==*(s+1)))
		{
			for(j=0;j<32;j++)
			{
				if(mode)
				{
					tmp = cfont16[i].Msk[j];
				}
				else
				{
					tmp = ~(cfont16[i].Msk[j]);
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+(j%2)*8+k, y+j/2,1);
					}
					else
					{
						OLED_Set_Pixel(x+(j%2)*8+k, y+j/2,0);
					}
				}
			}	
			break;
		}	
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowFont24(u8 x,u8 y,u8 *s,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示一个24x24的汉字
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
								s:the start address of the Chinese character
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/ 
void GUI_ShowFont24(u8 x,u8 y,u8 *s,u8 mode)
{
	u8 i,j,k,tmp;
	u16 num;
	num = sizeof(cfont24)/sizeof(typFNT_GB24);
  for(i=0;i<num;i++)
	{
		if((cfont24[i].Index[0]==*s)&&(cfont24[i].Index[1]==*(s+1)))
		{
			for(j=0;j<72;j++)
			{
				if(mode)
				{
					tmp = cfont24[i].Msk[j];
				}
				else
				{
					tmp = ~(cfont24[i].Msk[j]);
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+(j%3)*8+k, y+j/3,1);
					}
					else
					{
						OLED_Set_Pixel(x+(j%3)*8+k, y+j/3,0);
					}
				}
			}	
			break;
		}	
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowFont32(u8 x,u8 y,u8 *s,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示一个32x32的汉字
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
								s:the start address of the Chinese character
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/ 
void GUI_ShowFont32(u8 x,u8 y,u8 *s,u8 mode)
{
	u8 i,j,k,tmp;
	u16 num;
	num = sizeof(cfont32)/sizeof(typFNT_GB32);
  for(i=0;i<num;i++)
	{
		if((cfont32[i].Index[0]==*s)&&(cfont32[i].Index[1]==*(s+1)))
		{
			for(j=0;j<128;j++)
			{
				if(mode)
				{
					tmp = cfont32[i].Msk[j];
				}
				else
				{
					tmp = ~(cfont32[i].Msk[j]);
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+(j%4)*8+k, y+j/4,1);
					}
					else
					{
						OLED_Set_Pixel(x+(j%4)*8+k, y+j/4,0);
					}
				}
			}	
			break;
		}	
	}
	OLED_Display();
}

/*****************************************************************************
 * @name       :void GUI_ShowCHinese(u8 x,u8 y,u8 hsize,u8 *str,u8 mode)
 * @date       :2018-08-27 
 * @function   :显示中文字符串
 * @parameters :x:the bebinning x coordinate of the Chinese strings
                y:the bebinning y coordinate of the Chinese strings
								size:the size of Chinese strings
								str:the start address of the Chinese strings
								mode:0-white background and black character
								     1-black background and white character
 * @retvalue   :None
******************************************************************************/	   		   
void GUI_ShowCHinese(u8 x,u8 y,u8 hsize,u8 *str,u8 mode)
{ 
	while(*str!='\0')
	{
		if(hsize == 16)
		{
			GUI_ShowFont16(x,y,str,mode);
		}
		else if(hsize == 24)
		{
			GUI_ShowFont24(x,y,str,mode);
		}
		else if(hsize == 32)
		{
			GUI_ShowFont32(x,y,str,mode);
		}
		else
		{
			return;
		}
		x+=hsize;
		if(x>WIDTH-hsize)
		{
			x=0;
			y+=hsize;
		}
		str+=2;
	}			
}

/*****************************************************************************
 * @name       :void GUI_DrawBMP(u8 x,u8 y,u8 width, u8 height, u8 BMP[], u8 mode)
 * @date       :2018-08-27 
 * @function   :显示一个BMP的单色图片
 * @parameters :x:the bebinning x coordinate of the BMP monochromatic picture
                y:the bebinning y coordinate of the BMP monochromatic picture
								width:the width of BMP monochromatic picture
								height:the height of BMP monochromatic picture
								BMP:the start address of BMP monochromatic picture array
								mode:0-white background and black character
								     1-black background and white character								
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawBMP(u8 x,u8 y,u8 width, u8 height, u8 BMP[], u8 mode)
{ 	
 u8 i,j,k;
 u8 tmp;
 for(i=0;i<height;i++)
 {
		for(j=0;j<(width+7)/8;j++)
		{
		    if(mode)
				{
					tmp = BMP[i*((width+7)/8)+j];
				}
				else
				{
					tmp = ~BMP[i*((width+7)/8)+j];
				}
				for(k=0;k<8;k++)
				{
					if(tmp&(0x80>>k))
					{
						OLED_Set_Pixel(x+j*8+k, y+i,1);
					}
					else
					{
						OLED_Set_Pixel(x+j*8+k, y+i,0);
					}
				}
		}
 } 
 OLED_Display();
} 
