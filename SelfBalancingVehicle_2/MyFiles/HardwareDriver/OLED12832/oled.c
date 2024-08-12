//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖĞ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼ş Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üĞŞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑİÊ¾Àı³Ì(STM32F103ZEÏµÁĞIIC)
//              ËµÃ÷: 
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPD6£¨SCL£©
//              SDA   ½ÓPD7£¨SDA£©            
//              ----------------------------------------------------------------
//Copyright(C) ÖĞ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"
#include "i2c.h"
#include "stdlib.h"
#include "oledfont.h" 
//#include "my_IIC.h"
#include <stdio.h>

// ×¢Òâ£¡£¡£¡¸ÄIO¿ÚÔÚÕâÀï¸Ä£¡
GPIOType SCL1={GPIOB, GPIO_PIN_10};
GPIOType SDA1={GPIOB, GPIO_PIN_11};
IICIOType IIC1={&SCL1,&SDA1};

//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
//   IIC_Start(IIC1);
//   IIC_Send_Byte(0x78,IIC1);            //Slave address,SA0=0
//	IIC_Wait_Ack(IIC1);	
//  IIC_Send_Byte(0x00,IIC1);			//write command
//	IIC_Wait_Ack(IIC1);	
//   IIC_Send_Byte(IIC_Command,IIC1); 
//	IIC_Wait_Ack(IIC1);	
//   IIC_Stop(IIC1);
	// ÒÔÉÏÎªÈí¼şIIC£¬ÒÔÏÂÊ¹ÓÃHAL¿âÓ²¼şIIC
//	HAL_I2C_Mem_Write(&hi2c2, OLED_DEVADDR_WRITE, OLED_MEMADDR_CMD, I2C_MEMADD_SIZE_8BIT, &IIC_Command, 1, 0x100);
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
//   IIC_Start(IIC1);
//   IIC_Send_Byte(0x78,IIC1);			//D/C#=0; R/W#=0
//	IIC_Wait_Ack(IIC1);	
//   IIC_Send_Byte(0x40,IIC1);			//write data
//	IIC_Wait_Ack(IIC1);	
//   IIC_Send_Byte(IIC_Data,IIC1);
//	IIC_Wait_Ack(IIC1);	
//   IIC_Stop(IIC1);
		// ÒÔÉÏÎªÈí¼şIIC£¬ÒÔÏÂÊ¹ÓÃHAL¿âÓ²¼şIIC
//	HAL_I2C_Mem_Write(&hi2c2, OLED_DEVADDR_WRITE, OLED_MEMADDR_DATA, I2C_MEMADD_SIZE_8BIT, &IIC_Data, 1, 0x100);
}
void OLED_WR_Byte(unsigned char dat,unsigned char isData)
{
	if(isData)
   Write_IIC_Data(dat);
	else
   Write_IIC_Command(dat);
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,OLED_CMD);	//page0-page1
		OLED_WR_Byte(0x00,OLED_CMD);		//low column start address
		OLED_WR_Byte(0x10,OLED_CMD);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,OLED_DATA);
			}
	}
}

