/*****************************************************************
*������Դ���ٿ��ڴ���������ʵ��
*�������ⷢ�����
*****************************************************************/

#ifndef __INFRARED_H
#define __INFRARED_H

#include "main.h"
#include "CD4051.h"

#define u8 unsigned char

//#define BITBAND_REG(REG, BIT) (*((uint32_t volatile*)(0x42000000u + (((uint32_t)&(REG)-(uint32_t)0x40000000u)<<5) + (((uint32_t)(BIT))<<2))))

//#define RI_TXD BITBAND_REG(GPIOA->ODR, 15)  
#define RI_TXD_H CD4051_OutPut(TG_RI_TXD, 1)
#define RI_TXD_L CD4051_OutPut(TG_RI_TXD, 0)

static u8 HW_K[6]={0x03,0x05,0x14,0x45,0xDE,0x92};  //��������
static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //��Դ��λ��1
static u8 H_S[4]={0x80,0x7F,0x05,~(0x05)};	 //��Ƭ�Ϸ�

//void Infrared_Init(void);
void Infrared_Send(u8 *s,int n);

#endif




