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
float gBalanceUprightMC = -5.0f;//-2.3f;		// 直立环的机械中值

float gBalanceVelocityKP = 5.0f;
float gBalanceVelocityKI = 0.03f;

float gBalanceSteeringKP = 0.0f;
float gBalanceSteeringKD = 0.0f;


// 直立PD控制：融合欧拉角倾斜角，机械中值，倾斜角角速度
int BalanceUpright(float angle, float mechanicalBalance, float gyro)
{
	float bias = angle - mechanicalBalance;	// 角度误差
	int uprightOutput = gBalanceUprightKP * bias + gBalanceUprightKD * gyro;	// 计算出的pwm
	return uprightOutput;
}

// 速度环PI控制：左编码器速度值，右编码器速度值
int BalanceVelocity(int encoderLeft, int encoderRight, float pitch)
{
	static float velocityOutput, encoderLeast, encoder;
	static float encoderIntegral;
	
	// 最新左右转速差=左右编码器之和-目标速度（设为零）
	encoderLeast = encoderLeft + encoderRight - 0;
	
	// 左右转速差的一阶低通滤波
	encoder *= 0.7f;
	encoder += encoderLeast * 0.3f;
	// 通过转速差积分出两轮相差的位移
	encoderIntegral += encoder;
	
	// 积分限幅，比较最值为PWM最值
	if(encoderIntegral > 1200)
		encoderIntegral = 1200;
	else if(encoderIntegral < -1200)
		encoderIntegral = -1200;
	
	// PI计算
	velocityOutput = encoder * gBalanceVelocityKP + encoderIntegral * gBalanceVelocityKI;
	
	// 速度控制，电机关闭后清除积分
	if(pitch<-50 || pitch>50)
		encoderIntegral = 0;
	return velocityOutput;
}

// 转向环控制
int BalanceSteering(float diffDegree, float gyroZ)
{
	int steeringOutput = 0;
	
	if(diffDegree == 0)	// 没有转向需求时，使用航向角速度帮助走直线
		steeringOutput = gBalanceSteeringKD * gyroZ;
	else	// 有转向需求时，将差速程度通过比例关系输出到PWM叠加
		steeringOutput = gBalanceSteeringKP * diffDegree;
	
	return steeringOutput;
}

// 此函数放置于Main循环中，用于轮询获取数据
void OnMainForBalanceControl(void)
{
//	static uint8_t mpuErrorCount = 0;
	
	// MPU的FIFO如果读取慢了就堵塞，清空，
	// 如果读得太快又产生不出值，
	// 所以放在main里面通过轮询方式读取结果并缓存
	// 还有FIFO清空的代码是有50ms的软件延时，这也是为什么我不想放在中断处理的主要原因
	
//	if(mpu_dmp_get_data(&gPitch, &gRoll, &gYaw) == 0)
//	{

//		mpuErrorCount ++;
//		if(mpuErrorCount >= 5)	// 连续5次读取失败，重新初始化MPU
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
	static uint16_t tCount = 0;	// 辅助时间累计
	
	gCountForReport ++;
	
	++tCount;
	if(tCount >= 1)	// 结合定时器周期，确保每隔1ms执行一次以下任务
	{
		tCount = 0;
		
		mpu_dmp_get_data(&gPitch, &gRoll, &gYaw);
		gGyroY = MPU_Get_Gyroscope_Y();
		//gGyroZ = MPU_Get_Gyroscope_Z();
		
		int16_t balanceOutput = BalanceUpright(gPitch, gBalanceUprightMC , gGyroY)
			+ BalanceVelocity(GetLeftMotorSpeed()*10,GetRightMotorSpeed()*10, gPitch);
		
		//int16_t steeringOutput = 0;//BalanceSteering(0.0f, gGyroZ);
		// 转弯采用差速控制，所以一边加一边减
		int16_t leftMotorSpeed = balanceOutput;// + steeringOutput;
		int16_t rightMotorSpeed = balanceOutput;// - steeringOutput;
		
		// 当倾角过大时，关闭电机
		if(gPitch >= 50 || gPitch <= -50)
		{
			leftMotorSpeed = 0;
			rightMotorSpeed = 0;
		}

		SetLeftMotor(leftMotorSpeed);
		SetRightMotor(rightMotorSpeed);
	}
}

