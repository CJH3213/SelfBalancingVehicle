//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖÐ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼þ Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : Evk123
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üÐÞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : 0.69´çOLED ½Ó¿ÚÑÝÊ¾Àý³Ì(STM32F103ZEÏµÁÐIIC)
//              ËµÃ÷: 
//              ----------------------------------------------------------------
//              GND   µçÔ´µØ
//              VCC   ½Ó5V»ò3.3vµçÔ´
//              SCL   ½ÓPD6£¨SCL£©
//              SDA   ½ÓPD7£¨SDA£©            
//              ----------------------------------------------------------------
//Copyright(C) ÖÐ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////£

#include "oled.h"
#include "i2c.h"
#include "stdlib.h"
#include "oledfont.h" 
//#include "my_IIC.h"

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
	// ÒÔÉÏÎªÈí¼þIIC£¬ÒÔÏÂÊ¹ÓÃHAL¿âÓ²¼þIIC
	HAL_I2C_Mem_Write(&hi2c2, DEVADDR_OLED_WRITE, 0x00, I2C_MEMADD_SIZE_8BIT, &IIC_Command, 1, 0x100);
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
		// ÒÔÉÏÎªÈí¼þIIC£¬ÒÔÏÂÊ¹ÓÃHAL¿âÓ²¼þIIC
	HAL_I2C_Mem_Write(&hi2c2, DEVADDR_OLED_WRITE, 0x40, I2C_MEMADD_SIZE_8BIT, &IIC_Data, 1, 0x100);
}
void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{
	if(cmd)
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
//ÇåÆÁº¯Êý,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<4;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //¸üÐÂÏÔÊ¾
}
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐµÍµØÖ·
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁÐ¸ßµØÖ·   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //¸üÐÂÏÔÊ¾
}
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õý³£ÏÔÊ¾				 
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
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else
		{
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
		}
}
//m^nº¯Êý
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//ÏÔÊ¾Ò»´®10½øÖÆÊý×Ö
//x,y :Æðµã×ø±ê	 
//len :Êý×ÖµÄÎ»Êý
//size:×ÖÌå´óÐ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µþ¼ÓÄ£Ê½
//num:ÊýÖµ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)//´Ó¸ßÎ»µ½µØÎ»ÏÔÊ¾
	{
		temp=(num/oled_pow(10,len-t-1))%10;						//»ñÈ¡µ±Ç°Î»µÄÊý×Ö
		if(enshow==0&&t<(len-1))											//Èç¹û»¹Ã»ÓÐÏÔÊ¾¹ýÊý×ÖÇÒ³¤¶ÈÒ²Ã»ÓÐÒç³ö£¬²¢ÇÒ×î¸ßÎ»Ã»ÓÐÊý×Ö
		{
			if(temp==0) 																
			{
				OLED_ShowChar(x+charWidth*t,y,' ',charWidth);	//¾ÍÏÔÊ¾Ò»¸ö¿Õ×Ö·û
				continue;																 	//½áÊøµ¥´ÎÑ­»·
			}else enshow=1;														 	//Èç¹ûÓÐÊý×ÖÏÔÊ¾ºó¾Í²»ÔÙÏÔÊ¾¿Õ×Ö·ûÁË
		 	 
		}
	 	OLED_ShowChar(x+charWidth*t,y,temp+'0',charWidth); 
	}
} 

//ÏÔÊ¾Ò»´®16½øÖÆÊý×Ö
//x,y :Æðµã×ø±ê	 
//len :Êý×ÖµÄÎ»Êý
//size:×ÖÌå´óÐ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µþ¼ÓÄ£Ê½
//num:ÊýÖµ(0~4294967295);	 		  
void OLED_ShowNum_HEX(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t charWidth)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)//´Ó¸ßÎ»µ½µØÎ»ÏÔÊ¾
	{
		temp=(num/oled_pow(16,len-t-1))%16;						//»ñÈ¡µ±Ç°Î»µÄÊý×Ö
		if(enshow==0&&t<(len-1))											//Èç¹û»¹Ã»ÓÐÏÔÊ¾¹ýÊý×ÖÇÒ³¤¶ÈÒ²Ã»ÓÐÒç³ö£¬²¢ÇÒ×î¸ßÎ»Ã»ÓÐÊý×Ö
		{
			if(temp==0) 																
			{
				OLED_ShowChar(x+charWidth*t,y,' ',charWidth);	//¾ÍÏÔÊ¾Ò»¸ö¿Õ×Ö·û
				continue;																 	//½áÊøµ¥´ÎÑ­»·
			}else enshow=1;														 	//Èç¹ûÓÐÊý×ÖÏÔÊ¾ºó¾Í²»ÔÙÏÔÊ¾¿Õ×Ö·ûÁË
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
//ÏÔÊ¾ºº×Ö
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
/***********¹¦ÄÜÃèÊö£ºÏÔÊ¾ÏÔÊ¾BMPÍ¼Æ¬128¡Á64ÆðÊ¼µã×ø±ê(x,y),xµÄ·¶Î§0¡«127£¬yÎªÒ³µÄ·¶Î§0¡«7*****************/
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





























