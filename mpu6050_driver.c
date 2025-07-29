#include "mpu6050_driver.h"

// ��ȡƮ����
void MPU_Get_Gyro_Offset(short* gx_offset, short* gy_offset, short* gz_offset)
{
    int i;
    short gx, gy, gz;
    long temp_gx = 0, temp_gy = 0, temp_gz = 0;

    // ȷ���豸���ھ�ֹ״̬
    for(i = 0; i < 400; i++)
    {
        MPU_Get_Gyroscope(&gx, &gy, &gz);
        temp_gx += gx;
        temp_gy += gy;
        temp_gz += gz;
        HAL_Delay(5); // ��ʱ5����
    }
    *gx_offset = temp_gx / 400;
    *gy_offset = temp_gy / 400;
    *gz_offset = temp_gz / 400;
}

// ʹ�þ�̬������������һ�ε�״̬
static float g_last_yaw = 0.0f;
static int g_revolution_count = 0;
static bool g_is_yaw_initialized = false;

/**
 * @brief ��һ����[-180, 180]��Χ�ڵ�yaw�Ƕ�ת��Ϊ�����ĽǶ�ֵ��
 * 
 * @param current_yaw �Ӵ�������ȡ�ĵ�ǰyawֵ (-180 to 180)��
 * @return float ������yaw�Ƕ�ֵ (���� 370, -450 ��)��
 */
float convert_to_continuous_yaw(float current_yaw) 
{
    // ����һ����ֵ����⡰���䡱�����ֵӦ�ô���180��ͨ��ȡ270��300�Ƚϰ�ȫ��
    const float WRAP_AROUND_THRESHOLD = 300.0f;

    // �״ε���ʱ���г�ʼ��
    if (!g_is_yaw_initialized) {
        g_last_yaw = current_yaw;
        g_is_yaw_initialized = true;
        g_revolution_count = 0;
    }

    // �������ϴζ����Ĳ���
    float diff = current_yaw - g_last_yaw;

    // ����Ƿ����ˡ����䡱
    if (diff > WRAP_AROUND_THRESHOLD) {
        // �����Ƕ��������Ƕ� (����, �� 170�� �� -175��), ʵ��������ת, Ȧ��Ӧ������
        // ��ʱ diff �ӽ� -360 (���� -175 - 170 = -345)
        // ����߼�������Ǵ�-180���䵽+180�������˵��������ת����
        g_revolution_count--;
    } else if (diff < -WRAP_AROUND_THRESHOLD) {
        // �Ӹ��Ƕ��������Ƕ� (����, �� -170�� �� 175��), ʵ��������ת, Ȧ��Ӧ�ü�С
        // ��ʱ diff �ӽ� 360 (���� 175 - (-170) = 345)
        // ����߼�������Ǵ�+180���䵽-180�������˵��������ת����
        g_revolution_count++;
    }

    // �����ϴε�yawֵ�Ա��´ε���
    g_last_yaw = current_yaw;

    // ����������yawֵ
    float continuous_yaw = current_yaw + (float)g_revolution_count * 360.0f;

    return continuous_yaw;
}

