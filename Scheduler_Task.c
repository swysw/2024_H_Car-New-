/*
 * 2024 ����� H �� - ѭ��С��
 * ��������� STM32F407 ���İ�ʵ��
 * Ӳ��������
 *  ������������� * 2
 *  TB6612
 *  12V ��ѹģ��
 *  ��Ϊ 8 ·�Ҷȴ�����������
 *  MPU6050
 *  0.96 �� Oled
 */

#include "Scheduler_Task.h"

void System_Init(void)
{
  Led_Init();
  Key_Init();
  Oled_Init();
  Uart_Init();
  Gray_Init();
  Motor_Init();
  Encoder_Init();
  Mpu6050_Init();
  PID_Init();
  Uart_Printf(&huart1, "=== System Init ===\r\n");
  HAL_TIM_Base_Start_IT(&htim2);
}

extern Encoder left_encoder;
extern Encoder right_encoder;

extern MOTOR left_motor;
extern MOTOR right_motor;

unsigned char measure_timer5ms;
unsigned char key_timer10ms;

unsigned char output_ff_flag;
unsigned int intput_timer500ms;

unsigned char intput_ff_flag;
unsigned int output_timer500ms;

unsigned int led_timer500ms; // ÿ����һ���㣬LED ���� 500ms ��Ϩ��

unsigned char point_count = 0; // �����ĵ�λ��������Ȧ + 1����Ȧ + 1��

unsigned char system_mode = 4; // ϵͳ״̬��1 ~ 4 ��Ӧ 4 ����Ŀ��

unsigned char circle_count = 0; // �������Ȧ��������

unsigned int distance = 0; // ��¼С��ÿһ����ʻ�ľ���

// TIM2 �жϷ�������1ms �жϣ�
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance != htim2.Instance) return;
 
  /* 10ms �����������*/
  if(++key_timer10ms >= 10)
  {
    key_timer10ms = 0;
    Key_Task();
  }
  
  /* 5ms ��������*/
  if(++measure_timer5ms >= 5) 
  {
    measure_timer5ms = 0;
    Encoder_Task();
    distance += left_encoder.speed_cm_s;
    Mpu6050_Task();
    Gray_Task();
    PID_Task();
  }
  
  /* ��Ȧ������ */
  if(Digtal != 0x00)
  {
    output_ff_flag = 1;
    if(++intput_timer500ms >= 800) intput_timer500ms = 800;
  }
  else if(output_ff_flag == 1 && intput_timer500ms == 800)
  {
    output_ff_flag = 0;
    intput_timer500ms = 0;
    point_count++;
    Car_State_Update();
  }
  
  /* ��Ȧ������ */
  if(Digtal == 0x00)
  {
    intput_ff_flag = 1;
    if(++output_timer500ms >= 800) output_timer500ms = 800;
  }
  else if(intput_ff_flag == 1 && output_timer500ms == 800)
  {
    intput_ff_flag = 0;
    output_timer500ms = 0;
    point_count++;
    Car_State_Update();
  }
  
  /* LED ״̬��� */
  if(led_state == 1 && ++led_timer500ms >= 500)
  {
    led_state = 0;
    led_timer500ms = 0;
  }
}

// ÿ�ε�λ��������ʱ������ϵͳ״̬ͬ������С����״̬
void Car_State_Update(void)
{
  led_state = 1;
  distance = 0;
  
  switch(system_mode)
  {
    case 1: // ��һ�⣺ֱ����ʻ A -> B
      if(point_count == 1)
      {
        pid_running = 0;
        Motor_Brake(&left_motor);
        Motor_Brake(&right_motor);
      }
      break;
    case 2: // �ڶ��⣺����һȦ A -> B -> C -> D
      if(point_count == 1)
        pid_control_mode = 1; // ʹ��ѭ��������
      else if(point_count == 2)
      {
        pid_control_mode = 0; // ʹ�ýǶȻ�����
        pid_set_target(&pid_angle, -176);
      }
      else if(point_count == 3)
        pid_control_mode = 1; // ʹ��ѭ��������
      else if(point_count == 4)
      {
        pid_running = 0;
        Motor_Brake(&left_motor);
        Motor_Brake(&right_motor);
      }
      break;
    case 3: // �����⣺8 �ֻ���һȦ A -> C -> B -> D
      if(point_count == 1)
      {
        pid_control_mode = 1; // ʹ��ѭ��������
      }
      else if(point_count == 2)
      {
        pid_control_mode = 0; // ʹ�ýǶȻ�����
        pid_set_target(&pid_angle, 253);
      }
      else if(point_count == 3)
      {
        pid_control_mode = 1; // ʹ��ѭ��������
      }
      else if(point_count == 4)
      {
        pid_running = 0;
        Motor_Brake(&left_motor);
        Motor_Brake(&right_motor);
      }
      break;
    case 4: // �����⣺8 �ֻ�����Ȧ
      if(point_count == 1)
      {
        pid_control_mode = 1; // ʹ��ѭ��������
      }
      else if(point_count == 2)
      {
        pid_control_mode = 0; // ʹ�ýǶȻ�����
        pid_set_target(&pid_angle, 253 - (0.3 * circle_count)); // ����Ȧ������Ʈ�Ʋ���
      }
      else if(point_count == 3)
        pid_control_mode = 1; // ʹ��ѭ��������
      else if(point_count == 4)
      {
        if(++circle_count >= 4)
        {
          pid_running = 0;
          Motor_Brake(&left_motor);
          Motor_Brake(&right_motor);
        }
        point_count = 0;
        pid_control_mode = 0; // ʹ�ýǶȻ�����
        pid_set_target(&pid_angle, 0 - (0.2 * circle_count)); // ����Ȧ������Ʈ�Ʋ���
      }
      break;
  }
  
  /* ������ʷ��� */
  pid_reset(&pid_line);
  pid_reset(&pid_angle);
}
