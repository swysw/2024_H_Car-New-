#include "uart_app.h"

extern uint8_t uart_rx_dma_buffer[BUFFER_SIZE]; // DMA ��ȡ������

extern uint8_t ring_buffer_input[BUFFER_SIZE]; // ���λ�������Ӧ����������
extern struct rt_ringbuffer ring_buffer; // ���λ�����

extern uint8_t uart_data_buffer[BUFFER_SIZE]; // ���ݴ�������

void Uart_Init(void)
{
  rt_ringbuffer_init(&ring_buffer, ring_buffer_input, BUFFER_SIZE);
  
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_rx_dma_buffer, BUFFER_SIZE); // ������ȡ�ж�
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT); // �ر� DMA ��"�����ж�"����
}

void Uart_Task(void)
{
  uint16_t uart_data_len = rt_ringbuffer_data_len(&ring_buffer);
  if(uart_data_len > 0)
  {
    rt_ringbuffer_get(&ring_buffer, uart_data_buffer, uart_data_len);
    uart_data_buffer[uart_data_len] = '\0';
    /* ���ݽ��� */

    Uart_Printf(&huart1, "Ringbuffer:%s\r\n", uart_data_buffer);
    
    memset(uart_data_buffer, 0, uart_data_len);
  }
}

