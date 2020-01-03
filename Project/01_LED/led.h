/**********************************************************
*  文件名：led.h
*  修改日期：2020-01-02
*  姓名：ZhangHJ
*  说明：整合LED工程的头文件
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

sbit HC138_A = P2^5;	// 74HC138数据输入端 A
sbit HC138_B = P2^6;	// 74HC138数据输入端 B
sbit HC138_C = P2^7;	// 74HC138数据输入端 C
sbit WR = P4^2;

#endif