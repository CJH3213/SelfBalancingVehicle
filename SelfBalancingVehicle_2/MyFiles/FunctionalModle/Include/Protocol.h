#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "main.h"

void ProtocolEncode(uint8_t *pData, uint32_t len);
void ProtocolDecode(uint8_t *pData, uint32_t len);

#endif

