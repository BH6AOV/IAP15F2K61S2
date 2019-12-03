#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define FSOC 11059200L
#define CYCLE (12000000.0/FSOC)
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit TRIG=P1^0;		// Send
sbit ECHO=P1^1;		// Recive
uchar code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x40,0x79,0x24,0x30,0x19,0x12,0x12,0x78,0x00,0x12,0x9C,0xC6};
uchar code select[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uint distence = 0;
//#define somenop{_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

void Delay10us()		//@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 25;
	while (--i);
}

void Delay2ms()		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 22;
	j = 128;
	do
	{
		while (--j);
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

void LED_Display(uchar selection, uchar segment)
{
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0x00;
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = tab[segment];
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = select[selection];
	Delay2ms();
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0xFF;
}

void Send_Info()
{
	uint i;
	for(i=0; i<8; i++)
	{
		TRIG = 1;
		Delay10us();
		TRIG = 0;
		Delay10us();
	}
}

void Timer1_Init(void)
{
	AUXR &= 0xBF;	// 定时器T1 12分频
	TMOD &= 0x0F;	// 定时器T1 方式1
	TL1 = 0x00;
	TH1 = 0x00;
	TF1 = 0;
}


void main()
{
	uint temp,i;
	System_Init();
	Timer1_Init();
	while(1)
	{
		Send_Info();
		TR1 = 1;
		while((ECHO)&&(!TF1));
		TR1 = 0;
		if(TF1)
		{
			TF1 = 0;
			distence = 999;
		}
		else
		{
			temp = ((TH1 << 8) | TL1);
			distence = temp * CYCLE * 0.017;
			TL1 = 0x00;	
			TH1 = 0x00;
		}
		distence = (uint)(distence*10);
		for(i=0; i<50g; i++)
		{
			LED_Display(4, distence/1000);
			LED_Display(5, distence/100%10);
			LED_Display(6, distence%100/10+10);
			LED_Display(7, distence%10);
		}
	}
}