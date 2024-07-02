#ifndef __SYN7318_H
#define __SYN7318_H
//#include "stm32f4xx.h"
#include "main.h"

#define BITBAND_REG(REG, BIT) (*((uint32_t volatile*)(0x42000000u + (((uint32_t)&(REG)-(uint32_t)0x40000000u)<<5) + (((uint32_t)(BIT))<<2))))
#define LED1 BITBAND_REG(GPIOC->ODR, 13)

extern uint8_t Back[4];   //接收命令回传的数组
extern uint8_t ASR[6];    //接收识别结果回传的数组
extern uint8_t S[4];      //接收模块当前工作状态回传的数组

enum SYN_SetRageType{near=0x01,middle=0x02,far=0x03};		//识别距离：0.2米内、0.2~3米、大于3米

void SYN7318_Init(void);
uint8_t SYN7318_Rst(void);  //语音模块复位
void Status_Query(void);		//模块状态查询
void SYN_TTS(uint8_t *Pst);		//语音合成播放
void Start_ASR(uint8_t Dict);		//启动语音识别
void SYN_SetParameter(enum SYN_SetRageType SYN_SetRage, uint16_t MaxQuiet, uint16_t MaxVoice, uint8_t RejectionLevel);		//设置模块参数
void SYN_SetWakeUPName(char *Name);		//设置自定义唤醒名

void SYN7318_Test( void);  // 开启语音测试
void Yu_Yin_Asr(void);  // 语音识别处理函数

#endif

