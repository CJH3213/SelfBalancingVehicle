#ifndef __SERIALPORT_H
#define __SERIALPORT_H

/*----------------------------------------------------------------------------*/
/*ͷ�ļ�               																									  		*/
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*ѭ������               																									  	*/
/*----------------------------------------------------------------------------*/
#define SERIALPORT_QUEUE_SIZE 1000

typedef struct
{
	uint8_t mItems[SERIALPORT_QUEUE_SIZE];
	uint32_t mFront;
	uint32_t mRear;
	uint32_t mCount;
} Queue;

/*----------------------------------------------------------------------------*/
/*Ӧ�ò㴮��		               																								*/
/*----------------------------------------------------------------------------*/
// �������շ�������
#define SERIALPORT_BUFF_SIZE 10*2		// ���󻺳��������Լ���IDLE��������ȫ���ж�Ƶ���̶�

typedef struct
{
	UART_HandleTypeDef *mHuart;
	
	// Ӧ�ò���շ�����
	Queue mSerialRxQueue, mSerialTxQueue;
	uint32_t mSerialRxQueueReadyCount;
	
	// �����Ľ��ջ�����
	uint8_t mSerialRxBuff[SERIALPORT_BUFF_SIZE];
	uint32_t mSerialRxBuffNextPos;	// ��¼��һ�δ��Ŀ�ʼ����

	// �����ķ��ͻ�����
	uint8_t mSerialTxBuff[SERIALPORT_BUFF_SIZE];
	uint8_t mIsSerialTxBusy;
	
}SerialPortHandle;

extern SerialPortHandle gSerialPort1, gSerialPort2;

void SerialPortOpen(void);
void SyncSerialRxQueueReadyCountOnIDLE(UART_HandleTypeDef* huart);
void SerialRxBuffCopyToQueue(UART_HandleTypeDef* huart);
void OnUartTransmitCompleteForSerialTx(UART_HandleTypeDef* huart);
void OnUartErrorForSerial(UART_HandleTypeDef *huart);
uint32_t SerialRead(SerialPortHandle *hSerialPort, uint8_t* pData, uint32_t pLen);
uint32_t SerialWrite(SerialPortHandle *hSerialPort, uint8_t* pData, uint32_t len);
void Test(SerialPortHandle *hSerialPort);

#endif
