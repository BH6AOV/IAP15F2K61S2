#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;

sfr P4 = 0xC0;
sbit col_1 = P4^4;
sbit col_2 = P4^2;
sbit col_3 = P3^5;
sbit col_4 = P3^4;
sbit row_1 = P3^0;
sbit row_2 = P3^1;
sbit row_3 = P3^2;
sbit row_4 = P3^3;

uchar Key_Number = 0;

// 0~F
uchar code segment[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};

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

void DisplayNum(uchar number)
{
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0xFF;
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = segment[number];
}

void Matrix_Key()
{
	// Row One
	row_1 = 0;
	row_2 = row_3 = row_4 = 1;
	col_1 = col_2 = col_3 = col_4 = 1;
	if(col_1 == 0)
	{
		Key_Number = 0;
	}
	else if(col_2 == 0)
	{
		Key_Number = 1;
	}
	else if(col_3 == 0)
	{
		Key_Number = 2;
	}
	else if(col_4 == 0)
	{
		Key_Number = 3;
	}
	
	// Row two
	row_2 = 0;
	row_1 = row_3 = row_4 = 1;
	col_1 = col_2 = col_3 = col_4 = 1;
	if(col_1 == 0)
	{
		Key_Number = 4;
	}
	else if(col_2 == 0)
	{
		Key_Number = 5;
	}
	else if(col_3 == 0)
	{
		Key_Number = 6;
	}
	else if(col_4 == 0)
	{
		Key_Number = 7;
	}
	
	// Row three
	row_3 = 0;
	row_1 = row_2 = row_4 = 1;
	col_1 = col_2 = col_3 = col_4 = 1;
	if(col_1 == 0)
	{
		Key_Number = 8;
	}
	else if(col_2 == 0)
	{
		Key_Number = 9;
	}
	else if(col_3 == 0)
	{
		Key_Number = 10;
	}
	else if(col_4 == 0)
	{
		Key_Number = 11;
	}
	
	// Row four
	row_4 = 0;
	row_1 = row_2 = row_3 = 1;
	col_1 = col_2 = col_3 = col_4 = 1;
	if(col_1 == 0)
	{
		Key_Number = 12;
	}
	else if(col_2 == 0)
	{
		Key_Number = 13;
	}
	else if(col_3 == 0)
	{
		Key_Number = 14;
	}
	else if(col_4 == 0)
	{
		Key_Number = 15;
	}
	
	DisplayNum(Key_Number);
}


void main()
{
	System_Init();
	while(1)
	{
		Matrix_Key();
		Delay2ms();
	}
}


