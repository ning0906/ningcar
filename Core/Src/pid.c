#include "pid.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "mortor.h"
//传感器数据变量
int Encoder_Left,Encoder_Right;
float pitch,roll,yaw;//角度值
short  gyrox,gyroy,gyroz;//角速度值
short aacx,aacy,aacz;//角加速度值

//闭环控制中间变量
int Vertical_Out,Velocity_Out,Turn_Out,Target_Speed,Target_turn,moto1,moto2;
float Med_angle=5.5;//平衡值角度值偏移量（机械中值）




float Vertical_Kp,Vertical_Kd;
float Velocity_Kp,Velocity_Ki;
float Turn_Kp,Turn_Kd;
uint8_t stop;
extern TIM_HandleTypeDef htim2,htim4;
//直立环PD控制器
//期望角度、真实角度、角速度
int Vertical(float mad,float angle,float gyro_Y)
{
	int temp;
	temp=Vertical_Kp*(angle-mad)+Vertical_Kd*gyro_Y;
	return temp;


}
//速度环PI控制器
//期望速度、左编码器、右编码器
int Velocity(int target,int encoder_L,int encoder_R)
{
	static int Err_LowOut_last,Encoder_S;
	static float a=0.7;
	int Err,Err_LowOut,temp;
	//1、计算偏差值
	Err=(encoder_L+encoder_R)-target;
	//2、低通滤波
	Err_LowOut=(1-a)*Err+a*Err_LowOut_last;
	Err_LowOut_last=Err_LowOut;
	//3、积分
	Encoder_S+=Err_LowOut;
	//4、积分限幅（-20000~20000）
	Encoder_S=Encoder_S>20000?20000:(Encoder_S<-20000?-20000:Encoder_S);
	if(stop==1)Encoder_S=0,stop=0;
	//5、速度环计算
	temp=Velocity_Kp*Err_LowOut+Velocity_Ki*Encoder_S;
	return temp;
	

}
//转向环PD控制器
//角速度，角度值
int Turn(float gyro_Z,int target_turn)
{
	int temp;
	temp=Turn_Kp*target_turn+Turn_Kd*gyro_Z;
	return temp;



}
void Control(void)//每隔10ms调用一次
{
	int PWM_Out;
	//1、读取陀螺仪和编码器的数据
	Encoder_Left=Read_Speed(&htim2);
	Encoder_Right=-Read_Speed(&htim4);
	mpu_dmp_get_data(&pitch,&roll,&yaw);
	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	//2、将数据传入PID控制器，计算出输出结果，即左右电机转速值
	Velocity_Out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);
	Vertical_Out=Vertical(Velocity_Out+Med_angle,roll,gyrox);
	Turn_Out=Turn(gyroz,Target_turn);
	PWM_Out=Vertical_Out;
	moto1=PWM_Out-Turn_Out;
	moto2=PWM_Out+Turn_Out;
	Limit(&moto1,&moto2);
	load(moto1,moto2);
	
	


}










