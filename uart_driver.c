#include "uart_driver.h"


uint8_t uart_rx_dma_buffer[BUFFER_SIZE]; // DMA ��ȡ������

uint8_t ring_buffer_input[BUFFER_SIZE]; // ���λ�������Ӧ����������
struct rt_ringbuffer ring_buffer; // ���λ�����

uint8_t uart_data_buffer[BUFFER_SIZE]; // ���ݴ�������

int Uart_Printf(UART_HandleTypeDef *huart, const char *format, ...)
{
	char buffer[512]; // ��ʱ�洢��ʽ������ַ���
	va_list arg;      // ����ɱ����
	int len;          // �����ַ�������

	va_start(arg, format);
	// ��ȫ�ظ�ʽ���ַ����� buffer
	len = vsnprintf(buffer, sizeof(buffer), format, arg);
	va_end(arg);

	// ͨ�� HAL �ⷢ�� buffer �е�����
	HAL_UART_Transmit(huart, (uint8_t *)buffer, (uint16_t)len, 0xFF);
	return len;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // 1. ȷ����Ŀ�괮�� (USART1)
    if (huart->Instance == USART1)
    {
        // 2. ����ֹͣ��ǰ�� DMA ���� (������ڽ�����)
        //    ��Ϊ�����ж���ζ�ŷ��ͷ��Ѿ�ֹͣ����ֹ DMA �����ȴ������
        HAL_UART_DMAStop(huart);

        // 3. �� DMA ����������Ч������ (Size ���ֽ�) ���Ƶ�����������
        rt_ringbuffer_put(&ring_buffer, uart_rx_dma_buffer, Size);
        // ע�⣺����ʹ���� Size��ֻ����ʵ�ʽ��յ�������
        
        // 4. ����"����֪ͨ��"��������ѭ�������ݴ�����

        // 5. ��� DMA ���ջ�������Ϊ�´ν�����׼��
        //    ��Ȼ memcpy ֻ������ Size �������������������������
        memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));

        // 6. **�ؼ�������������һ�� DMA ���н���**
        //    �����ٴε��ã�����ֻ�������һ��
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_rx_dma_buffer, sizeof(uart_rx_dma_buffer));
        
        // 7. ���֮ǰ�ر��˰����жϣ�������Ҫ�������ٴιر� (������Ҫ)
         __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
    }
}
