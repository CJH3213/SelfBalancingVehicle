/*****************************************************************
*例程来源：百科融创官网主车实例
*任务板SYN7318语音模块测试
*****************************************************************/

//#include "stm32f4xx.h"
#include "syn7318.h"
#include "delay.h"
#include <string.h>
//#include "LED.h"

//#include "main.h"
#include "usart.h"


uint8_t Back[4] = {0};   //接收命令回传的数组
uint8_t ASR[6] = {0};    //接收识别结果回传的数组
uint8_t S[4] = {0};      //接收模块当前工作状态回传的数组

unsigned char Wake_Up[] = {0xfd,0x00,0x02,0x51,0x1f};		//开始语音唤醒，识别词条更新
unsigned char Stop_Wake_Up[] = {0xFD,0x00,0x01,0x52};		//停止语音唤醒

unsigned char Start_ASR_Buf[] = {0xFD,0x00,0x02,0x10,0x03};		//开始语音识别，暂停播放
unsigned char Stop_ASR_Buf[] = {0xFD,0x00,0x01,0x11};		//停止语音识别

unsigned char Play_MP3[] ={ 0xFD,0x00,0x1E,0x01,0x01,0xC6,0xF4,0xB6,0xAF,0xD3, 0xEF ,0xD2, 0xF4,
                            	0xBF, 0xD8, 0xD6 ,0xC6 ,0xBC ,0xDD ,0xCA ,0xBB ,0xA3 ,0xAC, 0xC7, 0xEB,
                             	0xB7, 0xA2, 0xB3, 0xF6 ,0xD6, 0xB8, 0xC1, 0xEE };
unsigned char Ysn7813_flag=0;

#define SYN7318_RST_H GPIOA->BSRR = GPIO_PIN_12
#define SYN7318_RST_L GPIOA->BRR = GPIO_PIN_12


void Yu_Yin_Asr(void);

//void USART6_Init(uint32_t baudrate)
//{
//	GPIO_InitTypeDef  GPIO_TypeDefStructure;
//	USART_InitTypeDef USART_TypeDefStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
//	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
//	
//	//PC6-Tx
//	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF;		//复用功能
//	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
//	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;     //上拉
//	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
//	
///*	//PC7-RX
//	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AF;		//输入
//	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP; 	//上拉
//	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);*/
//	
//	
//	USART_TypeDefStructure.USART_BaudRate = baudrate;					   //波特率
//	USART_TypeDefStructure.USART_HardwareFlowControl = 				       //无硬件控制流
//												 USART_HardwareFlowControl_None;  
//	USART_TypeDefStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; 		//接收与发送模式
//	USART_TypeDefStructure.USART_Parity = USART_Parity_No; 		       		//无校验位
//	USART_TypeDefStructure.USART_StopBits = USART_StopBits_1;        		//停止位1
//	USART_TypeDefStructure.USART_WordLength = USART_WordLength_8b;   		//数据位8位
//	USART_Init(USART6,&USART_TypeDefStructure);
//	
//	USART_Cmd(USART6,ENABLE);
//	USART_ClearFlag(USART6, USART_FLAG_TC);						//清除发送完成标志位
//	USART_ClearFlag(USART6, USART_FLAG_RXNE);					//清除接收完成标志位

//}


//发送一个字符
int U6SendChar(int ch) 
{
//	while(!(USART_GetFlagStatus(USART6,USART_FLAG_TXE)));
//	USART_SendData(USART6,ch&0x1FF);
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	return (ch);
}
/*********************************************************************
【函 数 名】：SYN7318_Put_Char----发送一个字节函数
【参数说明】：txd---待发送的字节（8位）
【简    例】：SYN7318_Put_Char('d');  发送‘d’
*********************************************************************/
void SYN7318_Put_Char(uint8_t txd)
{
	  U6SendChar( txd );
}

/*********************************************************************
【函 数 名】：SYN7318_Put_String----发送字符串函数
【参数说明】：Pst：存放字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0x00,0x01,0x02,0x03};
              SYN7318_Put_String(d,4); ---发送数组d中的元素
*********************************************************************/
void SYN7318_Put_String(uint8_t* Pst,uint8_t Length)
{ 
   	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		  SYN7318_Put_Char( Pst[i]);
	}
}

