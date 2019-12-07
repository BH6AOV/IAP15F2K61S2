#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define dec(dat) ((dat>>4)*10+(dat&0x0f)) 		// BCD码转十进制
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
*  函数名称：DS1302写操作函数
*  日期：2019-10-4
*  姓名：ZhangHJ
*  说明：写操作包含两个参数,add表示要写入的地址;wdata表示要写入的数据
*				 1. 读写操作需要先将RST拉高才能进行
*				 2. 先写入控制字节,在时钟上升沿串行写入数据
*				 3. 再写入数据字节,同样在时钟上升沿写入数据
*				 4. 最后拉低RST禁止数据传输
***********************************************************/
void DS1302Write(uchar add,uchar wdata)
{
	uchar a;
	//wdata = hex(wdata);					// 转换为BCD码
	RST=0;									// 拉低RST引脚,终止数据传输
	SCLK=0;									// 拉低SCLK引脚,清零时钟线
	RST=1;									// 拉高RST引脚,所有数据传输都要拉高RST脚,启动控制逻辑
	//先写入控制字节
	for(a=0; a<8; a++)
	{
			IO= add & 0x01;					// IO引脚准备好要写入的1位数据
			SCLK=1;							// SCLK上升沿,1位数据从IO脚写入,低位先写入
			add>>=1;						// 数据右移1位
			SCLK=0;							// 拉低SCLK,为下次写入准备,循环8次写入1字节
	}
	//再写入数据字节
	for(a=0; a<8; a++)
	{
			IO= wdata & 0x01;
			SCLK=1;
			wdata>>=1;
			SCLK=0;
	}
	RST=0;									// 数据传输完拉低RST
}  



/**********************************************************
*  函数名称：DS1302读操作函数
*  日期：2019-10-4
*  姓名：ZhangHJ
*  说明：读操作只需一个参数,即需要读取的寄存器地址add
*				 1. 首先需要将需要读取的地址(1Byte)写入寄存器,在上升沿进行写入操作
*				 2. 之后读取该地址中的数据,在下降沿读取数据
*				 3. 最后返回读取到的1Byte数据
***********************************************************/ 
uchar DS1302Read(uchar add)
{
	uchar a, rdata=0;
	RST = 0;											// 拉低RST引脚,终止数据传输
	SCLK = 0;											// 拉低SCLK引脚,清零时钟线
	RST = 1;											// 拉高RST引脚,启动控制逻辑
	//发送控制字节
	for(a=0; a<8; a++)
	{
		SCLK = 0;
		IO = add & 0x01;
		SCLK = 1;										// 制造一个上升沿,写入地址
		add >>= 1;
	}
	//读1字节数据
	for(a=0; a<8; a++)
	{
		SCLK = 1;
		rdata >>= 1;
		SCLK = 0;										// 制造一个下降沿,读取数据
		if(IO)
		{												// 如果读到1
			rdata |= 0x80;								// 把最高位置为1,记录到rdata中
		}
	}
	RST=0;												// 拉低RST
	SCLK=1;
	IO=0;
	IO=1;
	//return dec(d);        							// 读取的数据转换成十进制
	return rdata;
}



/**********************************************************
*  函数名称：DS1302初始化函数
*  日期：2019-10-4
*  姓名：ZhangHJ
*  说明：初始化即为对DS1302初始数据的写入
*				 1. 首先需要关闭写保护,以允许数据写入
*				 2. 之后按"秒分时日月周年"的顺序写入初始数据
*				 3. 最后开启写保护,禁止数据写入
***********************************************************/
void ds1302_init()
{
   uchar k;
   DS1302Write(0x8e,0x00);  				// 禁止写保护，即允许数据写入
	if(DS1302Read(0x81) & 0x80)				// 掉电检测,时钟停止标志位
   for(k=0;k<7;k++)							// 写入7个字节的时钟信号：秒分时日月周年
   {
     DS1302Write(write_addr[k],Time[k]);
   }
   DS1302Write(0x8e,0x80);  				// 打开写保护,禁止数据写入
}




/**********************************************************
*  函数名称：DS1302数据读取函数
*  日期：2019-10-4
*  姓名：ZhangHJ
*  说明：数据读取即为按寄存器地址读取数据的过程
*				 直接按地址读取寄存器中的数据,然后将数据放到time数组里保存即可
***********************************************************/ 
void read_time()
{
   uchar n;
   for(n=0;n<7;n++)
    Time[n]=DS1302Read(read_addr[n]);   //读取秒分时日月周年
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

