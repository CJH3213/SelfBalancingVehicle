//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#ifndef __MYIIC_H
#define __MYIIC_H
#include "main.h"

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}GPIOType;

typedef struct
{
	GPIOType *IIC_SCL;
	GPIOType *IIC_SDA;
}IICIOType;


//extern IICIOType IIC1;		//����IIC�ӿ�
//extern IICIOType IIC2;

//-----------------OLED IIC�˿ڶ���----------------  					   
#define IIC_SCL_Clr() IICIO.IIC_SCL->GPIOx->BRR = IICIO.IIC_SCL->GPIO_Pin//SCL IIC�ӿڵ�ʱ���ź�
#define IIC_SCL_Set() IICIO.IIC_SCL->GPIOx->BSRR = IICIO.IIC_SCL->GPIO_Pin

#define IIC_SDA_Clr() IICIO.IIC_SDA->GPIOx->BRR = IICIO.IIC_SDA->GPIO_Pin//SDA IIC�ӿڵ������ź�
#define IIC_SDA_Set() IICIO.IIC_SDA->GPIOx->BSRR = IICIO.IIC_SDA->GPIO_Pin

#define IIC_SDA_Read() (IICIO.IIC_SDA->GPIOx->IDR) & (IICIO.IIC_SDA->GPIO_Pin)//SDA IIC�ӿڶ�����

//����������
#define IIC_ACK 1
#define IIC_NACK 0
enum boolType{bFalse=0,bTrue};

//IIC���в�������
void IIC_Init(IICIOType IICIO);                //��ʼ��IIC��IO��				 
uint8_t IIC_Start(IICIOType IICIO);				//����IIC��ʼ�ź�
uint8_t IIC_Stop(IICIOType IICIO);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd,IICIOType IICIO);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(IICIOType IICIO);//IIC��ȡһ���ֽڡ�����Ϊ1����Ӧ��Ϊ0������Ӧ��
uint8_t IIC_Wait_Ack(IICIOType IICIO); 				//IIC�ȴ�ACK�źš�����ֵ0Ϊ��Ӧ��1Ϊ��Ӧ��
void IIC_Ack(IICIOType IICIO);					//IIC����ACK�ź�
void IIC_NAck(IICIOType IICIO);				//IIC������ACK�ź�

//void IIC_single_byte_write(unsigned char sla,unsigned char Waddr,unsigned char Data,IICIOType IICIO);
//unsigned char IIC_single_byte_read(unsigned char sla_w,unsigned char sla_r, unsigned char Waddr,IICIOType IICIO);

enum boolType IIC_CheckDevice(uint8_t SlaveAddress,IICIOType IICIO);//����ַ�Ƿ����豸
#endif
















