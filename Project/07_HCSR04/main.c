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
uint distence = 0;	// Distence

/**********************************************************
*  函数名称：10us延时@11.0592MHz
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
void Delay10us()	//@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 25;
	while (--i);
}



/**********************************************************
*  函数名称：2ms延时@11.0592MHz
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：系统初始化
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：关闭开发板的蜂鸣器、继电器和LED
***********************************************************/
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



/**********************************************************
*  函数名称：LED显示函数
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：用于显示距离数值
*		 1. 参数selection表示位选
*		 2. 参数segment表示位选数组索引值
***********************************************************/
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



/**********************************************************
*  函数名称：超声波触发
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：用于发送超声波触发信号
*		 1. 超声波触发需要由发送端口发送8个40KHz的脉冲信号
*		 2. 这里40KHz可能不太准确
***********************************************************/
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



/**********************************************************
*  函数名称：定时器T1配置
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：用于配置定时器T1
*		 1. 配置定时器1为12分频，定时模式方式1
*		 2. 初值设为0
***********************************************************/
void Timer1_Init(void)
{
	AUXR &= 0xBF;	// 定时器T1 12分频
	TMOD &= 0x0F;	// 定时器T1 方式1
	TL1 = 0x00;
	TH1 = 0x00;
	TF1 = 0;
}



/**********************************************************
*  函数名称：测距函数
*  修改日期：2019-12-3
*  修改人：ZhangHJ
*  说明：用于获取距离数据
*		 1. 在“定时器未溢出并且回响信号为高电平”时，持续计时
*		 2. 如果定时器溢出，说明距离太长；此时需要清除定时器溢出标志位
*		 3. 如果回响信号为低电平，说明回响时间正常；此时需要注意重置定时器初始值
*		 4. 根据定时器的数值计算距离
***********************************************************/
void Get_Distence()
{
	uint temp;
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
}



// 主函数功能：超声波测距，在数码管上显示距离数值
void main()
{
	uint i;
	System_Init();
	Timer1_Init();
	while(1)
	{
		Send_Info();
		Get_Distence();
		for(i=0; i<50; i++)		// 延时显示
		{
			LED_Display(4, distence/1000);
			LED_Display(5, distence/100%10);
			LED_Display(6, distence%100/10+10);
			LED_Display(7, distence%10);
		}
	}
}