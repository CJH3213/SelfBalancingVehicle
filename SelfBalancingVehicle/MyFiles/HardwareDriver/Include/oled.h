//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PD6（SCL）
//              SDA   接PD7（SDA）            
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 

#include "main.h"
#include "my_IIC.h"

//#include "stdlib.h"
#define DEVADDR_OLED_WRITE 0x78		// OLED的7位设备地址（写地址）
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


/**********************************************
//OLED控制用函数
**********************************************/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);			//向OLED写一字节数据。参数dat：数据内容；参数cmd：0为写命令，1为写数据。  
void OLED_Display_On(void);			//显示开启
void OLED_Display_Off(void);	  //显示关闭		   		    
void OLED_Init(void);						//OLED初始化
void OLED_Clear(void);					//清屏
//void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);			
//void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t charWidth);						//显示一个字符
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth);			//显示数字串
void OLED_ShowNum_HEX(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth);	//显16进制数字串
void OLED_ShowString(uint8_t x,uint8_t y, char *chr,uint8_t charWidth);					//显示字符串
void OLED_Set_Pos(uint8_t x, uint8_t y);															//坐标设置
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);														//显示中文
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,const uint8_t BMP[]);			//显示图像
void fill_picture(uint8_t fill_Data);			//全屏填充
//void Picture(void);
#endif  
	 



