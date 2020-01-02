#include "led.h"

// 主函数功能：关闭蜂鸣器和继电器,LED循环点亮
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