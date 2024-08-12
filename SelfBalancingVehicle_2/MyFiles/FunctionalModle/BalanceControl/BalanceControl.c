#include "BalanceControl.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "Motors.h"
#include <stdio.h>
#include "Protocol.h"


float gPitch = 0, gRoll = 0, gYaw = 0;
int16_t gGyroY = 0, gGyroZ = 0;
uint16_t gCountForReport = 0;

float gBalanceUprightKP = 80.0f;//+180.0f;
float gBalanceUprightKD = 0.4f;//+0.46f;
float gBalanceUprightMC = -5.0f;//-2.3f;		// ֱ�����Ļ�е��ֵ

float gBalanceVelocityKP = 5.0f;
float gBalanceVelocityKI = 0.03f;

float gBalanceSteeringKP = 0.0f;
float gBalanceSteeringKD = 0.0f;


// ֱ��PD���ƣ��ں�ŷ������б�ǣ���е��ֵ����б�ǽ��ٶ�
int BalanceUpright(float angle, float mechanicalBalance, float gyro)
{
	float bias = angle - mechanicalBalance;	// �Ƕ����
	int uprightOutput = gBalanceUprightKP * bias + gBalanceUprightKD * gyro;	// �������pwm
	return uprightOutput;
}

// �ٶȻ�PI���ƣ���������ٶ�ֵ���ұ������ٶ�ֵ
int BalanceVelocity(int encoderLeft, int encoderRight, float pitch)
{
	static float velocityOutput, encoderLeast, encoder;
	static float encoderIntegral;
	
	// ��������ת�ٲ�=���ұ�����֮��-Ŀ���ٶȣ���Ϊ�㣩
	encoderLeast = encoderLeft + encoderRight - 0;
	
	// ����ת�ٲ��һ�׵�ͨ�˲�
	encoder *= 0.7f;
	encoder += encoderLeast * 0.3f;
	// ͨ��ת�ٲ���ֳ���������λ��
	encoderIntegral += encoder;
	
	// �����޷����Ƚ���ֵΪPWM��ֵ
	if(encoderIntegral > 1200)
		encoderIntegral = 1200;
	else if(encoderIntegral < -1200)
		encoderIntegral = -1200;
	
	// PI����
	velocityOutput = encoder * gBalanceVelocityKP + encoderIntegral * gBalanceVelocityKI;
	
	// �ٶȿ��ƣ�����رպ��������
	if(pitch<-50 || pitch>50)
		encoderIntegral = 0;
	return velocityOutput;
}

// ת�򻷿���
int BalanceSteering(float diffDegree, float gyroZ)
{
	int steeringOutput = 0;
	
	if(diffDegree == 0)	// û��ת������ʱ��ʹ�ú�����ٶȰ�����ֱ��
		steeringOutput = gBalanceSteeringKD * gyroZ;
	else	// ��ת������ʱ�������ٳ̶�ͨ��������ϵ�����PWM����
		steeringOutput = gBalanceSteeringKP * diffDegree;
	
	return steeringOutput;
}

// �˺���������Mainѭ���У�������ѯ��ȡ����
void OnMainForBalanceControl(void)
{
//	static uint8_t mpuErrorCount = 0;
	
	// MPU��FIFO�����ȡ���˾Ͷ�������գ�
	// �������̫���ֲ�������ֵ��
	// ���Է���main����ͨ����ѯ��ʽ��ȡ���������
	// ����FIFO��յĴ�������50ms�������ʱ����Ҳ��Ϊʲô�Ҳ�������жϴ������Ҫԭ��
	
//	if(mpu_dmp_get_data(&gPitch, &gRoll, &gYaw) == 0)
//	{

//		mpuErrorCount ++;
//		if(mpuErrorCount >= 5)	// ����5�ζ�ȡʧ�ܣ����³�ʼ��MPU
//		{
//			mpuErrorCount = 0;
//			while( mpu_dmp_init() ){};
//		}
//	}
//	mpuErrorCount = 0;
	
//	while(mpu_dmp_get_data(&gPitch, &gRoll, &gYaw) != 0)
//	{
//		gGyroY = MPU_Get_Gyroscope_Y();
//		gGyroZ = MPU_Get_Gyroscope_Z();
//	}
//	
//	gGyroY = MPU_Get_Gyroscope_Y();
//	gGyroZ = MPU_Get_Gyroscope_Z();
	
//	gGyroY = MPU_Get_Gyroscope_Y();
	
	if(gCountForReport >= 100)
	{
		gCountForReport = 0;
		
		float fv =  gGyroY;
		fv /= 100.0f;
		uint32_t u32V = *(uint32_t*)&fv;
		uint8_t data[6];
		data[0] = 0x0F;
		data[1] = 0x01;
		data[2] = u32V >> 24;
		data[3] = u32V >> 16;
		data[4] = u32V >> 8;
		data[5] = u32V;
		ProtocolEncode(data, sizeof(data));
		
		u32V = *(uint32_t*)&gPitch;
		data[0] = 0x0F;
		data[1] = 0x02;
		data[2] = u32V >> 24;
		data[3] = u32V >> 16;
		data[4] = u32V >> 8;
		data[5] = u32V;
		ProtocolEncode(data, sizeof(data));	
		
	}
}

void OnTIMForBalanceControl(void)
{
	static uint16_t tCount = 0;	// ����ʱ���ۼ�
	
	gCountForReport ++;
	
	++tCount;
	if(tCount >= 1)	// ��϶�ʱ�����ڣ�ȷ��ÿ��1msִ��һ����������
	{
		tCount = 0;
		
		mpu_dmp_get_data(&gPitch, &gRoll, &gYaw);
		gGyroY = MPU_Get_Gyroscope_Y();
		//gGyroZ = MPU_Get_Gyroscope_Z();
		
		int16_t balanceOutput = BalanceUpright(gPitch, gBalanceUprightMC , gGyroY)
			+ BalanceVelocity(GetLeftMotorSpeed()*10,GetRightMotorSpeed()*10, gPitch);
		
		//int16_t steeringOutput = 0;//BalanceSteering(0.0f, gGyroZ);
		// ת����ò��ٿ��ƣ�����һ�߼�һ�߼�
		int16_t leftMotorSpeed = balanceOutput;// + steeringOutput;
		int16_t rightMotorSpeed = balanceOutput;// - steeringOutput;
		
		// ����ǹ���ʱ���رյ��
		if(gPitch >= 50 || gPitch <= -50)
		{
			leftMotorSpeed = 0;
			rightMotorSpeed = 0;
		}

		SetLeftMotor(leftMotorSpeed);
		SetRightMotor(rightMotorSpeed);
	}
}

