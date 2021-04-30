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

#include "headfile.h"

int16 enc;
uint8 dir_mode = 0x00;


int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    gpio_init(C0, GPO, 1, GPIO_PIN_CONFIG);
    gpio_init(C1, GPO, 1, GPIO_PIN_CONFIG);


    //CH32V103R8T6�����Ƽ�ʹ�����������������
    //CH32V103R8T6�����Ƽ�ʹ�����������������
    //CH32V103R8T6�����Ƽ�ʹ�����������������

    timer_quad_init(TIMER_2, TIMER2_CHA_A0, TIMER2_CHB_A1);

    EnableGlobalIRQ(0);


    while(1)
    {
        enc = timer_quad_get(TIMER_2);      //������ȡֵ
        timer_quad_clear(TIMER_2);          //��ն�ʱ���ļ�����ֵ
        printf("enc = %d\r\n", enc);
        systick_delay_ms(500);
    }


}



