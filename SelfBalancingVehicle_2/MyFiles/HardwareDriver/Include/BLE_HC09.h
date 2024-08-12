#ifndef __BLE_HC09_H 
#define __BLE_HC09_H

#include "main.h"
#include "SerialPort.h"

#define BLE_USART_WRITE(pData, len) SerialWrite(pData, len)
#define BLE_USART_Read(pData, len) SerialRead(pData, len)

#endif
