#include "key_app.h"

extern PID_T pid_speed_left;  // �����ٶȻ� PID ������
extern PID_T pid_speed_right; // �����ٶȻ� PID ������

extern bool pid_running;

void Key_Init()
{
    Ebtn_Init();
}

void Key_Task()
{
    ebtn_process(HAL_GetTick());
}

/* �������¼��Ļص����� */
// ����ԭ��: void (*ebtn_evt_fn)(struct ebtn_btn *btn, ebtn_evt_t evt);
void my_handle_key_event(struct ebtn_btn *btn, ebtn_evt_t evt) {
//    uint16_t key_id = btn->key_id;                 // ��ȡ�����¼��İ��� ID
    uint16_t click_cnt = ebtn_click_get_count(btn); // ��ȡ�������� (���� ONCLICK �¼�ʱ������)
    switch (evt) {
        case EBTN_EVT_ONPRESS: // �����¼� (�����ɹ��󴥷�һ��)
            break;
        case EBTN_EVT_ONRELEASE: // �ͷ��¼� (�����ɹ��󴥷�һ��)
            break;
        case EBTN_EVT_ONCLICK: // ����/�����¼� (���ͷź󣬻�ﵽ�������������ʱ�󴥷�)
            switch (click_cnt) 
            {
              case 1: // ���� PID ����
                pid_running = 1;
                Uart_Printf(&huart1, "key down\r\n");
                break;
            }
            break;
        case EBTN_EVT_KEEPALIVE: // ���ֻ/�����¼� (���³���ʱ�䳬����ֵ�󣬰����ڴ���)
            if(++system_mode >= 5) system_mode = 1;
            break;
        default: // δ֪�¼� (�����ϲ�Ӧ����)
            // printf(" - Unknown Event\n");
            break;
    }
}

