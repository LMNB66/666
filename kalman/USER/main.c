#include "delay.h"
#include "usart.h"
#include "mpu6050.h"
#include "kalman.h"
#include "led.h"
#include "exti.h"
#include "inv_mpu.h"

void usart1_send_char(u8 c)
{
    while((USART1->SR&0X40)==0);//等待上一次发送完毕  
    USART1->DR=c;   
}

//匿名四轴上位机发送协议
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
    u8 send_buf[32];
    u8 i;
    if(len>28)return;    //最多28字节数据
    send_buf[len+3]=0;  //校验数置零
    send_buf[0]=0X88;   //帧头
    send_buf[1]=fun;    //功能字
    send_buf[2]=len;    //数据长度
    for(i=0;i<len;i++)send_buf[3+i]=data[i];         //复制数据
    for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];    //计算校验和
    for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);   //发送数据到串口1
}

//发送浮点数
void send_mpu6050_data(u8 a, float theta)
{
    unsigned char buf[4];
	
	unsigned char *p;
    p=(unsigned char *)&theta;
    buf[0]=(unsigned char)(*(p+3));
    buf[1]=(unsigned char)(*(p+2));
    buf[2]=(unsigned char)(*(p+1));
    buf[3]=(unsigned char)(*(p+0));

    usart1_niming_report(a, buf, 4);
}

int main(void)
{
	delay_init();
	uart_init(115200);
	Led_Init();
//	led=0;
	MPU_Init();
	mpu_dmp_init();
	kalman_init();
	MPU6050_EXTI_Init();
//	led=1;
	while(1)
	{
		send_mpu6050_data(0xa1, theta_deg2);
		send_mpu6050_data(0xa2, roll);
	}
}
