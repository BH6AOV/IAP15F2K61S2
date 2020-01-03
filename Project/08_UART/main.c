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
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��1ʱ��ΪFosc/12,��12T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xE8;			//�趨��ʱ��ֵ
	TH1 = 0xFF;			//�趨��ʱ��ֵ
	ET1 = 0;			//��ֹ��ʱ��1�ж�
	TR1 = 1;			//������ʱ��1
}

/**********************************************************
*  �������ƣ�����1�жϷ�����
*  ���ڣ�2019-11-21
*  ������ZhangHJ
*  ˵����1. ��������
*		 2. ������ձ�־λ
*		 3. ���ͽ��յ�������
*		 4. �ȴ��������
*		 5. ������ͱ�־λ
***********************************************************/
void UartService() interrupt 4
{
	uchar temp;
	temp = SBUF;	// ��������
	RI = 0;			// ��������жϱ�־λ
	show_data = temp;
//	SBUF = show_data;
//	while(!TI);		// �ȴ��������
//	TI = 0;			// ��������жϱ�־λ
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