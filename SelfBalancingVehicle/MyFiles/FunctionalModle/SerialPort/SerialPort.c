#include "SerialPort.h"
#include "usart.h"
#include <stdio.h>

/**** 队列类定义部分 ****/

#define QUEUE_SIZE 1000

typedef struct
{
	uint8_t mItems[QUEUE_SIZE];
	uint32_t mFront;
	uint32_t mRear;
	uint32_t mCount;
} Queue;

void InitQueue(Queue *q)
{
	q->mFront = 0;
	q->mRear = (uint32_t)-1;
	q->mCount = 0;
}

#define IsQueueEmpty(q) ((q)->mCount == 0)
#define IsQueueFull(q) ((q)->mCount >= QUEUE_SIZE)

uint32_t Enqueue(Queue *q, uint8_t *values, uint32_t len)
{
	if(q->mCount >= QUEUE_SIZE || len == 0)
		return 0;
	
	if(len > QUEUE_SIZE-q->mCount)
		len = QUEUE_SIZE-q->mCount;
	
	for(uint32_t i=0; i < len; i++)
	{
		q->mRear = (q->mRear + 1)%QUEUE_SIZE;
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
		q->mFront = (q->mFront + 1) % QUEUE_SIZE;
	}
	q->mCount -= len;
	return len;
}



/**** 串口DMA处理部分 ****/

UART_HandleTypeDef *mHuart;

// 应用层的收发队列
Queue gSerialRxQueue, gSerialTxQueue;
uint32_t mSerialRxQueueReadyCount = 0;

// 物理层的收发缓冲区
#define BUFF_SIZE 10*2		// 增大缓冲区，可以减少IDLE、半满、全满中断频繁程度

uint8_t mSerialRxBuff[BUFF_SIZE];
uint32_t mSerialRxBuffNextPos = 0;	// 记录下一次从哪开始复制

uint8_t mSerialTxBuff[BUFF_SIZE];
uint8_t mIsSerialTxBusy = 0;

void SerialPortOpen()
{
	InitQueue(&gSerialRxQueue);
	InitQueue(&gSerialTxQueue);
	
	mHuart = &huart1;
	mSerialRxBuffNextPos = 0;
	__HAL_UART_ENABLE_IT(mHuart, UART_IT_IDLE);
	HAL_UART_Receive_DMA(mHuart, mSerialRxBuff, BUFF_SIZE);
}

void SyncSerialRxQueueReadyCountOnIDLE()
{
	mSerialRxQueueReadyCount = gSerialRxQueue.mCount;
}

void SerialRxBuffCopyToQueue(void)
{
	// 读取buff长度作为接收末尾+1位置，注意在全满中断时为0
	uint32_t currentPos = BUFF_SIZE - __HAL_DMA_GET_COUNTER(mHuart->hdmarx);
	
	if(currentPos == mSerialRxBuffNextPos)
	{
		// 如果触发了IDLE中断，但是没有收到新的数据，说明串口出错了
		// 当接收到的数据由于波特率或其他配置不正确以及受到干扰时
		// 串口就会进入错误状态，同时触发HAL_UART_ErrorCallback()
		// 比如波特率不匹配的错误状态是HAL_UART_ERROR_FE（帧错误）
		// OnUartErrorForSerial()将被调用，重启接收
		return;
	}
	
	// 如果 currentPos 小于 mSerialRxBuffLastPos，说明发生了环绕
	else if (currentPos < mSerialRxBuffNextPos)
	{
			Enqueue(&gSerialRxQueue, mSerialRxBuff + mSerialRxBuffNextPos, BUFF_SIZE - mSerialRxBuffNextPos);
			//Enqueue(&gSerialRxQueue, mSerialRxBuff, currentPos);
	}
	else
			Enqueue(&gSerialRxQueue, mSerialRxBuff + mSerialRxBuffNextPos, currentPos - mSerialRxBuffNextPos);
		
	mSerialRxBuffNextPos = currentPos;
	
//	mSerialTxBuff[0] = gSerialRxQueue.mCount;
//	HAL_UART_Transmit_DMA(mHuart, mSerialTxBuff, 1);
}

void SerialTxQueueCopyToBuff(void)
{
	uint32_t len = gSerialTxQueue.mCount;
	if(len == 0)
		return;
	if(len > BUFF_SIZE)
		len = BUFF_SIZE;
	
	mIsSerialTxBusy = 1;
	
	Dequeue(&gSerialTxQueue, mSerialTxBuff, len);
	
	HAL_UART_Transmit_DMA(mHuart, mSerialTxBuff, len);
}

void OnUartTransmitCompleteForSerialTx(void)
{
	mIsSerialTxBusy = 0;
	SerialTxQueueCopyToBuff();	// 发送完成后，再检测一下发送队列是否还有内容
}

void OnUartErrorForSerial(void)
{
	// 串口错误状态处理，重新启动接收
	if(mHuart->ErrorCode != HAL_UART_ERROR_NONE)
	{
		mSerialRxBuffNextPos = 0;
		HAL_UART_Receive_DMA(mHuart, mSerialRxBuff, BUFF_SIZE);
	}
}


/**** 用户应用接口部分 ****/

void ByteCopy(uint8_t* src, uint8_t* dst, uint32_t num)
{
	for(uint8_t* end = src + num; src < end; src ++)
		*(dst++) = *src;
}

uint32_t SerialRead(uint8_t* pData, uint32_t len)
{
	if(pData == NULL || len == 0 || mSerialRxQueueReadyCount == 0)
		return 0;
	
	if(mSerialRxQueueReadyCount < len)
		len = mSerialRxQueueReadyCount;
	mSerialRxQueueReadyCount -= len;
	
	len = Dequeue(&gSerialRxQueue, pData, len);
	
	return len;
}

uint32_t SerialWrite(uint8_t* pData, uint32_t len)
{
	if(pData == NULL || len == 0 || IsQueueFull(&gSerialTxQueue))
		return 0;
	
	len = Enqueue(&gSerialTxQueue, pData, len);
	
	//HAL_UART_Transmit(&huart1, gSerialTxQueue.mItems, gSerialTxQueue.mCount, 0xFFFFFF);
	
	if(mIsSerialTxBusy == 0)
		SerialTxQueueCopyToBuff();
	
	return len;
}


void Test()
{
	static uint32_t tLen = 0;
	static uint8_t tData[100];
	
	// 接收测试
	tLen = SerialRead(tData, 0xFFFF);
	if(tLen > 0)
	{
		//mSerialTxBuff[0] = gSerialRxQueue.mFront;
		// HAL_UART_Transmit_DMA(mHuart, tData, tLen);
		SerialWrite(tData, tLen);
	}
}

// 重定向printf输出到串口队列
int fputc(int ch, FILE *f)
{
	SerialWrite((uint8_t*)&ch, 1);
	return ch;
}
