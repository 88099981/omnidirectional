/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            main
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ790875685)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�

//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//�Ҽ��������̣�ѡ��ˢ��


// �������Ƽ�ʹ��1.8��TFT��ʾ���۲�Ч��������û��1.8��TFT��������ͨ������ʱ��Watch���ڹ۲찴��״̬��
// ͨ�����ߵ��Թ��ܣ��鿴 key1_status key2_status key3_status  ����ֵ���ɡ�
// �ڱ������У�����������ʱ������״ֵ̬�ͻ��һ��
#include "headfile.h"

#define BEEP_PIN   D2       //�������������


int main(void)
{

    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    systick_delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�


    //��ʼ������������
    gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);

    EnableGlobalIRQ(0);

    while(1)
    {
        gpio_set(BEEP_PIN,0);//�رշ�����
        systick_delay_ms(1000);
        gpio_set(BEEP_PIN,1);//�򿪷�����
        systick_delay_ms(1000);
    }


}
