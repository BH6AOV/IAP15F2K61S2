#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit HC138_A = P2^5;	// 74HC138��������� A
sbit HC138_B = P2^6;	// 74HC138��������� B
sbit HC138_C = P2^7;	// 74HC138��������� C

/**********************************************************
*  �������ƣ�100������ʱ����(11.0592MHz)
*  ���ڣ�2019-11-29
*  ������ZhangHJ
*  ˵������STC-ISP�����Զ�����
***********************************************************/
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	_nop_();
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


/**********************************************************
*  �������ƣ�ϵͳ��ʼ������
*  ���ڣ�2019-11-29
*  ������ZhangHJ
*  ˵�������ڹرռ̵����ͷ�����
*		 1. 74HC138����101 --> Y5=0 --> CY5=1
*		 2. 74HC573�����뾭��ULN2003ȡ����,�رռ̵����ͷ�����
***********************************************************/
void System_Init()
{
	HC138_C = 1;
	HC138_B = 0;
	HC138_A = 1;	// Y5=0 --> CY5 =1
	P0 = 0x00;		// �رռ̵����ͷ�����
}


/**********************************************************
*  �������ƣ�LED��ʼ������
*  ���ڣ�2019-11-29
*  ������ZhangHJ
*  ˵������������LED
*		 1. 74HC138����100 --> Y4=0 --> CY4=1
*		 2. WR���Y4 --> CY4
***********************************************************/
void LED_config()
{
	HC138_C = 1;
	HC138_B = 0;
	HC138_A = 0;
	WR = 0;
}


// ���������ܣ��رշ������ͼ̵���,LEDѭ������
void main()
{
	System_Init();
	LED_config();
	P0 = 0xFE;
	while(1)
	{
		Delay100ms();
		P0 = _crol_(P0, 1);
	}
}