#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H

#include "MyDefine.h"

// TB6612 ������ýṹ��
typedef struct MOTOR_Config{
    TIM_HandleTypeDef *htim; // ��ʱ��
    uint32_t pwm_channel; // PWM ͨ��
    struct {
        GPIO_TypeDef *port;
        uint16_t pin;
    } in1, in2; // ��Ӧ TB6612 �е� AIN �� BIN
    unsigned char reverse; // ����ķ����Ƿ�ת��0-������1-��ת
} MOTOR_Config;


// ����ṹ��
typedef struct MOTOR
{
    MOTOR_Config config;
    int speed;
    int dead_band_speed; // �������� -dead_band_speed < speed < dead_band_speed ʱ�������ת����
} MOTOR;



// �����ʼ������(����صĹ�������)
void Motor_Config_Init(MOTOR* motor, TIM_HandleTypeDef *htim, uint32_t pwm_channel, 
                       GPIO_TypeDef *in1_port, uint16_t in1_pin, GPIO_TypeDef *in2_port, uint16_t in2_pin, 
                       unsigned char reverse, int dead_band_speed);

// �ٶȿ���
void Motor_Set_Speed(MOTOR* motor, int speed);

// ���ֹͣ  
void Motor_Stop(MOTOR* motor);          

// ���ɲ��
void Motor_Brake(MOTOR* motor);         

#endif
