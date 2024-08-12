#ifndef __SERIALPORT_H
#define __SERIALPORT_H

/*----------------------------------------------------------------------------*/
/*头文件               																									  		*/
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*循环队列               																									  	*/
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
/*应用层串口		               																								*/
/*----------------------------------------------------------------------------*/
// 物理层的收发缓冲区
#define SERIALPORT_BUFF_SIZE 10*2		// 增大缓冲区，可以减少IDLE、半满、全满中断频繁程度

typedef struct
{
	UART_HandleTypeDef *mHuart;
	
	// 应用层的收发队列
	Queue mSerialRxQueue, mSerialTxQueue;
	uint32_t mSerialRxQueueReadyCount;
	
	// 物理层的接收缓冲区
	uint8_t mSerialRxBuff[SERIALPORT_BUFF_SIZE];
	uint32_t mSerialRxBuffNextPos;	// 记录下一次从哪开始复制

	// 物理层的发送缓冲区
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
