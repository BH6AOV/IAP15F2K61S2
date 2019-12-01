#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define OFF_SEGMENT 10
#define OFF_SELECTION 0
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;

uchar code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
uchar code segment[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

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

void tube_segment(uint number)
{
	uchar segment;
	segment = tab[number];
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;			// Y7C
	P0 = segment;
}

void tube_selection(uchar selection)
{
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = selection;
}

void tube_show(uint number, uchar segment)
{
	tube_selection(OFF_SELECTION);
	tube_segment(number);
	tube_selection(segment);
	Delay600us();
	tube_segment(OFF_SEGMENT);
}

void main()
{
	uchar i;
	System_Init();
	while(1)
	{
		for(i=0; i<8; i++)
		{
			tube_show(i, segment[i]);
		}
	}
}