/*********************************************************************
【函 数 名】：SYN7318_Get_char----接收一个字节函数
【参数说明】：无参
【返 回 值】：接收到的字节
【简    例】：uchar d;
              d = SYN7318_Get_char();
              SYN7318_Get_char(d); -----输出接收到的字节
*********************************************************************/

uint8_t SYN7318_Get_char(void) 
{
  uint8_t return_data=0;	
//	while(!HAL_UART_GetState(&huart1));
	HAL_UART_Receive(&huart1, &return_data, 1, 0xffff);
	return return_data;	
}

/*********************************************************************
【函 数 名】：SYN7318_Get_String----接收字符串函数
【参数说明】：Pst：存放接收到的字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0};
              SYN7318_Get_String(d,4);
              SYN7318_Get_String(d,4);----输出接收到的字符串 
*********************************************************************/
void SYN7318_Get_String(unsigned char *Pst,unsigned char Length)
{
//  unsigned char i;
//  for(i=0;i<Length;i++)
//  {
//     Pst[i] = SYN7318_Get_char();
//  }
		HAL_UART_Receive(&huart1, Pst, Length, 0xffff);
}

uint8_t  SYN7318_Rst(void)  //语音模块复位
{
	SYN7318_RST_H;	
	delay_ms(10); 
	SYN7318_RST_L;	
	delay_ms(100);	
	SYN7318_RST_H;  
	
	while ( 0x55 == SYN7318_Get_char() );
	SYN7318_Get_String(Back ,3 );
	if(Back[2] ==0x4A )  return 1;
	else return 0;
	
}


void SYN7318_Init(void)
{
//	USART6_Init(115200);
//	
//	GPIO_InitTypeDef  GPIO_TypeDefStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

//	//PB9 -- SYN7318_RESET
//	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;	//复用功能
//	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
//	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;     //上拉
//	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOB,&GPIO_TypeDefStructure);
//	
//	GPIO_SetBits(GPIOB,GPIO_Pin_9);						//默认为高电平
	
}
/*********************************************************************
【函 数 名】：SYN_TTS----语音合成播放函数
【参数说明】：Pst：存放要合成播放的文本的数组名
【简    例】：uchar Data[] = {"北京龙邱"};
              SYN_TTS(Data); -----合成播放北京龙邱
*********************************************************************/
void SYN_TTS(uint8_t *Pst)
{
	uint8_t Length;
	uint8_t Frame[5];   //保存发送命令的数组
	
	Length = strlen((char*)Pst); 
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //语音合成播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
	SYN7318_Put_String(Frame, 5);	
	SYN7318_Put_String(Pst, Length);
	
	SYN7318_Get_String(Back,4);	
	//接收成功
	while(!(Back[3] == 0x41))
	{
		SYN7318_Get_String(Back,4);
	}
		
	//空闲监测
	SYN7318_Get_String(Back,4);
	while(!(Back[3] == 0x4f))
	{
		SYN7318_Get_String(Back,4);
	}	
}


/*********************************************************************
【函 数 名】：Start_ASR----开始语音识别函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); ----识别0x00词典中的词条
*********************************************************************/
void Start_ASR(uint8_t Dict)
{

	uint8_t Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //开始语音识别命令
	Frame[4] = Dict;      //词典编号  在这里修改想要识别的词典编号
	
	ASR[3]=0;
	
	SYN7318_Put_String(Frame, 5);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 SYN7318_Get_String(Back,3);  //语音识别命令回传结果
		if(Back[0] == 0xfc)
		{
			 SYN7318_Get_String(ASR,Back[2]);
		}
	}
}

/*********************************************************************
【函 数 名】：Stop_ASR----停止语音识别函数
【参数说明】：无参
*********************************************************************/
void Stop_ASR()
{
	uint8_t Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //停止语音识别命令
	
  SYN7318_Put_String(Frame, 4);	
  SYN7318_Get_String(Back,4); 

}

