#include "my_IIC.h"
//#include "tim.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////	 
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
////////////////////////////////////////////////////////////////////////////////
 
//初始化IIC
void IIC_Init(IICIOType IICIO)
{	
	IIC_SCL_Set();
	IIC_SDA_Set();	
}

//产生IIC起始信号
//返回：1成功，0失败
uint8_t IIC_Start(IICIOType IICIO)
{
//	SDA_OUT();     //sda线输出
	IIC_SDA_Set();//IIC_SDA=1;	  	  
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(4);
	if(!IIC_SDA_Read())		return 0;			//如果SDA是低电平被占用，返回失败
 	IIC_SDA_Clr();//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_Clr();//IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
	return 1;
}

//产生IIC停止信号
//返回：1失败，0成功
uint8_t IIC_Stop(IICIOType IICIO)
{
//	SDA_OUT();//sda线输出
	IIC_SCL_Clr();//IIC_SCL=0;
	IIC_SDA_Clr();//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(1);
	if(IIC_SDA_Read())		return 0;
	IIC_SDA_Set();//IIC_SDA=1;//发送I2C总线结束信号
	delay_us(1);
	if(!IIC_SDA_Read())		return 0;
	delay_us(4);
	return 1;	
}
//等待应答信号到来
//返回值：1，接收应答成功
//        0，接收应答失败
uint8_t IIC_Wait_Ack(IICIOType IICIO)
{
	uint8_t ucErrTime=0;
//	SDA_IN();      //SDA设置为输入  
	IIC_SDA_Set();//IIC_SDA=1;
	delay_us(1);	   
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(1);	 
	while(IIC_SDA_Read())
	{
		ucErrTime++;
		if(ucErrTime>250)		//等待超时
		{
			IIC_Stop(IICIO);
			return 0;
		}
	}
	IIC_SCL_Clr();//IIC_SCL=0;//时钟输出0 	   
	return 1;  
} 
//产生ACK应答
void IIC_Ack(IICIOType IICIO)
{
	IIC_SCL_Clr();//IIC_SCL=0;
//	SDA_OUT();
	IIC_SDA_Clr();//IIC_SDA=0;
	delay_us(2);
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(2);
	IIC_SCL_Clr();//IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(IICIOType IICIO)
{
	IIC_SCL_Clr();//IIC_SCL=0;
//	SDA_OUT();
	IIC_SDA_Set();//IIC_SDA=1;
	delay_us(2);
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(2);
	IIC_SCL_Clr();//IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答 X
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd,IICIOType IICIO)
{                        
//	SDA_OUT(); 	    
    IIC_SCL_Clr();//IIC_SCL=0;//拉低时钟，待SDA输出后再拉高SCL
    for(uint8_t t=0;t<8;t++)
    {   
     if(txd&0x80){IIC_SDA_Set();}//IIC_SDA=(txd&0x80)>>7;
			else IIC_SDA_Clr(); 
        txd<<=1; 	  
		delay_us(2);
		IIC_SCL_Set();//IIC_SCL=1;
		delay_us(2); 
		IIC_SCL_Clr();//IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节
uint8_t IIC_Read_Byte(IICIOType IICIO)
{
	unsigned char i,receive=0;
//	SDA_IN();//SDA设置为输入
	
	IIC_SDA_Set();	//数据线释放
	for(i=0;i<8;i++ )
	{
		receive<<=1;
		IIC_SCL_Set();//IIC_SCL=1;
		delay_us(2);
		if(IIC_SDA_Read())receive |= 0x01;
		IIC_SCL_Clr();
		delay_us(2); 
	}					 
//	if (!ack)
//			IIC_NAck(IICIO);//发送nACK
//	else
//			IIC_Ack(IICIO); //发送ACK   
	return receive;
}
//-------------------------------------------------------------------
void IIC_single_byte_write(unsigned char sla,unsigned char Waddr,unsigned char Data,IICIOType IICIO)
{//向任意地址写入一个字节数据函数
    IIC_Start(IICIO);//产生起始信号
    IIC_Send_Byte(sla,IICIO);//写入设备地址（写）
    IIC_Wait_Ack(IICIO);//等待设备的应答
    IIC_Send_Byte(Waddr,IICIO);//写入要操作的单元地址。
    IIC_Wait_Ack(IICIO);//等待设备的应答。
    IIC_Send_Byte(Data,IICIO);//写入数据。
    IIC_Wait_Ack(IICIO);//等待设备的应答。
    IIC_Stop(IICIO);//产生停止符号。
}
//-------------------------------------------------------------------
unsigned char IIC_single_byte_read(unsigned char sla_w,unsigned char sla_r, unsigned char Waddr,IICIOType IICIO)
{//从任意地址读取一个字节数据函数
    unsigned char Data;//定义一个缓冲寄存器。
    IIC_Start(IICIO);//产生起始信号
    IIC_Send_Byte(sla_w,IICIO);//写入设备地址（写）
    IIC_Wait_Ack(IICIO);//等待设备的应答
    IIC_Send_Byte(Waddr,IICIO);//写入要操作的单元地址。
    IIC_Wait_Ack(IICIO);//等待设备的应答。
    IIC_Stop(IICIO);//产生停止符号。
    IIC_Start(IICIO);//产生起始信号
    IIC_Send_Byte(sla_r,IICIO);//写入设备地址（读）
    IIC_Wait_Ack(IICIO);//等待设备的应答
//    Data=IIC_Read_Byte(0,IICIO);//读出数据，无应答
    delay_us(1); 
    IIC_Stop(IICIO);//产生停止符号。
    //-------------------返回读取的数据--------------------
    return Data;//返回读取的一个字节数据。
}

//-------------------------------------------------------------------
//检查地址是否有设备
//返回：0成功，1失败
enum boolType IIC_CheckDevice(uint8_t SlaveAddress,IICIOType IICIO)
{
	if(IIC_Start(IICIO)==bFalse)		return bFalse;
	IIC_Send_Byte(SlaveAddress,IICIO);
	if(IIC_Wait_Ack(IICIO)==bFalse)
	{
		IIC_Stop(IICIO);
		return bFalse;
	}
	if(IIC_Stop(IICIO)==bFalse)		return bFalse;
	return bTrue;
}


























