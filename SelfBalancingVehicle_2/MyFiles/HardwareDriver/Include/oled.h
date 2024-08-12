/**
  ******************************************************************************
  * @file           : oled.h
  * @brief          : oled12832��oled12864��Ļ����
  ******************************************************************************
  **
  *
  ******************************************************************************
  */
#ifndef __OLED_H
#define __OLED_H			  	 

/*----------------------------------------------------------------------------**
**ͷ�ļ�               																									  		
**----------------------------------------------------------------------------*/
#include "main.h"
#include "my_IIC.h"
//#include "stdlib.h"

/*----------------------------------------------------------------------------**
**���峣������              																									
**----------------------------------------------------------------------------*/
#define OLED_DEVADDR_WRITE (0x3C<<1)		// OLED��7λ�豸��ַ��д��ַ��
#define OLED_MEMADDR_CMD	0x00
#define OLED_MEMADDR_DATA 0x40
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



/*----------------------------------------------------------------------------**
**OLED�����ú���               																								
**----------------------------------------------------------------------------*/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);			//��OLEDдһ�ֽ����ݡ�����dat���������ݣ�����cmd��0Ϊд���1Ϊд���ݡ�  
void OLED_Display_On(void);			//��ʾ����
void OLED_Display_Off(void);	  //��ʾ�ر�		   		    
void OLED_Init(void);						//OLED��ʼ��
void OLED_Clear(void);					//����
//void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);			
//void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);

/*----------------------------------------------------------------------------**
**OLEDֱ����ʾ����               																							
** row���в������������꣬0��ʼ��ÿ����8�����ص㣬OLED12832��4�У�OLED12864��8�С�
** x�����غ����꣬0��ʼ���ǰ����ص���㣬��Χ0~127��
**----------------------------------------------------------------------------*/
void OLED_ShowChar(uint8_t x,uint8_t row,uint8_t chr,uint8_t charWidth);						//��ʾһ���ַ�
void OLED_ShowNum(uint8_t x,uint8_t row,uint32_t num,uint8_t len,uint8_t charWidth);			//��ʾ���ִ�
void OLED_ShowNum_HEX(uint8_t x,uint8_t row,uint32_t num,uint8_t len,uint8_t charWidth);	//��16�������ִ�
void OLED_ShowString(uint8_t x,uint8_t row, char *chr,uint8_t charWidth);					//��ʾ�ַ���
void OLED_Set_Pos(uint8_t x, uint8_t row);															//��������
void OLED_ShowCHinese(uint8_t x,uint8_t row,uint8_t no);														//��ʾ����
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,const uint8_t BMP[]);			//��ʾͼ��
void fill_picture(uint8_t fill_Data);			//ȫ�����
//void Picture(void);

char* OLED_S32ToString(int32_t num);		// OLEDר�õ��з�32λ����ת�ַ�������Ҫ��������ָ��
char* OLED_FloatToString(float num, uint8_t keepDecimalsNum);		// OLEDר�õ�float����������ת�ַ�������Ҫ��������ָ��

/*----------------------------------------------------------------------------**
**OLED����������ʾ����               																							
**----------------------------------------------------------------------------*/
#define OLED_BUFFER 1		// �Ƿ����û�����ʾ���ܣ�1������0�ر�
#if OLED_BUFFER == 1
	#define OLED_WIDTH 128
	#define OLED_HEIGHT 32

void OLED_Buffer_Refresh(void);
void OLED_Buffer_Clear(void);
void OLED_Buffer_ShowPoint(uint8_t x, uint8_t y, uint8_t color);
//void OLED_Buffer_Show8PixelsOfColumn(uint8_t x, uint8_t y, uint8_t byte, uint8_t cover);
void OLED_Buffer_ShowChar(uint8_t x, uint8_t y, char c, uint8_t charWidth);
void OLED_Buffer_ShowString(uint8_t x,uint8_t y, char str[],uint8_t charWidth);
#endif

#endif  
	 



