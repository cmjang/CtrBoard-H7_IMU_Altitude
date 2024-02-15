
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
    for (int i = 0; i < 1000; ++i) { //0Ư��ʼ��
        imu963ra_get_acc();
        imu963ra_get_gyro();

        //0Ư��ʼ����ֵ�ж�
        if(fabsf(gyroscope[0])+ fabsf(gyroscope[1])+ fabsf(gyroscope[2])<gyroscope_threshold) {
            gyroscopeOffset[0] += imu963ra_gyro_transition(imu963ra_gyro_x);
            gyroscopeOffset[1] += imu963ra_gyro_transition(imu963ra_gyro_y);
            gyroscopeOffset[2] += imu963ra_gyro_transition(imu963ra_gyro_z);
        }
        else
        {
            i--;   //������ֵ����ȡһ��
        }
        system_delay_ms(2);  //�����Լ��Ĳ�������ȡֵ demo��500Hz��2ms�ж�
    }
    gyroscopeOffset[0]/=1000;
    gyroscopeOffset[1]/=1000;
    gyroscopeOffset[2]/=1000;

    //=============================================================================================

    Mahony_Init(500);  //���ò�����  �����2ms����һ�� ������Ϊ500Hz
    imu963ra_get_acc();  //���ü��ٶȼ� ����һ����ʼ�Ĵ���λ�ü��㡣ȥ�����Ҳ���Խ�����̬���㵫��λ�������ٶȻ����
    //Ĭ��Ҳû�Ӵ����� �������Ҫ���Լ�һ�£������ƱȽ��鷳Ҫ����У׼
    MahonyAHRSinit(imu963ra_acc_transition(imu963ra_acc_x),imu963ra_acc_transition(imu963ra_acc_y),imu963ra_acc_transition(imu963ra_acc_z),0,0,0);  //�ϵ���ٿ�ʼ��ʼ��
    return true;
}

void Attitude_Calculate()
{
    // replace this with actual gyroscope data in degrees/s
    imu963ra_get_acc();
    imu963ra_get_gyro();
    //imu963ra_get_mag();
    //Ҫ���нǶ��л� ����Ƕȵ�λ�뿴�����ע��
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
    //��ȥ��Ʈ ���ǰ����Ưȥ�� ����ǵ�ע��
    gyroscope[0]-= gyroscopeOffset[0];
    gyroscope[1]-= gyroscopeOffset[1];
    gyroscope[2]-= gyroscopeOffset[2];
//===================================================================================================
    //��̬���� ����λΪ�����ƣ�һ�㲻�þ��� ��0
    Mahony_update(gyroscope[0],gyroscope[1],gyroscope[2],accelerometer[0],accelerometer[1],accelerometer[2],0,0,0);
    Mahony_computeAngles(); //�Ƕȼ���
    eulerAngle[0]=getPitch(); //��ȡPitch
    eulerAngle[1]=getRoll(); //��ȡRoll
    eulerAngle[2]=getYaw();  //��ȡYaw
}
