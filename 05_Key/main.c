#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;
uchar led[] = {0xFC,0xF3,0xCF,0x3F};

/**********************************************************
*  函数名称：15ms延时函数
*  日期：2019-12-01
*  姓名：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
void Delay15ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	_nop_();
	_nop_();
	i = 1;
	j = 162;
	k = 89;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



/**********************************************************
*  函数名称：系统初始化函数
*  日期：2019-12-01
*  姓名：ZhangHJ
*  说明：关闭蜂鸣器,继电器和LED
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
*  日期：2019-12-01
*  姓名：ZhangHJ
*  说明：显示led数组里的数字
***********************************************************/
void LED_Display(uchar num)
{
	uchar display = led[num];
	HC138_A = 0;
	HC138_B = 0;
	HC138_C = 1;
	WR = 0;
	P0 = display;
}



// 主函数功能：由独立按键控制LED显示
void main()
{
	uchar flag;
	System_Init();
	while(1)
	{
		P3 |= 0x0f;
		flag = P3|0xf0;
		if(flag != 0xff)
		{
			Delay15ms();
			switch(flag|0xf0)
			{
				case 0xfe:
					LED_Display(0);
					break;
				case 0xfd:
					LED_Display(1);
					break;
				case 0xfb:
					LED_Display(2);
					break;
				case 0xf7:
					LED_Display(3);
					break;
			}
		}
	}
	
	
}