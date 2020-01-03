#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define Hex2Char(x) ((x/10)*10+(x%10)-48)
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
uchar show_data;
uchar tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void System_Init()
{
	HC138_A = 1;
	HC138_B = 0;
	HC138_C = 1;
	WR = 0;
	P0 = 0x00;
	HC138_A = 0;
	HC138_B = 0;
	HC138_C = 1;
	WR = 0;
	P0 = 0xFF;
}

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;			//设定定时初值
	TH1 = 0xFF;			//设定定时初值
	ET1 = 0;			//禁止定时器1中断
	TR1 = 1;			//启动定时器1
}

/**********************************************************
*  函数名称：串口1中断服务函数
*  日期：2019-11-21
*  姓名：ZhangHJ
*  说明：1. 接收数据
*		 2. 清除接收标志位
*		 3. 发送接收到的数据
*		 4. 等待发送完成
*		 5. 清除发送标志位
***********************************************************/
void UartService() interrupt 4
{
	uchar temp;
	temp = SBUF;	// 接收数据
	RI = 0;			// 清除接收中断标志位
	show_data = temp;
//	SBUF = show_data;
//	while(!TI);		// 等待发送完成
//	TI = 0;			// 清除发送中断标志位
}

void LED_Init()
{
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0xff;
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
}

void main()
{
	System_Init();
	UartInit();
	LED_Init();
	while(1)
	{
		P0 = tab[Hex2Char(show_data)];
		Delay20ms();
	}
}