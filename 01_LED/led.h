/**********************************************************
*  �ļ�����led.h
*  �޸����ڣ�2020-01-02
*  ������ZhangHJ
*  ˵��������LED���̵�ͷ�ļ�
***********************************************************/
#ifndef __LED__
#define __LED__

#include "stc15f2k60s2.h"
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

void Delay100ms();
void System_Init();
void LED_config();

sbit HC138_A = P2^5;	// 74HC138��������� A
sbit HC138_B = P2^6;	// 74HC138��������� B
sbit HC138_C = P2^7;	// 74HC138��������� C
sbit WR = P4^2;

#endif