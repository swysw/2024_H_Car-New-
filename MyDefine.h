#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

/* ========== HAL ��ͷ�ļ� ========== */
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"

/* ========== C ����ͷ�ļ� ========== */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* ========== �����ͷ�ļ� ========== */
#include "ebtn.h"

#include "oled.h"

// #include "WouoUI.h"
// #include "WouoUI_user.h"

#include "ringbuffer.h"

#include "hardware_iic.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"

#include "pid.h"

/* ========== ������ͷ�ļ� ========== */
#include "led_driver.h"
#include "key_driver.h"
#include "oled_driver.h"
#include "uart_driver.h"
#include "motor_driver.h"
#include "encoder_driver.h"
#include "mpu6050_driver.h"

/* ========== Ӧ�ò�ͷ�ļ� ========== */
#include "led_app.h"
#include "key_app.h"
#include "oled_app.h"
#include "uart_app.h"
#include "gray_app.h"
#include "motor_app.h" 
#include "encoder_app.h"
#include "mpu6050_app.h"
#include "pid_app.h"

/* ========== ���ĵ�����ͷ�ļ� ========== */
#include "Scheduler.h"
#include "Scheduler_Task.h"

#endif
