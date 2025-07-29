#include "motor_driver.h"

#define Motor_ABS(x) ((x) >= 0 ? (x) : -(x))

void Motor_Config_Init(MOTOR* motor, TIM_HandleTypeDef *htim, uint32_t pwm_channel, 
                       GPIO_TypeDef *in1_port, uint16_t in1_pin, GPIO_TypeDef *in2_port, uint16_t in2_pin, 
                       unsigned char reverse, int dead_band_speed)
{
    motor->config.htim = htim;
    motor->config.pwm_channel = pwm_channel;
    motor->config.in1.port = in1_port;
    motor->config.in1.pin = in1_pin;
    motor->config.in2.port = in2_port;
    motor->config.in2.pin = in2_pin;
    motor->config.reverse = reverse;
    
    motor->dead_band_speed = dead_band_speed;
    motor->speed = 0;
  
    HAL_GPIO_WritePin(motor->config.in1.port, motor->config.in1.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(motor->config.in2.port, motor->config.in2.pin, GPIO_PIN_RESET);

    HAL_TIM_PWM_Start(motor->config.htim, motor->config.pwm_channel);
    __HAL_TIM_SET_COMPARE(motor->config.htim, motor->config.pwm_channel, motor->speed);
}

// ����ٶ��޷�
int Motor_Limit_Speed(MOTOR* motor, int speed, int max_speed, int min_speed)
{
    if(speed > max_speed)
        speed = max_speed;
    else if(speed < min_speed)
        speed = min_speed;

    return speed;
}

// �����������
int Motor_Dead_Compensation(MOTOR* motor)
{
  if(motor->speed > 0 && motor->speed < motor->dead_band_speed)
    return motor->dead_band_speed;
  else if(motor->speed < 0 && motor->speed > -motor->dead_band_speed) 
    return -motor->dead_band_speed;
  else
    return motor->speed;
}


// �ٶȿ���
void Motor_Set_Speed(MOTOR* motor, int speed)
{
    // ����ٶ��޷�
    motor->speed = Motor_Limit_Speed(motor, speed, motor->config.htim->Init.Period, -(motor->config.htim->Init.Period));
  
    // �����������
    motor->speed = Motor_Dead_Compensation(motor);

    // �����ٶ�����ѡ����
    if(motor->speed >= 0) // ��ת
    {
        HAL_GPIO_WritePin(motor->config.in1.port, motor->config.in1.pin, motor->config.reverse == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->config.in2.port, motor->config.in2.pin, motor->config.reverse == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
    else // ��ת
    {
        HAL_GPIO_WritePin(motor->config.in1.port, motor->config.in1.pin, motor->config.reverse == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->config.in2.port, motor->config.in2.pin, motor->config.reverse == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }


    // �����ٶ�����ռ�ձ�
    __HAL_TIM_SET_COMPARE(motor->config.htim, motor->config.pwm_channel, Motor_ABS(motor->speed));
}

// ���ֹͣ  
void Motor_Stop(MOTOR* motor)
{
    HAL_GPIO_WritePin(motor->config.in1.port, motor->config.in1.pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->config.in2.port, motor->config.in2.pin, GPIO_PIN_RESET);

    motor->speed = 0;
    __HAL_TIM_SET_COMPARE(motor->config.htim, motor->config.pwm_channel, Motor_ABS(motor->speed));
}    

// ���ɲ��
void Motor_Brake(MOTOR* motor)
{
    HAL_GPIO_WritePin(motor->config.in1.port, motor->config.in1.pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(motor->config.in2.port, motor->config.in2.pin, GPIO_PIN_SET);

    motor->speed = 0;
    __HAL_TIM_SET_COMPARE(motor->config.htim, motor->config.pwm_channel, Motor_ABS(motor->speed));
}
