#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include "main.h"

void SerialPortOpen(void);
void SyncSerialRxQueueReadyCountOnIDLE(void);
void SerialRxBuffCopyToQueue(void);
void OnUartTransmitCompleteForSerialTx(void);
void OnUartErrorForSerial(void);
uint32_t SerialRead(uint8_t* pData, uint32_t pLen);
uint32_t SerialWrite(uint8_t* pData, uint32_t len);
void Test(void);

#endif
