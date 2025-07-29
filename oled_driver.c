#include "oled_driver.h"

/**
 * @brief	ʹ������printf�ķ�ʽ��ʾ�ַ�������ʾ6x8��С��ASCII�ַ�
 * @param {uint8_t} x����ʾ�ַ����Ŀ�ʼ������x:0~127
 * @param {uint8_t} y����ʾ�ַ����Ŀ�ʼ������ y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {uint8_t} Char_Size����ʾ�ַ����������С,ѡ������ 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @param {const char *} *format ����ʾ�ĸ�ʽ���ַ���
 * ���磺Oled_Printf(0, 0, 12, 0, "Data = %d", dat);
**/
int Oled_Printf(uint8_t x, uint8_t y, uint8_t Char_Size, uint8_t Color_Turn, const char *format, ...)
{
	char buffer[128]; // ��ʱ�洢��ʽ������ַ���
	va_list arg;      // ����ɱ����
	int len;          // �����ַ�������

	va_start(arg, format);
	// ��ȫ�ظ�ʽ���ַ����� buffer
	len = vsnprintf(buffer, sizeof(buffer), format, arg);
	va_end(arg);

	OLED_ShowString(x, y, buffer, Char_Size, Color_Turn);

	return len;
}

//void OLED_SendBuff(uint8_t buff[8][128])  
//{  
//  for(uint8_t page=0; page<8; page++) 
//  {  // ����8��ҳ
//    OLED_Set_Pos(0, page);             // ������ʼ�е�ַ��ҳ��ַ
//    for(uint8_t col=0; col<128; col++) // ����128��
//    { 
//      OLED_WR_DATA(buff[page][col]); // д���Դ�����
//    }
//  }
//}

void OLED_SendBuff(uint8_t buff[8][128])  
{
    for(uint8_t page=0; page<8; page++) {
        OLED_Set_Pos(0, page);
        // ʹ��HAL_I2C_Mem_Write���п鴫��
        HAL_I2C_Mem_Write(&hi2c1, 
                        0x78,          // OLED��ַ
                        0x40,          // ���ݼĴ�����ַ
                        I2C_MEMADD_SIZE_8BIT,
                        buff[page],    // ֱ�Ӵ�����ҳ����
                        128,           // ÿҳ128�ֽ�
                        100);          // ��ʱʱ��100ms
    }
}
