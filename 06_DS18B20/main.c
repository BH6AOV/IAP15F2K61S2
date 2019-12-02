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

// 0~9、0.~9.、°、C
uchar code tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x40,0x79,0x24,0x30,0x19,0x12,0x12,0x78,0x00,0x12,0x9C,0xC6};
uchar code select[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

/**********************************************************
*  函数名称：500us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：2us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
void Delay2us()		//@11.0592MHz
{
	unsigned char i;

	i = 3;
	while (--i);
}



/**********************************************************
*  函数名称：600us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：100us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}



/**********************************************************
*  函数名称：420us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：60us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：480us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：50us延时
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：由STC-ISP软件自动生成
***********************************************************/
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



/**********************************************************
*  函数名称：系统初始化
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：关闭蜂鸣器、继电器和LED
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
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：用于显示温度数据
*		 1. 参数selection表示数码管位选
*		 2. 参数segment表示数码管段选
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
	Delay600us();
	Delay500us();
	HC138_A = 1;
	HC138_B = 1;
	HC138_C = 1;
	WR = 0;
	P0 = 0xFF;
}



/**********************************************************
*  函数名称：DS18B20初始化
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：用于初始化DS18B20温度传感器
*		 1. Master先将总线拉低480~960us，然后释放总线
*		 2. DS18B20等待15~60us之后，将总线拉低60~240us
***********************************************************/
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



/**********************************************************
*  函数名称：DS18B20写1Byte数据
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：用于向DS18B20写1Byte数据
*		 1. 写0或写1之前，Master都会把总线拉低
*		 2. 写1时，Master会在15us之内释放总线; 写0时，Master会维持总线低的状态
*		 3. DS18B20会在15~60us时隙内采集数据
***********************************************************/
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



/**********************************************************
*  函数名称：DS18B20读1Byte数据
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：用于从DS18B20读1Byte数据
*		 1. 对于读时隙，开始之前Master都要先拉低总线至少1us
*		 2. 18B20如果维持总线低状态，Master就会读到0; 18B20如果释放总线，由于上拉电阻作用，Master就会读到1
*		 3. Master会在15us的末位读入数据，整个读时隙维持60us左右
***********************************************************/
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



/**********************************************************
*  函数名称：DS18B20读取温度数据
*  修改日期：2019-12-2
*  修改人：ZhangHJ
*  说明：用于从DS18B20获取温度数据
*		 1. 初始化-->ROM命令-->控制命令
*		 2. 初始化-->跳过ROM-->温度转换命令
*		 3. 初始化-->跳过ROM-->读取温度命令
*		 4. 两次温度读取,然后处理数据
***********************************************************/
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
	temp = ((TPH << 8) | TPL) * 6.25;
}



// 主函数功能：DS18B20显示温度数值
void main()
{
	System_Init();
	while(1)
	{
		DS18B20_FUNCTION();
		Delay500us();
		{
			LED_Display(2, temp/1000);
			LED_Display(3, (temp/100)%10+10);
			LED_Display(4, (temp%100)/10);
			LED_Display(5, temp%10);
			LED_Display(6, 20);
			LED_Display(7, 21);
		}
	}
}