#include "Protocol.h"
#include "SerialPort.h"

#include "Motors.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "BalanceControl.h"

uint16_t ReverseEndianForU16(uint16_t src)
{
	return ((src & 0xFF00) >> 8) | ((src & 0xFF) << 8);
}
int16_t ReverseEndianForS16(int16_t src)
{
	return ((src & 0xFF00) >> 8) | ((src & 0xFF) << 8);
}

uint8_t CheckSum(uint8_t *pData, uint32_t len)
{
	uint8_t sum = 0;
	while(len--)
		sum += pData[len];
	return sum;
}

void ProtocolEncode(uint8_t *pData, uint32_t len)
{
	uint8_t bytes[50] = {0x7D};
	uint8_t *pBytes = bytes + 1;
	
	for(uint32_t i=0; i<len; i++)
	{
		if(pData[i] == 0x7D || pData[i] == 0x7E)
		{
			*(pBytes++) = 0x7E;
			*(pBytes++) = pData[i] - 0x20;
		}
		else
			*(pBytes++) = pData[i];
	}
	
	uint32_t bytesLen = pBytes - bytes;
	bytes[bytesLen] = CheckSum(bytes+1, bytesLen);
	
	SerialWrite(&gSerialPort1, bytes, bytesLen+1);
}

void ProtocolDecode(uint8_t *pData, uint32_t len)
{
	uint8_t bytes[50] = {0};
	// uint32_t bytesLen = 0;
	
	uint32_t srcLen = SerialRead(&gSerialPort1, bytes, 50);
	if(srcLen == 0)
		return;
	
	// Ѱ��֡ͷ0x7D
	for(uint32_t i=0; i<srcLen; i++)
	{
		if(bytes[i] != 0x7D)
			continue;
		
		i++;
		
		// Ѱ��֡βλ��
		uint32_t j=i;
		for(; j<srcLen; j++)
			if(bytes[j] == 0x7D)
				break;
		j--;
		
		// У��
		if(CheckSum(bytes+i, j-i) != bytes[j])
		{
			i = j;
			continue;
		}
		
		// ȥ��ת���0x7E
		uint8_t *pBytes = bytes;
		for(; i<=j; i++)
		{
			if(bytes[i] == 0x7E)
				*(pBytes++) = bytes[++i] + 0x20;
			else
				*(pBytes++) = bytes[i];
		}
		
		i = j;
		
		// ��������������
		SerialWrite(&gSerialPort1, bytes, pBytes-bytes);
		
		switch(bytes[0])
		{
			case 0x00:	// ��Ƭ����λ
				// __disable_irq();
				HAL_NVIC_SystemReset();
				break;
			
			case 0x01:	// ָ����������
				break;
			
			case 0x02:	// ���ɲ��
				if(bytes[1] == 0x00)
					BrakingLeftMotor();
				else if(bytes[1] == 0x01)
					BrakingRightMotor();
				break;
				
			case 0x03:	// ��������ٶ�
			{
				int16_t speed = (int16_t)(bytes[2]<<8 | bytes[3]);
				if(bytes[1] == 0x00)
					SetLeftMotor(speed);
				else if(bytes[1] == 0x01)
					SetRightMotor(speed);
			}break;
			
			case 0x04:	// �����ȡ���ٱ�����
			{
				int16_t s;	// STM32��С�˴��棨��λ�ڵ͵�ַ��
				if(bytes[1] == 0x00)
					s = GetLeftMotorSpeed();
				else if(bytes[1] == 0x01)
					s = GetRightMotorSpeed();
				s = ReverseEndianForU16(s);	// С��ת��ˣ�Э��涨�����ô��ģʽ��
				SerialWrite(&gSerialPort1, (uint8_t*)&s, sizeof(s));
			}break;
			
			case 0x05:	// ��λMPU6050
			{
				uint8_t res = mpu_dmp_init();
				SerialWrite(&gSerialPort1, &res, sizeof(res));
			}break;
			
			case 0x06:	// ��ȡ���ٶ�
			{
				if(bytes[1] == 0x00)
				{
					int16_t accs[3];
					MPU_Get_Accelerometer(&accs[0], &accs[1], &accs[2]);
					accs[0] = ReverseEndianForU16(accs[0]);
					accs[1] = ReverseEndianForU16(accs[1]);
					accs[2] = ReverseEndianForU16(accs[2]);
					SerialWrite(&gSerialPort1, (uint8_t*)accs, sizeof(accs));
				}
				else if(bytes[1] == 0x01)
				{
					int16_t accX = MPU_Get_Accelerometer_X();
					accX = ReverseEndianForU16(accX);
					SerialWrite(&gSerialPort1, (uint8_t*)&accX, sizeof(accX));
				}
				else if(bytes[1] == 0x02)
				{
					int16_t accY = MPU_Get_Accelerometer_Y();
					accY = ReverseEndianForU16(accY);
					SerialWrite(&gSerialPort1, (uint8_t*)&accY, sizeof(accY));
				}
				else if(bytes[1] == 0x03)
				{
					int16_t accZ = MPU_Get_Accelerometer_Z();
					accZ = ReverseEndianForU16(accZ);
					SerialWrite(&gSerialPort1, (uint8_t*)&accZ, sizeof(accZ));
				}
			}break;
			
			case 0x07:	// ��ȡ���ٶ�
			{
				if(bytes[1] == 0x00)
				{
					int16_t grys[3];
					MPU_Get_Gyroscope(&grys[0], &grys[1], &grys[2]);
					grys[0] = ReverseEndianForU16(grys[0]);
					grys[1] = ReverseEndianForU16(grys[1]);
					grys[2] = ReverseEndianForU16(grys[2]);
					
					SerialWrite(&gSerialPort1, (uint8_t*)grys, sizeof(grys));
				}
				else if(bytes[1] == 0x01)
				{
					int16_t gryX = MPU_Get_Gyroscope_X();
					gryX = ReverseEndianForU16(gryX);
					SerialWrite(&gSerialPort1, (uint8_t*)&gryX, sizeof(gryX));
				}
				else if(bytes[1] == 0x02)
				{
					int16_t gryY = MPU_Get_Gyroscope_Y();
					gryY = ReverseEndianForU16(gryY);
					SerialWrite(&gSerialPort1, (uint8_t*)&gryY, sizeof(gryY));
				}
				else if(bytes[1] == 0x03)
				{
					int16_t gryZ = MPU_Get_Gyroscope_Z();
					gryZ = ReverseEndianForU16(gryZ);
					SerialWrite(&gSerialPort1, (uint8_t*)&gryZ, sizeof(gryZ));
				}
			}break;
			
			case 0x08:	// ��ȡ�ں�ŷ����
			{
				// float pitch = 0, roll = 0, yaw = 0;
				// mpu_dmp_get_data(&pitch, &roll, &yaw);
				uint8_t data[2];
				int16_t pitch = gPitch*10.0f, roll = gRoll*10.0f, yaw = gYaw*10.0f;
				
				if(bytes[1] == 0x00 || bytes[1] == 0x01)
				{
					data[0] = pitch >> 8;
					data[1] = pitch & 0xFF;
					SerialWrite(&gSerialPort1, data, sizeof(data));
				}
				if(bytes[1] == 0x00 || bytes[1] == 0x02)
				{
					data[0] = roll >> 8;
					data[1] = roll & 0xFF;
					SerialWrite(&gSerialPort1, data, sizeof(data));
				}
				if(bytes[1] == 0x00 || bytes[1] == 0x03)
				{
					data[0] = yaw >> 8;
					data[1] = yaw & 0xFF;
					SerialWrite(&gSerialPort1, data, sizeof(data));
				}
			}break;
			
			case 0x09:	// ����ֱ����PD��KP��KD�ͻ�е��ֵ
			{
				if(bytes[1] == 0x00)
				{
					gBalanceUprightKP = (int16_t)(bytes[2]<<8 | bytes[3]) / 10.0f;
					gBalanceUprightMC = (int16_t)(bytes[4]<<8 | bytes[5]) / 100.0f;
					gBalanceUprightKD = (int16_t)(bytes[6]<<8 | bytes[7]) / 100.0f;
				}
				else if(bytes[1] == 0x01)
					gBalanceUprightKP = (int16_t)(bytes[2]<<8 | bytes[3]) / 10.0f;
				else if(bytes[1] == 0x02)
					gBalanceUprightMC = (int16_t)(bytes[2]<<8 | bytes[3]) / 100.0f;
				else if(bytes[1] == 0x03)
					gBalanceUprightKD = (int16_t)(bytes[2]<<8 | bytes[3]) / 100.0f;
			}break;
			
			case 0x0A:	// ����ת�ٻ�PD��KP��KI��ת�򻷵�KD
			{
				if(bytes[1] == 0x00)
				{
					gBalanceVelocityKP = (int16_t)(bytes[2]<<8 | bytes[3]) / 100.0f;
					gBalanceVelocityKI = (int16_t)(bytes[4]<<8 | bytes[5]) / 100.0f;
					gBalanceSteeringKD = (int16_t)(bytes[6]<<8 | bytes[7]) / 100.0f;
				}
				else if(bytes[1] == 0x01)
					gBalanceVelocityKP = (int16_t)(bytes[2]<<8 | bytes[3]) / 100.0f;
				else if(bytes[1] == 0x02)
					gBalanceVelocityKI = (int16_t)(bytes[2]<<8 | bytes[3]) / 100.0f;
				else if(bytes[1] == 0x03)
					gBalanceSteeringKD = (int16_t)(bytes[2]<<8 | bytes[3]) / 100.0f;
			}break;
			
			default:
				break;
		}
	}
	
}

