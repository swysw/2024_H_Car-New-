#include "mpu6050_app.h"

float Pitch, Roll, Yaw; // pitch-�����ǣ���-��+����roll����ǣ�ǰ-��+����yaw-����ǣ���+��-����Ҫ���������ǵ�xyz�ῴ

void Mpu6050_Init(void)
{
  MPU_Init();
	mpu_dmp_init();
}

void Mpu6050_Task(void)
{
  mpu_dmp_get_data(&Pitch, &Roll, &Yaw);
  Yaw = convert_to_continuous_yaw(Yaw);
}