//×ø±êÉèÖÃ
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//¿ªÆôOLEDÏÔÊ¾    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//¹Ø±ÕOLEDÏÔÊ¾     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//ÇåÆÁº¯Êı,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<4;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //¸üĞÂÏÔÊ¾
}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //¸üĞÂÏÔÊ¾
}
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õı³£ÏÔÊ¾				 
//size:Ñ¡Ôñ×ÖÌå£¬×Ö¿í£º16/8 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t charWidth)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ
		if(x>Max_Column-1){x=0;y=y+2;}
		if(charWidth ==8)
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c][i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c][i+8],OLED_DATA);
		}
		else
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
		}
}
//m^nº¯Êı
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//ÏÔÊ¾Ò»´®10½øÖÆÊı×Ö
//x,y :Æğµã×ø±ê	 
//len :Êı×ÖµÄÎ»Êı
//size:×ÖÌå´óĞ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µş¼ÓÄ£Ê½
//num:ÊıÖµ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)//´Ó¸ßÎ»µ½µØÎ»ÏÔÊ¾
	{
		temp=(num/oled_pow(10,len-t-1))%10;						//»ñÈ¡µ±Ç°Î»µÄÊı×Ö
		if(enshow==0&&t<(len-1))											//Èç¹û»¹Ã»ÓĞÏÔÊ¾¹ıÊı×ÖÇÒ³¤¶ÈÒ²Ã»ÓĞÒç³ö£¬²¢ÇÒ×î¸ßÎ»Ã»ÓĞÊı×Ö
		{
			if(temp==0) 																
			{
				OLED_ShowChar(x+charWidth*t,y,' ',charWidth);	//¾ÍÏÔÊ¾Ò»¸ö¿Õ×Ö·û
				continue;																 	//½áÊøµ¥´ÎÑ­»·
			}else enshow=1;														 	//Èç¹ûÓĞÊı×ÖÏÔÊ¾ºó¾Í²»ÔÙÏÔÊ¾¿Õ×Ö·ûÁË
		 	 
		}
	 	OLED_ShowChar(x+charWidth*t,y,temp+'0',charWidth); 
	}
} 

//ÏÔÊ¾Ò»´®16½øÖÆÊı×Ö
//x,y :Æğµã×ø±ê	 
//len :Êı×ÖµÄÎ»Êı
//size:×ÖÌå´óĞ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µş¼ÓÄ£Ê½
//num:ÊıÖµ(0~4294967295);	 		  
void OLED_ShowNum_HEX(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)//´Ó¸ßÎ»µ½µØÎ»ÏÔÊ¾
	{
		temp=(num/oled_pow(16,len-t-1))%16;						//»ñÈ¡µ±Ç°Î»µÄÊı×Ö
		if(enshow==0&&t<(len-1))											//Èç¹û»¹Ã»ÓĞÏÔÊ¾¹ıÊı×ÖÇÒ³¤¶ÈÒ²Ã»ÓĞÒç³ö£¬²¢ÇÒ×î¸ßÎ»Ã»ÓĞÊı×Ö
		{
			if(temp==0) 																
			{
				OLED_ShowChar(x+charWidth*t,y,' ',charWidth);	//¾ÍÏÔÊ¾Ò»¸ö¿Õ×Ö·û
				continue;																 	//½áÊøµ¥´ÎÑ­»·
			}else enshow=1;														 	//Èç¹ûÓĞÊı×ÖÏÔÊ¾ºó¾Í²»ÔÙÏÔÊ¾¿Õ×Ö·ûÁË
		}
	 	OLED_ShowChar(x+charWidth*t,y,(temp<10)?(temp+'0'):(temp+'0'+7),charWidth); 
	}
}

