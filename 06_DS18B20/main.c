#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit DB = P1^4;
uchar TPH = 0;
uchar TPL = 0;
uint temp = 0;

uchar code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar code select[] = {0x01,0x02,0x04,0x08};

void Delay500us()		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 6;
	j = 93;
	do
	{
		while (--j);
	} while (--i);
}

void Delay2us()		//@11.0592MHz
{
	unsigned char i;

	i = 3;
	while (--i);
}

void Delay600us()		//@11.0592MHz
{
	unsigned char i, j;
	i = 7;
	j = 113;
	do
	{
		while (--j);
	} while (--i);
}

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

void Delay420us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 5;
	j = 129;
	do
	{
		while (--j);
	} while (--i);
}

void Delay60us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 1;
	j = 162;
	do
	{
		while (--j);
	} while (--i);
}


void Delay480us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 6;
	j = 38;
	do
	{
		while (--j);
	} while (--i);
}

void Delay50us()		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();
	i = 1;
	j = 134;
	do
	{
		while (--j);
	} while (--i);
}

//void Delay390us()		//@11.0592MHz
//{
//	unsigned char i, j;
//	_nop_();
//	i = 5;
//	j = 46;
//	do
//	{
//		while (--j);
//	} while (--i);
//}


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
	Delay600us();
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0xFF;
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0x00;
}
	
void DS18B20_Init()
{
	CY = 1;
	while(CY)
	{
		DB = 0;
		Delay480us();
		DB = 1;
		Delay60us();
		CY = DB;
		Delay420us();
	}
}

void DS18B20_Write_Byte(uchar byte_data)
{
	uchar i;
	for(i=0; i<8; i++)
	{
		DB = 0;
		Delay10us();
		DB = byte_data & 0x01;		// 0000000x
		Delay60us();
		DB = 1;
		Delay10us();
		byte_data >>= 1;
	}
}

uchar DS18B20_Read_Byte()
{
	uchar i;
	uchar byte_data = 0;
	for(i=0; i<8; i++)
	{
		byte_data >>= 1;
		DB = 0;
		Delay2us();
		DB = 1;
		Delay10us();
		if(DB)
		byte_data |= 0x80;
		Delay50us();
	}
	return byte_data;
}

void DS18B20_FUNCTION()
{
	DS18B20_Init();
	DS18B20_Write_Byte(0xCC);	// Skip ROM
	DS18B20_Write_Byte(0x44);	// Convert T
	while(!DB);
	DS18B20_Init();
	DS18B20_Write_Byte(0xCC);	// Skip ROM
	DS18B20_Write_Byte(0xBE);	// Read Scratchpad
	TPL = DS18B20_Read_Byte();
	TPH = DS18B20_Read_Byte();
	temp = ((TPH << 8) + TPL) * 6.25;
}

void main()
{
	System_Init();
	while(1)
	{
		DS18B20_FUNCTION();
		Delay500us();
		{
			LED_Display(0, temp/1000);
			LED_Display(1, (temp/100)%10);
			LED_Display(2, (temp%100)/10);
			LED_Display(3, temp%10);
		}
	}
}