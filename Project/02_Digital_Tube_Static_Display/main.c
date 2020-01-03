#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;

// Digital tube segment from 0 to 9
uchar code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
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
	WR = 0;		// Reset WR  -->  Set Y5C
	P0 = 0x00;
	HC138_A = 0;
	HC138_B = 0;
	HC138_C = 1;
	WR = 0;		// Reset WR  -->  Set Y4C
	P0 = 0xFF;
}
void tube_selection()
{
	// Reset Y6
	HC138_A = 0;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;		// Reset WR  -->  Set Y6C
	// Digital tube selection
	P0 = 0xFF;
}

void tube_segment()
{
	// Reset Y7
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;		// Reset WR  -->  Set Y7C
}

void main()
{
	uint i;
	System_Init();
	tube_selection();
	tube_segment();
	while(1)
	{
		// Digital tube segment selection
		for(i=0; i<9; i++)
		{
			P0 = tab[i];
			Delay1000ms();
		}
	}
}