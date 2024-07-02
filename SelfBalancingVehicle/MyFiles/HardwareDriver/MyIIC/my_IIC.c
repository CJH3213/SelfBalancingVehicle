#include "my_IIC.h"
//#include "tim.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////	 
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
////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_Init(IICIOType IICIO)
{	
	IIC_SCL_Set();
	IIC_SDA_Set();	
}

//����IIC��ʼ�ź�
//���أ�1�ɹ���0ʧ��
uint8_t IIC_Start(IICIOType IICIO)
{
//	SDA_OUT();     //sda�����
	IIC_SDA_Set();//IIC_SDA=1;	  	  
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(4);
	if(!IIC_SDA_Read())		return 0;			//���SDA�ǵ͵�ƽ��ռ�ã�����ʧ��
 	IIC_SDA_Clr();//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_Clr();//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
	return 1;
}

//����IICֹͣ�ź�
//���أ�1ʧ�ܣ�0�ɹ�
uint8_t IIC_Stop(IICIOType IICIO)
{
//	SDA_OUT();//sda�����
	IIC_SCL_Clr();//IIC_SCL=0;
	IIC_SDA_Clr();//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(1);
	if(IIC_SDA_Read())		return 0;
	IIC_SDA_Set();//IIC_SDA=1;//����I2C���߽����ź�
	delay_us(1);
	if(!IIC_SDA_Read())		return 0;
	delay_us(4);
	return 1;	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ɹ�
//        0������Ӧ��ʧ��
uint8_t IIC_Wait_Ack(IICIOType IICIO)
{
	uint8_t ucErrTime=0;
//	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA_Set();//IIC_SDA=1;
	delay_us(1);	   
	IIC_SCL_Set();//IIC_SCL=1;
	delay_us(1);	 
	while(IIC_SDA_Read())
	{
		ucErrTime++;
		if(ucErrTime>250)		//�ȴ���ʱ
		{
			IIC_Stop(IICIO);
			return 0;
		}
	}
	IIC_SCL_Clr();//IIC_SCL=0;//ʱ�����0 	   
	return 1;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ�� X
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd,IICIOType IICIO)
{                        
//	SDA_OUT(); 	    
    IIC_SCL_Clr();//IIC_SCL=0;//����ʱ�ӣ���SDA�����������SCL
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
//��1���ֽ�
uint8_t IIC_Read_Byte(IICIOType IICIO)
{
	unsigned char i,receive=0;
//	SDA_IN();//SDA����Ϊ����
	
	IIC_SDA_Set();	//�������ͷ�
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
//			IIC_NAck(IICIO);//����nACK
//	else
//			IIC_Ack(IICIO); //����ACK   
	return receive;
}
//-------------------------------------------------------------------
void IIC_single_byte_write(unsigned char sla,unsigned char Waddr,unsigned char Data,IICIOType IICIO)
{//�������ַд��һ���ֽ����ݺ���
    IIC_Start(IICIO);//������ʼ�ź�
    IIC_Send_Byte(sla,IICIO);//д���豸��ַ��д��
    IIC_Wait_Ack(IICIO);//�ȴ��豸��Ӧ��
    IIC_Send_Byte(Waddr,IICIO);//д��Ҫ�����ĵ�Ԫ��ַ��
    IIC_Wait_Ack(IICIO);//�ȴ��豸��Ӧ��
    IIC_Send_Byte(Data,IICIO);//д�����ݡ�
    IIC_Wait_Ack(IICIO);//�ȴ��豸��Ӧ��
    IIC_Stop(IICIO);//����ֹͣ���š�
}
//-------------------------------------------------------------------
unsigned char IIC_single_byte_read(unsigned char sla_w,unsigned char sla_r, unsigned char Waddr,IICIOType IICIO)
{//�������ַ��ȡһ���ֽ����ݺ���
    unsigned char Data;//����һ������Ĵ�����
    IIC_Start(IICIO);//������ʼ�ź�
    IIC_Send_Byte(sla_w,IICIO);//д���豸��ַ��д��
    IIC_Wait_Ack(IICIO);//�ȴ��豸��Ӧ��
    IIC_Send_Byte(Waddr,IICIO);//д��Ҫ�����ĵ�Ԫ��ַ��
    IIC_Wait_Ack(IICIO);//�ȴ��豸��Ӧ��
    IIC_Stop(IICIO);//����ֹͣ���š�
    IIC_Start(IICIO);//������ʼ�ź�
    IIC_Send_Byte(sla_r,IICIO);//д���豸��ַ������
    IIC_Wait_Ack(IICIO);//�ȴ��豸��Ӧ��
//    Data=IIC_Read_Byte(0,IICIO);//�������ݣ���Ӧ��
    delay_us(1); 
    IIC_Stop(IICIO);//����ֹͣ���š�
    //-------------------���ض�ȡ������--------------------
    return Data;//���ض�ȡ��һ���ֽ����ݡ�
}

//-------------------------------------------------------------------
//����ַ�Ƿ����豸
//���أ�0�ɹ���1ʧ��
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


























