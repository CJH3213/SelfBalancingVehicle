//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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


//extern IICIOType IIC1;		//两组IIC接口
//extern IICIOType IIC2;

//-----------------OLED IIC端口定义----------------  					   
#define IIC_SCL_Clr() IICIO.IIC_SCL->GPIOx->BRR = IICIO.IIC_SCL->GPIO_Pin//SCL IIC接口的时钟信号
#define IIC_SCL_Set() IICIO.IIC_SCL->GPIOx->BSRR = IICIO.IIC_SCL->GPIO_Pin

#define IIC_SDA_Clr() IICIO.IIC_SDA->GPIOx->BRR = IICIO.IIC_SDA->GPIO_Pin//SDA IIC接口的数据信号
#define IIC_SDA_Set() IICIO.IIC_SDA->GPIOx->BSRR = IICIO.IIC_SDA->GPIO_Pin

#define IIC_SDA_Read() (IICIO.IIC_SDA->GPIOx->IDR) & (IICIO.IIC_SDA->GPIO_Pin)//SDA IIC接口读数据

//常变量定义
#define IIC_ACK 1
#define IIC_NACK 0
enum boolType{bFalse=0,bTrue};

//IIC所有操作函数
void IIC_Init(IICIOType IICIO);                //初始化IIC的IO口				 
uint8_t IIC_Start(IICIOType IICIO);				//发送IIC开始信号
uint8_t IIC_Stop(IICIOType IICIO);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd,IICIOType IICIO);			//IIC发送一个字节
uint8_t IIC_Read_Byte(IICIOType IICIO);//IIC读取一个字节。参数为1发送应答，为0不发送应答
uint8_t IIC_Wait_Ack(IICIOType IICIO); 				//IIC等待ACK信号。返回值0为有应答，1为无应答
void IIC_Ack(IICIOType IICIO);					//IIC发送ACK信号
void IIC_NAck(IICIOType IICIO);				//IIC不发送ACK信号

//void IIC_single_byte_write(unsigned char sla,unsigned char Waddr,unsigned char Data,IICIOType IICIO);
//unsigned char IIC_single_byte_read(unsigned char sla_w,unsigned char sla_r, unsigned char Waddr,IICIOType IICIO);

enum boolType IIC_CheckDevice(uint8_t SlaveAddress,IICIOType IICIO);//检查地址是否有设备
#endif
















