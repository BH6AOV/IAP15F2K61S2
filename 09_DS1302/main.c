#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define dec(dat) ((dat>>4)*10+(dat&0x0f)) 		// BCD��תʮ����
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
sbit RST = P1^3;
sbit IO = P2^3;
sbit SCLK = P1^7;

uchar code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
uchar code select[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
uchar write_addr[] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
uchar read_addr[] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
uchar Time[7] = {0x00,0x35,0x09,0x06,0x12,0x05,0x19};

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
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
	Delay1ms();
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0xff;
}

/**********************************************************
*  �������ƣ�DS1302д��������
*  ���ڣ�2019-10-4
*  ������ZhangHJ
*  ˵����д����������������,add��ʾҪд��ĵ�ַ;wdata��ʾҪд�������
*				 1. ��д������Ҫ�Ƚ�RST���߲��ܽ���
*				 2. ��д������ֽ�,��ʱ�������ش���д������
*				 3. ��д�������ֽ�,ͬ����ʱ��������д������
*				 4. �������RST��ֹ���ݴ���
***********************************************************/
void DS1302Write(uchar add,uchar wdata)
{
	uchar a;
	//wdata = hex(wdata);					// ת��ΪBCD��
	RST=0;									// ����RST����,��ֹ���ݴ���
	SCLK=0;									// ����SCLK����,����ʱ����
	RST=1;									// ����RST����,�������ݴ��䶼Ҫ����RST��,���������߼�
	//��д������ֽ�
	for(a=0; a<8; a++)
	{
			IO= add & 0x01;					// IO����׼����Ҫд���1λ����
			SCLK=1;							// SCLK������,1λ���ݴ�IO��д��,��λ��д��
			add>>=1;						// ��������1λ
			SCLK=0;							// ����SCLK,Ϊ�´�д��׼��,ѭ��8��д��1�ֽ�
	}
	//��д�������ֽ�
	for(a=0; a<8; a++)
	{
			IO= wdata & 0x01;
			SCLK=1;
			wdata>>=1;
			SCLK=0;
	}
	RST=0;									// ���ݴ���������RST
}  



/**********************************************************
*  �������ƣ�DS1302����������
*  ���ڣ�2019-10-4
*  ������ZhangHJ
*  ˵����������ֻ��һ������,����Ҫ��ȡ�ļĴ�����ַadd
*				 1. ������Ҫ����Ҫ��ȡ�ĵ�ַ(1Byte)д��Ĵ���,�������ؽ���д�����
*				 2. ֮���ȡ�õ�ַ�е�����,���½��ض�ȡ����
*				 3. ��󷵻ض�ȡ����1Byte����
***********************************************************/ 
uchar DS1302Read(uchar add)
{
	uchar a, rdata=0;
	RST = 0;											// ����RST����,��ֹ���ݴ���
	SCLK = 0;											// ����SCLK����,����ʱ����
	RST = 1;											// ����RST����,���������߼�
	//���Ϳ����ֽ�
	for(a=0; a<8; a++)
	{
		SCLK = 0;
		IO = add & 0x01;
		SCLK = 1;										// ����һ��������,д���ַ
		add >>= 1;
	}
	//��1�ֽ�����
	for(a=0; a<8; a++)
	{
		SCLK = 1;
		rdata >>= 1;
		SCLK = 0;										// ����һ���½���,��ȡ����
		if(IO)
		{												// �������1
			rdata |= 0x80;								// �����λ��Ϊ1,��¼��rdata��
		}
	}
	RST=0;												// ����RST
	SCLK=1;
	IO=0;
	IO=1;
	//return dec(d);        							// ��ȡ������ת����ʮ����
	return rdata;
}



/**********************************************************
*  �������ƣ�DS1302��ʼ������
*  ���ڣ�2019-10-4
*  ������ZhangHJ
*  ˵������ʼ����Ϊ��DS1302��ʼ���ݵ�д��
*				 1. ������Ҫ�ر�д����,����������д��
*				 2. ֮��"���ʱ��������"��˳��д���ʼ����
*				 3. �����д����,��ֹ����д��
***********************************************************/
void ds1302_init()
{
   uchar k;
   DS1302Write(0x8e,0x00);  				// ��ֹд����������������д��
	if(DS1302Read(0x81) & 0x80)				// ������,ʱ��ֹͣ��־λ
   for(k=0;k<7;k++)							// д��7���ֽڵ�ʱ���źţ����ʱ��������
   {
     DS1302Write(write_addr[k],Time[k]);
   }
   DS1302Write(0x8e,0x80);  				// ��д����,��ֹ����д��
}




/**********************************************************
*  �������ƣ�DS1302���ݶ�ȡ����
*  ���ڣ�2019-10-4
*  ������ZhangHJ
*  ˵�������ݶ�ȡ��Ϊ���Ĵ�����ַ��ȡ���ݵĹ���
*				 ֱ�Ӱ���ַ��ȡ�Ĵ����е�����,Ȼ�����ݷŵ�time�����ﱣ�漴��
***********************************************************/ 
void read_time()
{
   uchar n;
   for(n=0;n<7;n++)
    Time[n]=DS1302Read(read_addr[n]);   //��ȡ���ʱ��������
}

void Time_Display()
{
	LED_Display(0,dec(Time[2])/10);
	LED_Display(1,dec(Time[2])%10);
	LED_Display(2,10);
	LED_Display(3,dec(Time[1])/10);
	LED_Display(4,dec(Time[1])%10);
	LED_Display(5,10);
	LED_Display(6,dec(Time[0])/10);
	LED_Display(7,dec(Time[0])%10);
}

void main()
{
	uchar i;
	System_Init();
	ds1302_init();
	while(1)
	{
		read_time();
		for(i=0; i<30; i++)
		Time_Display();
	}
}

