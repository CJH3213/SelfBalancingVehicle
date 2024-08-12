#include "SerialPort.h"
#include "usart.h"
#include <stdio.h>

/**** �����ඨ�岿�� ****/

void InitQueue(Queue *q)
{
	q->mFront = 0;
	q->mRear = (uint32_t)-1;
	q->mCount = 0;
}

#define IsQueueEmpty(q) ((q)->mCount == 0)
#define IsQueueFull(q) ((q)->mCount >= SERIALPORT_QUEUE_SIZE)

uint32_t Enqueue(Queue *q, uint8_t *values, uint32_t len)
{
	if(q->mCount >= SERIALPORT_QUEUE_SIZE || len == 0)
		return 0;
	
	if(len > SERIALPORT_QUEUE_SIZE-q->mCount)
		len = SERIALPORT_QUEUE_SIZE-q->mCount;
	
	for(uint32_t i=0; i < len; i++)
	{
		q->mRear = (q->mRear + 1)%SERIALPORT_QUEUE_SIZE;
		q->mItems[q->mRear] = values[i];
	}

	q->mCount += len;
	return len;
}

uint32_t Dequeue(Queue *q, uint8_t *values, uint32_t len)
{
	if(q->mCount <= 0)
		return 0;
	
	if(q->mCount < len)
		len = q->mCount;
	
	for(uint32_t i=0; i < len; i++)
	{
		values[i] = q->mItems[q->mFront];
		q->mFront = (q->mFront + 1) % SERIALPORT_QUEUE_SIZE;
	}
	q->mCount -= len;
	return len;
}




/**** ����DMA������ ****/

SerialPortHandle gSerialPort1, gSerialPort2;


void InitSerialPortHandle(SerialPortHandle* hSerialPort, UART_HandleTypeDef* huart)
{
	hSerialPort->mHuart = huart;
	hSerialPort->mSerialRxQueueReadyCount = 0;
	hSerialPort->mSerialRxBuffNextPos = 0;
	hSerialPort->mIsSerialTxBusy = 0;
	InitQueue(&(hSerialPort->mSerialRxQueue));
	InitQueue(&(hSerialPort->mSerialTxQueue));
	
	__HAL_UART_ENABLE_IT(hSerialPort->mHuart, UART_IT_IDLE);
	HAL_UART_Receive_DMA(hSerialPort->mHuart, hSerialPort->mSerialRxBuff, SERIALPORT_BUFF_SIZE);
}

SerialPortHandle* GetSerialPortHandleByUartHandle(UART_HandleTypeDef* huart)
{
	if(huart == &huart1)
		return &gSerialPort1;
//	if(huart == &huart2)
//		return &gSerialPort2;
	return NULL;
}

void SerialPortOpen()
{
	InitSerialPortHandle(&gSerialPort1, &huart1);
//	InitSerialPortHandle(&gSerialPort2, &huart2);
}

void SyncSerialRxQueueReadyCountOnIDLE(UART_HandleTypeDef* huart)
{
	SerialPortHandle *hSerialPort = GetSerialPortHandleByUartHandle(huart);
	if(hSerialPort == NULL)
		return;
	
	hSerialPort->mSerialRxQueueReadyCount = hSerialPort->mSerialRxQueue.mCount;
}

void SerialRxBuffCopyToQueue(UART_HandleTypeDef* huart)
{
	SerialPortHandle *hSerialPort = GetSerialPortHandleByUartHandle(huart);
	if(hSerialPort == NULL)
		return;
	
	// ��ȡbuff������Ϊ����ĩβ+1λ�ã�ע����ȫ���ж�ʱΪ0
	uint32_t currentPos = SERIALPORT_BUFF_SIZE - __HAL_DMA_GET_COUNTER(hSerialPort->mHuart->hdmarx);
	
	if(currentPos == hSerialPort->mSerialRxBuffNextPos)
	{
		// ���������IDLE�жϣ�����û���յ��µ����ݣ�˵�����ڳ�����
		// �����յ����������ڲ����ʻ��������ò���ȷ�Լ��ܵ�����ʱ
		// ���ھͻ�������״̬��ͬʱ����HAL_UART_ErrorCallback()
		// ���粨���ʲ�ƥ��Ĵ���״̬��HAL_UART_ERROR_FE��֡����
		// OnUartErrorForSerial()�������ã���������
		return;
	}
	
	// ��� currentPos С�� mSerialRxBuffLastPos��˵�������˻���
	else if (currentPos < hSerialPort->mSerialRxBuffNextPos)
	{
			Enqueue(&hSerialPort->mSerialRxQueue, hSerialPort->mSerialRxBuff + hSerialPort->mSerialRxBuffNextPos, 
				SERIALPORT_BUFF_SIZE - hSerialPort->mSerialRxBuffNextPos);
	}
	else
			Enqueue(&hSerialPort->mSerialRxQueue, hSerialPort->mSerialRxBuff + hSerialPort->mSerialRxBuffNextPos, 
				currentPos - hSerialPort->mSerialRxBuffNextPos);
		
	hSerialPort->mSerialRxBuffNextPos = currentPos;
}

