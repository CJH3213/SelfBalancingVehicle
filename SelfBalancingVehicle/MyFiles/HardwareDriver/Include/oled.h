//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : Evk123
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PD6��SCL��
//              SDA   ��PD7��SDA��            
//              ----------------------------------------------------------------
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 

#include "main.h"
#include "my_IIC.h"

//#include "stdlib.h"
#define DEVADDR_OLED_WRITE 0x78		// OLED��7λ�豸��ַ��д��ַ��
#define OLED_CMD 0	//д����
#define OLED_DATA 1	//д����

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		32
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	32	    						  
//-----------------OLED IIC�˿ڶ���----------------  					   

//#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOA, SCL_Pin, GPIO_PIN_RESET);//SCL IIC�ӿڵ�ʱ���ź�
//#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOA, SCL_Pin, GPIO_PIN_SET);

//#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOA, SDA_Pin, GPIO_PIN_RESET);//SDA IIC�ӿڵ������ź�
//#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOA, SDA_Pin, GPIO_PIN_SET);


/**********************************************
//OLED�����ú���
**********************************************/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);			//��OLEDдһ�ֽ����ݡ�����dat���������ݣ�����cmd��0Ϊд���1Ϊд���ݡ�  
void OLED_Display_On(void);			//��ʾ����
void OLED_Display_Off(void);	  //��ʾ�ر�		   		    
void OLED_Init(void);						//OLED��ʼ��
void OLED_Clear(void);					//����
//void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);			
//void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t charWidth);						//��ʾһ���ַ�
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth);			//��ʾ���ִ�
void OLED_ShowNum_HEX(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth);	//��16�������ִ�
void OLED_ShowString(uint8_t x,uint8_t y, char *chr,uint8_t charWidth);					//��ʾ�ַ���
void OLED_Set_Pos(uint8_t x, uint8_t y);															//��������
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);														//��ʾ����
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,const uint8_t BMP[]);			//��ʾͼ��
void fill_picture(uint8_t fill_Data);			//ȫ�����
//void Picture(void);
#endif  
	 



