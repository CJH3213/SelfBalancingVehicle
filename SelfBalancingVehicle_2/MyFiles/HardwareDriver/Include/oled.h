/**
  ******************************************************************************
  * @file           : oled.h
  * @brief          : oled12832、oled12864屏幕驱动
  ******************************************************************************
  **
  *
  ******************************************************************************
  */
#ifndef __OLED_H
#define __OLED_H			  	 

/*----------------------------------------------------------------------------**
**头文件               																									  		
**----------------------------------------------------------------------------*/
#include "main.h"
#include "my_IIC.h"
//#include "stdlib.h"

/*----------------------------------------------------------------------------**
**定义常量变量              																									
**----------------------------------------------------------------------------*/
#define OLED_DEVADDR_WRITE (0x3C<<1)		// OLED的7位设备地址（写地址）
#define OLED_MEMADDR_CMD	0x00
#define OLED_MEMADDR_DATA 0x40
#define OLED_CMD 0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		32
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	32	    						  
//-----------------OLED IIC端口定义----------------  					   

//#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOA, SCL_Pin, GPIO_PIN_RESET);//SCL IIC接口的时钟信号
//#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOA, SCL_Pin, GPIO_PIN_SET);

//#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOA, SDA_Pin, GPIO_PIN_RESET);//SDA IIC接口的数据信号
//#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOA, SDA_Pin, GPIO_PIN_SET);



/*----------------------------------------------------------------------------**
**OLED控制用函数               																								
**----------------------------------------------------------------------------*/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);			//向OLED写一字节数据。参数dat：数据内容；参数cmd：0为写命令，1为写数据。  
void OLED_Display_On(void);			//显示开启
void OLED_Display_Off(void);	  //显示关闭		   		    
void OLED_Init(void);						//OLED初始化
void OLED_Clear(void);					//清屏
//void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);			
//void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);

/*----------------------------------------------------------------------------**
**OLED直接显示函数               																							
** row是行不是像素纵坐标，0开始，每行有8个像素点，OLED12832有4行，OLED12864有8行。
** x是像素横坐标，0开始，是按像素点计算，范围0~127。
**----------------------------------------------------------------------------*/
void OLED_ShowChar(uint8_t x,uint8_t row,uint8_t chr,uint8_t charWidth);						//显示一个字符
void OLED_ShowNum(uint8_t x,uint8_t row,uint32_t num,uint8_t len,uint8_t charWidth);			//显示数字串
void OLED_ShowNum_HEX(uint8_t x,uint8_t row,uint32_t num,uint8_t len,uint8_t charWidth);	//显16进制数字串
void OLED_ShowString(uint8_t x,uint8_t row, char *chr,uint8_t charWidth);					//显示字符串
void OLED_Set_Pos(uint8_t x, uint8_t row);															//坐标设置
void OLED_ShowCHinese(uint8_t x,uint8_t row,uint8_t no);														//显示中文
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,const uint8_t BMP[]);			//显示图像
void fill_picture(uint8_t fill_Data);			//全屏填充
//void Picture(void);

char* OLED_S32ToString(int32_t num);		// OLED专用的有符32位整数转字符串，不要析构返回指针
char* OLED_FloatToString(float num, uint8_t keepDecimalsNum);		// OLED专用的float单精浮点数转字符串，不要析构返回指针

/*----------------------------------------------------------------------------**
**OLED带缓存区显示函数               																							
**----------------------------------------------------------------------------*/
#define OLED_BUFFER 1		// 是否启用缓存显示功能：1开启，0关闭
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
	 



