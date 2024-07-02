/**
  ******************************************************************************
  * @file           : BH1750.c
  * @brief          : 光强度传感器驱动，IIC通信
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "BH1750.h"
//#include "my_IIC.h"

GPIOType SCL2={GPIOB, GPIO_PIN_10};
GPIOType SDA2={GPIOB, GPIO_PIN_11};
IICIOType IIC2={&SCL2,&SDA2};

//IIC写传感器
enum boolType BH1750_Write(uint8_t SlaveAddress, uint8_t REG_Address)
{
	if(IIC_Start(IIC2)==bFalse)		return bFalse;
	IIC_Send_Byte(SlaveAddress, IIC2);
	if(IIC_Wait_Ack(IIC2)==bFalse){IIC_Stop(IIC2);	return bFalse;}
	IIC_Send_Byte(REG_Address,IIC2);
	if(IIC_Wait_Ack(IIC2)==bFalse){IIC_Stop(IIC2);	return bFalse;}
	if(IIC_Stop(IIC2)==bFalse){return bFalse;}
	return bTrue;
	
//	IIC_Start(IIC2);
//	IIC_Send_Byte(IICSLADDR_BH1750, IIC2);
//	IIC_Send_Byte(REG_Address, IIC2);
//	IIC_Stop(IIC2);
//	return bTrue;
}

//IIC读传感器
enum boolType BH1750_Read(uint8_t SlaveAddress, uint8_t *buf, uint8_t len)
{
	if(IIC_Start(IIC2)==bFalse)		return bFalse;
	IIC_Send_Byte(SlaveAddress | 1, IIC2);
	if(IIC_Wait_Ack(IIC2)==bFalse){IIC_Stop(IIC2);	return bFalse;}
	for(uint16_t i=0; i<len; i++)
	{
		buf[i] = IIC_Read_Byte(IIC2);
		if(i<len-1)	IIC_Ack(IIC2);
	}
	IIC_NAck(IIC2);
	if(IIC_Stop(IIC2)==bFalse){return bFalse;}
	return bTrue;
	
//	IIC_Start(IIC2);
//	IIC_Send_Byte(SlaveAddress+1, IIC2);
//	IIC_Wait_Ack(IIC2);
//	for(uint8_t i=0; i<len; i++)
//	{
//		buf[i] = IIC_Read_Byte(IIC2);
//		if(i == 3)
//		{
//			IIC_NAck(IIC2);
//		}
//		else	IIC_Ack(IIC2);
//	}
//	IIC_Stop(IIC2);
//	return bTrue;
}

//传感器初始化
enum boolType BH1750_Init(void)
{
	BH1750_Write(IICSLADDR_BH1750, BH1750_ON);
	BH1750_Write(IICSLADDR_BH1750, RESOLUTION);
//	if(BH1750_Write(IICSLADDR_BH1750, BH1750_ON)==bFalse) return bFalse;
//	if(BH1750_Write(IICSLADDR_BH1750, RESOLUTION)==bFalse) return bFalse;
	return bTrue;
}


#include "oled.h"
#include "delay.h"
// 从传感器获取数据
uint16_t BH1750_RawData(void)
{
	uint8_t buf[2];
	BH1750_Write(IICSLADDR_BH1750, RESOLUTION);	//配置BH1750
	delay_ms(200);
	BH1750_Read(IICSLADDR_BH1750, buf, 2);		//读取光照强度数据
	return (((uint16_t)buf[0]<<8)+buf[1]);		//整合高低位数据
}

//将原数据转为光照强度，单位lx.
float BH1750_transValue(void)
{
		
    return (float)(BH1750_RawData() / 1.2f * SCALE_INTERVAL);
}