/***************************************************************************
【函 数 名】：Status_Query----模块状态查询函数
【参数说明】：无参
【简    例】：Status_Query(); 
***************************************************************************/
void Status_Query()
{
	uint8_t Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;    //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //状态查询命令
	
	SYN7318_Put_String(Frame, 4);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 SYN7318_Get_String(S,4);  //模块当前工作状态的回传结果
	}
}

/***************************************************************************
【函 数 名】：SYN_SetParameter----设置模块参数
【参数说明】：识别距离、静音上限、语音上限、拒识级别
【简    例】：
***************************************************************************/
void SYN_SetParameter(enum SYN_SetRageType SYN_SetRage, uint16_t MaxQuiet, uint16_t MaxVoice, uint8_t RejectionLevel)
{
	uint8_t Frame[10];		//保存发送命令的数组
	
	//保护数据范围，超出设为默认值
	if((uint8_t)SYN_SetRage<1 || (uint8_t)SYN_SetRage>3) SYN_SetRage = middle;
	if(MaxQuiet && (MaxQuiet <1000 || MaxQuiet>30000))	MaxQuiet = 4000;
	if(MaxVoice<1000 || MaxVoice>6000) MaxVoice = 4000;
	if(RejectionLevel<1 || RejectionLevel>5) RejectionLevel = 5;
	
	Frame[0] = 0xFD;
	Frame[1] = 0x00;
	Frame[2] = 0x07;
	Frame[3] = 0x1E;
	Frame[4] = (uint8_t)SYN_SetRage;
	Frame[5] = MaxQuiet>>8;
	Frame[6] = (uint8_t)MaxQuiet;
	Frame[7] = MaxVoice>>8;
	Frame[8] = (uint8_t)MaxVoice;
	Frame[9] = RejectionLevel;
	
	SYN7318_Put_String(Frame, 10);	
	SYN7318_Get_String(Back,4); 
}

/***************************************************************************
【函 数 名】：SYN_SetWakeUPName----设置自定义唤醒名
【参数说明】：唤醒名
【简    例】：
***************************************************************************/
void SYN_SetWakeUPName(char *Name)
{
	uint8_t Length;
	uint8_t Frame[5];   //保存发送命令的数组
	
	Length = strlen(Name); 
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x53;      //语音合成播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
	SYN7318_Put_String(Frame, 5);	
	SYN7318_Put_String((uint8_t *)Name, Length);
	
	SYN7318_Get_String(Back,4);
	if(Back[3] == 0x41)
	{
		SYN7318_Get_String(Back,4);  //语音识别命令回传结果
		//接收成功
		if(Back[3] == 0x23)
		{
			SYN_TTS((uint8_t *)"自定义唤醒名设置成功");
		}
		else	if(Back[3] == 0x24)
		{
			SYN_TTS((uint8_t *)"自定义唤醒名设置失败");
		}
		else
			SYN_TTS((uint8_t *)"自定义唤醒名回传错误");		
	}
}


