#ifndef __SYN7318_H
#define __SYN7318_H
//#include "stm32f4xx.h"
#include "main.h"

#define BITBAND_REG(REG, BIT) (*((uint32_t volatile*)(0x42000000u + (((uint32_t)&(REG)-(uint32_t)0x40000000u)<<5) + (((uint32_t)(BIT))<<2))))
#define LED1 BITBAND_REG(GPIOC->ODR, 13)

extern uint8_t Back[4];   //��������ش�������
extern uint8_t ASR[6];    //����ʶ�����ش�������
extern uint8_t S[4];      //����ģ�鵱ǰ����״̬�ش�������

enum SYN_SetRageType{near=0x01,middle=0x02,far=0x03};		//ʶ����룺0.2���ڡ�0.2~3�ס�����3��

void SYN7318_Init(void);
uint8_t SYN7318_Rst(void);  //����ģ�鸴λ
void Status_Query(void);		//ģ��״̬��ѯ
void SYN_TTS(uint8_t *Pst);		//�����ϳɲ���
void Start_ASR(uint8_t Dict);		//��������ʶ��
void SYN_SetParameter(enum SYN_SetRageType SYN_SetRage, uint16_t MaxQuiet, uint16_t MaxVoice, uint8_t RejectionLevel);		//����ģ�����
void SYN_SetWakeUPName(char *Name);		//�����Զ��廽����

void SYN7318_Test( void);  // ������������
void Yu_Yin_Asr(void);  // ����ʶ������

#endif

