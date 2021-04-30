/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ȫ������ ��д                                                               00**
**00             ��Ա ����گ�� ������ ʯ����                                              00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�     key                          00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "zf_gpio.h"
#include "hcar_key.h"
#include "hcar_init.h"
//����״̬����
vuint8 key1_status = 1;
vuint8 key2_status = 1;
vuint8 key3_status = 1;

//��һ�ο���״̬����
vuint8 key1_last_status;
vuint8 key2_last_status;
vuint8 key3_last_status;

//���ر�־λ
vuint8 key1_flag;
vuint8 key2_flag;
vuint8 key3_flag;

void hcar_key_init(void)
{
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);
}
void hcar_key_get(void)
{
    //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
    //���水��״̬
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;

    //��ȡ��ǰ����״̬
    key1_status = gpio_get(KEY1);
    key2_status = gpio_get(KEY2);
    key3_status = gpio_get(KEY3);


    //��⵽��������֮��  ���ſ���λ��־λ
    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key1_flag)
    {
        key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
        aim_y-=50;
    }

    if(key2_flag)
    {
        key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
        aim_y=100;
    }

    if(key3_flag)
    {
        key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
        aim_y+=50;
    }
}