//ÏÔÊ¾Ò»¸ö×Ö·ûºÅ´®
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t charWidth)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],charWidth);
		x+=charWidth;
		if(x>128-charWidth){x=0;y+=2;}
			j++;
	}
}
//ÏÔÊ¾ºº×Ö£¬×Ô¶¨Òå×Ö¿â£¬·Ç±ê×¼
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾ÏÔÊ¾BMPÍ¼Æ¬128¡Á64ÆğÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
//  if(y1%8==0) y=y1/8;      
//  else y=y1/8+1;
	for(y=y0;y<=y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<=x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//³õÊ¼»¯SSD1306					    
void OLED_Init(void)
{ 	
	HAL_Delay(500);
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
	
	OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
	
	OLED_WR_Byte(0x00,OLED_CMD); /*set display start line*/
	
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/
	OLED_WR_Byte(0x66,OLED_CMD); /*128*/
	
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/
	
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/
	OLED_WR_Byte(0x1F,OLED_CMD); /*duty = 1/32*/
	
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/
	
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/
	OLED_WR_Byte(0x80,OLED_CMD);
	
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/
	OLED_WR_Byte(0x1f,OLED_CMD);
	
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/
	OLED_WR_Byte(0x00,OLED_CMD);
	
	OLED_WR_Byte(0xdb,OLED_CMD); /*set vcomh*/
	OLED_WR_Byte(0x40,OLED_CMD);
	
	OLED_WR_Byte(0x8d,OLED_CMD); /*set charge pump enable*/
	OLED_WR_Byte(0x14,OLED_CMD);
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
	OLED_Clear();
}  


// ÓÃÓÚOLED×Ö·û´®×ª»»µÄÊı×é
char gOLEDSpecialString[100];

char* OLED_S32ToString(int32_t num)
{
	sprintf(gOLEDSpecialString, "%d", num);
	return gOLEDSpecialString;		// ×¢Òâ·µ»ØµÄÖ¸Õë²»ÄÜ±»ÊÍ·ÅÄÚ´æ£¡
}

char* OLED_FloatToString(float num, uint8_t keepDecimalsNum)
{
	char format[] = "%.0f";
	format[2] = '0' + keepDecimalsNum;
	sprintf(gOLEDSpecialString, format, num);
	return gOLEDSpecialString;		// ×¢Òâ·µ»ØµÄÖ¸Õë²»ÄÜ±»ÊÍ·ÅÄÚ´æ£¡
}




/*----------------------------------------------------------------------------**
**OLED´ø»º´æÇøÏÔÊ¾             																							
**----------------------------------------------------------------------------*/
#if OLED_BUFFER == 1
#include <string.h>
#include <stdio.h>

// ÏÔÊ¾»º´æÇø
// Ã¿¸ö×Ö½Ú´ú±íÄ³ĞĞÄ³ÁĞÖĞ´ÓÉÏÖÁÏÂµÄ8¸öÏñËØµã
// 4ĞĞ128ÁĞ£¬¹²4096¸öÏñËØµã£¬512×Ö½Ú
uint8_t gOLEDDispayBuffer[OLED_HEIGHT/8][OLED_WIDTH];

// ´®¿Ú1Á¬½ÓÀ¶ÑÀÄ£¿é£¬I2C2Á¬½ÓOLED£¬ËüÃÇ¹²ÓÃÍ¬ÑùµÄÁ½¸öDMAÍ¨µÀ£»
// Òò´ËÕâÀïÃ»ÓĞÊ¹ÓÃDMA·½Ê½¶ÔOLED·¢Êı¾İ£»
// ÒÔºóµÄ¸Ä½ø£º´®¿Ú2Ã»ÓĞÊ¹ÓÃ£¬¿ÉÒÔ½«À¶ÑÀÄ£¿é¸Ä½Óµ½´®¿Ú2¡£

void OLED_WriteOneByte(uint8_t memAddr, uint8_t byte)
{
//	HAL_I2C_Mem_Write(&hi2c2, OLED_DEVADDR_WRITE, memAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 100);
}

void OLED_WriteBytes(uint8_t memAddr, uint8_t bytes[], uint8_t len)
{
//	HAL_I2C_Mem_Write(&hi2c2, OLED_DEVADDR_WRITE, memAddr, I2C_MEMADD_SIZE_8BIT, bytes, len, 100);
}

void OLED_Buffer_Refresh()
{
	// ĞĞµØÖ·²»»á×ÔÔö£¬ÁĞµØÖ·»á×ÔÔö£¬ÇÒÁĞµØÖ·µ½´ï128ºó»á¹é0£»
	// Òò´ËÖ»ĞèÔÚ±éÀúÇ°½«ÁĞµØÖ·¹é0£¬ÎŞĞëÔÚ±éÀú¹ı³ÌÖĞÔÙ´ÎÉèÖÃ£»
	// ±éÀúÖĞĞĞµØÖ·¸Ä±äÊ±£¬ĞèÒª·¢ËÍĞÂµÄĞĞµØÖ·ÃüÁî¡£
	
	// ÉèÖÃÆğÊ¼ÁĞ£º0ÁĞ¡£0x00+ÁĞµÍ4Î»£¬0x10+ÁĞ¸ß4Î»¡£
	OLED_WriteBytes(OLED_MEMADDR_CMD, (uint8_t[]){0x00, 0x10}, 2);
	
	for(uint8_t row=0; row<OLED_HEIGHT/8; ++row)
	{
		OLED_WriteOneByte(OLED_MEMADDR_CMD, 0xb0 | row);
		OLED_WriteBytes(OLED_MEMADDR_DATA, &gOLEDDispayBuffer[row][0], 128);
	}
}

void OLED_Buffer_Clear()
{
	memset(gOLEDDispayBuffer, 0, sizeof(gOLEDDispayBuffer));
}

void OLED_Buffer_ShowPoint(uint8_t x, uint8_t y, uint8_t color)
{
	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
		return;
		
	if(color)
		gOLEDDispayBuffer[y/8][x] |= 1<<(y%8);
	else
		gOLEDDispayBuffer[y/8][x] &= ~(1<<(y%8));
}

//void OLED_Buffer_Show8PixelsOfColumn(uint8_t x, uint8_t y, uint8_t byte, uint8_t cover)
//{
//	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
//		return;
//	
//	uint8_t page = y/8;
//	uint8_t move = y%8;
//	
//	uint16_t data = gOLEDDispayBuffer[page][x];
//	if(page+1 < OLED_HEIGHT/8)
//		data |= gOLEDDispayBuffer[page+1][x]<<8;
//	
//	if(!cover)
//		data &= ~(0x00FF<<move);
//	
//	data |= byte<<move;
//	
//	gOLEDDispayBuffer[page][x] = data;
//	if(page+1 < OLED_HEIGHT/8)
//		gOLEDDispayBuffer[page+1][x] = data>>8;
//}

void OLED_Buffer_ShowPixelsOfColumn(uint8_t x, uint8_t y, uint8_t byte, uint8_t cover, uint8_t mask)
{
	if(x >= OLED_WIDTH || y >= OLED_HEIGHT)
		return;
	
	uint8_t page = y/8;
	uint8_t move = y%8;
	
	uint16_t data = gOLEDDispayBuffer[page][x];
	if(page+1 < OLED_HEIGHT/8)
		data |= gOLEDDispayBuffer[page+1][x]<<8;
	
	if(!cover)
		data &= ~(mask<<move);
	
	data |= (byte&mask)<<move;
	
	gOLEDDispayBuffer[page][x] = data;
	if(page+1 < OLED_HEIGHT/8)
		gOLEDDispayBuffer[page+1][x] = data>>8;
}

void OLED_Buffer_ShowChar(uint8_t x, uint8_t y, char c, uint8_t charWidth)
{
		unsigned char i=0;	
		c = c-' ';		// ¼ÆËã×Ö·ûÔÚ×Ö¿âÖĞµÄĞòºÅ

		if(charWidth == 8)
		{
			for(i=0;i<8;i++)
				OLED_Buffer_ShowPixelsOfColumn(x+i, y, F8X16[c][i], 1, 0xFF);
			for(i=0;i<8;i++)
				OLED_Buffer_ShowPixelsOfColumn(x+i, y+8, F8X16[c][i+8], 1, 0xFF);
		}
		else if(charWidth == 6)
			for(i=0;i<6;i++)
				OLED_Buffer_ShowPixelsOfColumn(x+i, y, F6x8[c][i], 1, 0xFF);
}

void OLED_Buffer_ShowString(uint8_t x,uint8_t y, char str[], uint8_t charWidth)
{
	for(uint8_t j=0; str[j]!='\0'; )
	{		
		OLED_Buffer_ShowChar(x,y,str[j],charWidth);
		
		x+=charWidth;
		if(x > OLED_WIDTH-charWidth)
		{x=0;y+=8;}
		
		j++;
	}
}

#endif
