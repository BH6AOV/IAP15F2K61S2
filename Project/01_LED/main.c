#include "led.h"

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