void SYN7318_Test( void)  // 开启语音测试
{   	
	 Ysn7813_flag = 1;
//	 SYN7318_Init();
		
		 SYN_TTS((uint8_t *)"请说语音驾驶唤醒");
		  LED1 = 0;
	
		  Status_Query();  //查询模块当前的工作状态
			if(S[3] == 0x4F)  //模块空闲即开启唤醒
			{
//				SYN_TTS((uint8_t *)"模块空闲");
				LED1 = 0;
				delay_ms(1);
				
				SYN7318_Put_String(Wake_Up,5);//发送唤醒指令
				SYN7318_Get_String(Back,4);   //接收反馈信息
				if(Back[3] == 0x41)         //接收成功
				{ 
//					SYN_TTS((uint8_t *)"唤醒指令成功");
					LED1 = 0;
					SYN7318_Get_String(Back,3); //接收前三位回传数据
					if(Back[0] == 0xfc)       //帧头判断
					{
//						SYN_TTS((uint8_t *)"帧头判断0xfc正确");
						LED1 = 0;
						SYN7318_Get_String(ASR,Back[2]);//接收回传数据
						if(ASR[0] == 0x21)            //唤醒成功
						{
//							SYN_TTS((uint8_t *)"唤醒成功");
							SYN7318_Put_String(Play_MP3,33);//播放“我在这” 

							SYN7318_Get_String(Back,4);
							SYN7318_Get_String(Back,4);
							while(!(Back[3] == 0x4f))    //等待空闲
							{
								LED1 = ~LED1;
								delay_ms(500);
							}
							Ysn7813_flag = 1;
         
							//开始语音识别
							while(Ysn7813_flag)
							{
									LED1 = ~LED1;
//									delay_ms(500);
								SYN7318_Put_String(Start_ASR_Buf,5);//发语音识别命令
								SYN7318_Get_String(Back,4);     //接收反馈信息
								if(Back[3] == 0x41)           //接收成功
								{
									LED1 = ~LED1;             //LED1反转
									SYN7318_Get_String(Back,3);  //语音识别命令回传结果
									if(Back[0] == 0xfc)        //帧头判断
									{
										LED1 = ~LED1;
										SYN7318_Get_String(ASR,Back[2]);//接收回传数据	                    
										Yu_Yin_Asr();
									}
								}
							}
							SYN7318_Put_String(Stop_Wake_Up,4);//发送停止唤醒指令
						}
						else                          //唤醒内部错误
						{
						
						}
					}
				}				
		  }
}


