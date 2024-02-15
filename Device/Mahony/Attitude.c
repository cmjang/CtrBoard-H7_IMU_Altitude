
#include "Attitude.h"
//==================================================================================================
#define gyroscope_threshold 5
//--------------------------------------------------------------------------------------
    static float gyroscopeOffset[3] = {0.0f, 0.0f, 0.0f};    //gyro
    float gyroscope[3] ={0.0f,0.0f,0.0f};                   //gyro
    float accelerometer[3] = {0.0f, 0.0f, 0.0f};    //acc
    float magnetometer[3] = {0.0f, 0.0f, 0.0f};     //mag
    float eulerAngle[3] = {0.0f, 0.0f, 0.0f};       //euler angle
//--------------------------------------------------------------------------------------


//===================================================================================================

bool Attitude_Init()
{
    //===========================================================================================
    for (int i = 0; i < 1000; ++i) { //0漂初始化
        imu963ra_get_acc();
        imu963ra_get_gyro();

        //0漂初始化阈值判断
        if(fabsf(gyroscope[0])+ fabsf(gyroscope[1])+ fabsf(gyroscope[2])<gyroscope_threshold) {
            gyroscopeOffset[0] += imu963ra_gyro_transition(imu963ra_gyro_x);
            gyroscopeOffset[1] += imu963ra_gyro_transition(imu963ra_gyro_y);
            gyroscopeOffset[2] += imu963ra_gyro_transition(imu963ra_gyro_z);
        }
        else
        {
            i--;   //超过阈值了再取一次
        }
        system_delay_ms(2);  //按照自己的采样率来取值 demo是500Hz即2ms中断
    }
    gyroscopeOffset[0]/=1000;
    gyroscopeOffset[1]/=1000;
    gyroscopeOffset[2]/=1000;

    //=============================================================================================

    Mahony_Init(500);  //设置采样率  这个是2ms解算一次 采样率为500Hz
    imu963ra_get_acc();  //利用加速度计 进行一个初始的大致位置计算。去掉这句也可以进行姿态解算但是位置收敛速度会很慢
    //默认也没加磁力计 如果有需要可以加一下，磁力计比较麻烦要进行校准
    MahonyAHRSinit(imu963ra_acc_transition(imu963ra_acc_x),imu963ra_acc_transition(imu963ra_acc_y),imu963ra_acc_transition(imu963ra_acc_z),0,0,0);  //上电快速开始初始化
    return true;
}

void Attitude_Calculate()
{
    // replace this with actual gyroscope data in degrees/s
    imu963ra_get_acc();
    imu963ra_get_gyro();
    //imu963ra_get_mag();
    //要进行角度切换 具体角度单位请看下面的注释
    gyroscope[0]= imu963ra_gyro_transition(imu963ra_gyro_x); //replace this with actual accelerometer data in degrees/s
    gyroscope[1]= imu963ra_gyro_transition(imu963ra_gyro_y);
    gyroscope[2]= imu963ra_gyro_transition(imu963ra_gyro_z);
    accelerometer[0]=imu963ra_acc_transition(imu963ra_acc_x);
    accelerometer[1]=imu963ra_acc_transition(imu963ra_acc_y);
    accelerometer[2]=imu963ra_acc_transition(imu963ra_acc_z); // replace this with actual accelerometer data in g
    magnetometer[0]=imu963ra_mag_transition(imu963ra_mag_x);
    magnetometer[1]=imu963ra_mag_transition(imu963ra_mag_y);
    magnetometer[2]=imu963ra_mag_transition(imu963ra_mag_z);// replace this with actual magnetometer data in arbitrary units;
    //=================================================================
    //减去零飘 如果前面零漂去了 这里记得注释
    gyroscope[0]-= gyroscopeOffset[0];
    gyroscope[1]-= gyroscopeOffset[1];
    gyroscope[2]-= gyroscopeOffset[2];
//===================================================================================================
    //姿态解算 后三位为磁力计，一般不用就行 填0
    Mahony_update(gyroscope[0],gyroscope[1],gyroscope[2],accelerometer[0],accelerometer[1],accelerometer[2],0,0,0);
    Mahony_computeAngles(); //角度计算
    eulerAngle[0]=getPitch(); //获取Pitch
    eulerAngle[1]=getRoll(); //获取Roll
    eulerAngle[2]=getYaw();  //获取Yaw
}