void SerialTxQueueCopyToBuff(SerialPortHandle *hSerialPort)
{
	uint32_t len = hSerialPort->mSerialTxQueue.mCount;
	if(len == 0)
		return;
	if(len > SERIALPORT_BUFF_SIZE)
		len = SERIALPORT_BUFF_SIZE;
	
	hSerialPort->mIsSerialTxBusy = 1;
	
	Dequeue(&hSerialPort->mSerialTxQueue, hSerialPort->mSerialTxBuff, len);
	
	HAL_UART_Transmit_DMA(hSerialPort->mHuart, hSerialPort->mSerialTxBuff, len);
}

void OnUartTransmitCompleteForSerialTx(UART_HandleTypeDef* huart)
{
	SerialPortHandle *hSerialPort = GetSerialPortHandleByUartHandle(huart);
	if(hSerialPort == NULL)
		return;
	
	hSerialPort->mIsSerialTxBusy = 0;
	SerialTxQueueCopyToBuff(hSerialPort);	// ������ɺ��ټ��һ�·��Ͷ����Ƿ�������
}

void OnUartErrorForSerial(UART_HandleTypeDef *huart)
{
	SerialPortHandle *hSerialPort = GetSerialPortHandleByUartHandle(huart);
	if(hSerialPort == NULL)
		return;
	
	// ���ڴ���״̬����������������
	if(hSerialPort->mHuart->ErrorCode != HAL_UART_ERROR_NONE)
	{
		hSerialPort->mSerialRxBuffNextPos = 0;
		HAL_UART_Receive_DMA(hSerialPort->mHuart, hSerialPort->mSerialRxBuff, SERIALPORT_BUFF_SIZE);
	}
}


/**** �û�Ӧ�ýӿڲ��� ****/

//void ByteCopy(uint8_t* src, uint8_t* dst, uint32_t num)
//{
//	for(uint8_t* end = src + num; src < end; src ++)
//		*(dst++) = *src;
//}

uint32_t SerialRead(SerialPortHandle *hSerialPort, uint8_t* pData, uint32_t len)
{
	if(hSerialPort == NULL || pData == NULL || len == 0 || hSerialPort->mSerialRxQueueReadyCount == 0)
		return 0;
	
	if(hSerialPort->mSerialRxQueueReadyCount < len)
		len = hSerialPort->mSerialRxQueueReadyCount;
	hSerialPort->mSerialRxQueueReadyCount -= len;
	
	len = Dequeue(&hSerialPort->mSerialRxQueue, pData, len);
	
	return len;
}

uint32_t SerialWrite(SerialPortHandle *hSerialPort, uint8_t* pData, uint32_t len)
{
	if(hSerialPort == NULL || pData == NULL || len == 0 || IsQueueFull(&hSerialPort->mSerialTxQueue))
		return 0;
	
	len = Enqueue(&hSerialPort->mSerialTxQueue, pData, len);
	
	if(hSerialPort->mIsSerialTxBusy == 0)
		SerialTxQueueCopyToBuff(hSerialPort);
	
	return len;
}


void Test(SerialPortHandle *hSerialPort)
{
	static uint32_t tLen = 0;
	static uint8_t tData[100];
	
	// ���ղ���
	tLen = SerialRead(hSerialPort, tData, 0xFFFF);
	if(tLen > 0)
	{
		//mSerialTxBuff[0] = gSerialRxQueue.mFront;
		// HAL_UART_Transmit_DMA(mHuart, tData, tLen);
		SerialWrite(hSerialPort, tData, tLen);
	}
}

// �ض���printf��������ڶ���
int fputc(int ch, FILE *f)
{
	SerialWrite(&gSerialPort1, (uint8_t*)&ch, 1);
	return ch;
}