void Yu_Yin_Asr(void)  // 语音识别处理函数
{
			switch(ASR[0])
				 {
						case 0x01:
							{
									switch(ASR[3])
											{
												case 0x00:  // 第一个词一般不用于识别
													{
										  				  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															  break;
													}
													case 0x01:  // 前进
													{
//															Go_Test( 50,50);
															SYN_TTS("小车前进已完成");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x02:
													{

//															Back_Test(50,50);
													 		SYN_TTS("小车后退已完成，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别 
															break;
													}
													case 0x03:
													{
 
//														  Left_Test( 80);
															SYN_TTS("小车左转完成，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别;
															break;
													}
													case 0x04:
													{
													   
													   
//															Right_Test( 80);
															SYN_TTS("小车右转完成，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x05:
													{
														  													    
//															 Stop_Test();
															 SYN_TTS("已停车，等待下一步指令");
														    SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															 break;
													}
													case 0x06:
													{
//															  Track_Test( 50);
																SYN_TTS("寻迹已完成，等待下一步指令");
																SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															  break;
													}
													case 0x07:
													{
					
														    SYN_TTS("报警器已打开，等待下一步指令");
														    SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x08:
													{

														    SYN_TTS("道闸已打开，等待下一步指令");
														    SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															 break;
													}
													case 0x09:
													{
														    

														    SYN_TTS("道闸已关闭，等待下一步指令");
														    SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															  break;
													}
													case 0x0a:
													{

														    SYN_TTS("LED显示已开，等待下一步指令");
																SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别										
															  break;
													}
													case 0x0b:
													{

														    SYN_TTS("计时系统已打开，等待下一步指令");
 														    SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															  break;
													}
													case 0x0c:
													{
														  

														  SYN_TTS("计时系统已关闭，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x0d:
													{
														  

														  SYN_TTS("LED显示距离已完成，等待下一步指令");
                                  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x0e:
													{
									//						LED_L =0;  // 开左灯
														  SYN_TTS("左侧提示灯已打开，等待下一步指令");
														SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别

															break;
													}
													case 0x0f:
													{
									//						LED_R =0;  // 开右灯
														  SYN_TTS("右侧提示灯已打开，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别														
															break;
													}
													case 0x10:
													{

									//						LED_L =1;  // 关闭提示灯
									//						LED_R =1;
														  SYN_TTS("提示灯已关闭，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x11:
													{
														  
									//						BEEP =0;  //开蜂鸣器
														  SYN_TTS("蜂鸣器已打开，等待下一步指令");
														  
															delay_ms(500);
									//						BEEP =1;
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x12:
													{
														  
									//						BEEP =1;  //关蜂鸣器
														  SYN_TTS("蜂鸣器已关闭，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x13:
													{
														    SYN_TTS("车牌已显示，等待下一步指令");
															SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x14:
													{
														   SYN_TTS("隧道排风系统已打开，等待下一步指令");
														   SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
														   break;
													}
													case 0x15:
													{
														  SYN_TTS("图片翻页完成，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
														  break;
													}
													case 0x16:
													{
														  SYN_TTS("调[=tiao2]光档[=dang3]位已加1，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
														  break;
													}
													case 0x17:
													{
														  																										
														  SYN_TTS("欢迎使用北京百科融[=rong2]创语音智能小车");
                               SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别					
															break;
													}
													case 0x18:
													{
														  																										
														  SYN_TTS("开始原地掉头，请稍后");
																			
/*															while(G_Flag);  //等待前进完成
															Left_Test( 80); 
															while(L_Flag);
															Left_Test( 80); 
															while(L_Flag);*/
															SYN_TTS("原地掉头已完成，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别	
															break;
													}
													case 0x19:
													{
														 
															SYN_TTS("好的，结束本次语音控制"); 
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												
														  LED1 = 0;
														  LED1 = 0;
														  Ysn7813_flag = 0;
//Ysn7813_flag_cs =0;
															break;
													}
	                        case 0x1A:  // 小车前进
													{
														 

//															Go_Test( 80,10);
															SYN_TTS(" 小车前进已完成，等待下一步指令");
															SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x1B:   //  小车后退
													{
													  	SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别 

//															Back_Test(80,10);
															SYN_TTS("小车后退已完成，等待下一步指令");	
															break;
													}
													case 0x1C:   //  小车左转
													{
														  
														  
//														   Left_Test( 80);
															 SYN_TTS("小车左转已完成，等待下一步指令");
                               SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别;
															break;
													}
													case 0x1D:  // 小车右转
													{
														  
//															Right_Test( 80);
															SYN_TTS("小车右转已完成，等待下一步指令");
														SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别

															break;
													}
													case 0x1E:  // 小车停止
													{
														  
														    
//															 Stop_Test();
															SYN_TTS("已停车，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
													case 0x1F:   //  小车循迹
													{
														 

//															Track_Test( 50);
															SYN_TTS("小车寻迹已完成，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}	
                          case 0x20:   //  向右转弯
													{
														  

//															Right_Test( 80);
															SYN_TTS("右转弯已完成，等待下一步指令");
														  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}														
                          case 0x21:   //  禁止右转
													{
														  
//															Left_Test( 80);
															SYN_TTS("禁止右转，执行左转弯完成，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
														}
                          case 0x22:   //  向左转弯
													{
														  
//															Left_Test( 80);
															SYN_TTS("左转弯已完成，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}														
                          case 0x23:   //  禁止右转
													{
														  
//															Left_Test( 80);
															SYN_TTS("禁止左转，执行右转弯完成，等待下一步指令");
                              SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;													
													}
													default:
													{
														  
															SYN_TTS("对不起，我没听清");
													   	SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
															break;
													}
												}
                        SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											case 0x02: //识别成功（无命令ID号）
											{
												SYN_TTS("对不起，我没听清");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											case 0x03://用户静音超时
											{
												SYN_TTS("进入休眠，随时为您服务");
												Ysn7813_flag = 0;
//												Ysn7813_flag_cs = 0;
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											case 0x04:
											{
												SYN_TTS("请安静，麻烦您再说一遍");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											case 0x05:
											{
												SYN_TTS("对不起，请再说一遍");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											case 0x06:
											{
												SYN_TTS("识别内部错误");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											case 0x07:
											{
												SYN_TTS("对不起，请再说一遍");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
											default:
											{
												SYN_TTS("错误");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												break;
											}
									}

}


//uint8_t Start_ASR_One_Buf[] = {0xFD,0x00,0x02,0x10,0x04};//  单次测试 使用用户词典2
////static uint8_t YY_HXC[8]={0x55 ,0x06 ,0x10 ,0x01 ,0x00 ,0x00 ,0x11 ,0xbb};  // 唤醒词
//static uint8_t YY_ZZW[8]={0x55 ,0x06 ,0x10 ,0x04 ,0x00 ,0x00 ,0x14 ,0xbb};  // 左转弯
//static uint8_t YY_SJ [8]={0x55 ,0x06 ,0x20 ,0x01 ,0x00 ,0x00 ,0x21 ,0xbb};  // 左转弯
///****************************************************************
//函数功能：全自动语音识别
//参	  数: mode =  1 随机指令  mode != 1 指定命令  num = 指定播报指令
//返回  值：无
//*****************************************************************/
//void SYN_7318_One_test(uint8_t mode,uint8_t num)  // mode 模式测试 1 随机指令 0 指定指令
//{
//	uint8_t error_num=0;
//	uint8_t str_test[5];
//	Ysn7813_flag = 1;
//	uint32_t max_tim = 0;
//	SYN7318_Init();
//	
//	while(Ysn7813_flag)
//	{
//		SYN7318_Rst();
//		Status_Query();  //查询模块当前的工作状态
//        sprintf((char*)str_test,"%d",S[3]);
//		Send_InfoData_To_Fifo(str_test,5);		
//		if(S[3] != 0x4F)  //模块空闲即开启唤醒  
//		{
//			SYN_TTS("模块忙碌");
//			max_tim = gt_get() + 2000;			
//			while(!SYN7318_Rst() && gt_get_sub(max_tim));
//			SYN_TTS("模块复位成功！");
//		}
//		delay_ms(100);
//		SYN7318_Put_String(Start_ASR_One_Buf,5);//发语音识别命令  // 用户词典 2  Start_ASR_One_Buf
//		SYN7318_Get_String(Back,4);     //接收反馈信息
//		if(Back[3] == 0x41)           //接收成功
//		{
//			LED1 = ~LED1;             //LED1反转
//			if(mode)
//			{
//				YY_ZZW[3] = num;
//				//Send_ZigBee_Info( YY_SJ,8); // 随机语音
////				Send_ZigbeeData_To_Fifo( YY_SJ,8);
//			}
//			else  
//			{
//				YY_ZZW[3] = num;
//				//Send_ZigBee_Info( YY_ZZW,8); // 指定语音
//				Send_ZigbeeData_To_Fifo( YY_ZZW,8);
//			}

//			SYN7318_Get_String(Back,3);  //语音识别命令回传结果
//			if(Back[0] == 0xfc)        //帧头判断
//			{
//				LED1 = ~LED1;
//				SYN7318_Get_String(ASR,Back[2]);//接收回传数据

//				if(ASR[0] ==0x01)
//				{
//					switch(ASR[5]) // 命令ID
//					{
//						case 0x02:   //  原地掉头
//						{
//							SYN_TTS("识别成功美好生活");
//							SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别							 
//							Ysn7813_flag =0;
//							break;
//						}														
//						case 0x03:   //  向右转弯
//						{								 
//							SYN_TTS("识别成功秀丽山河");						  
//							SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别							 
//							Ysn7813_flag =0;
//							break;
//						}														
//						case 0x04:   //  禁止右转
//						{														  															
//							SYN_TTS("识别成功追逐梦想");
//							SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别							
//							Ysn7813_flag =0;
//							break;
//						}
//						case 0x05:   //  左侧行驶
//						{
//							SYN_TTS("识别成功扬帆起航");
//							SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别							
//							Ysn7813_flag =0;
//							break;
//						}														
//						case 0x06:   //  左行被禁
//						{														  	
//							SYN_TTS("识别成功齐头并进");
//							SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别							
//							Ysn7813_flag =0;
//							break;													
//						}
//						default:
//						{														  
//							SYN_TTS("对不起，我没听清,再发一次");
//							SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
//							//Ysn7813_flag =0;
//							break;
//						}
//					}
//				}
//				else SYN_TTS("识别失败，再来一次！");     
//			}
//		}			
//		/*************************************************/
//		//SYN7318_Put_String(Stop_Wake_Up,4);//发送停止唤醒指令
//		LED1 = !LED1;
//		error_num++;	
//		if(error_num > 3) 
//			Ysn7813_flag = 0;	
//	}			
//}






