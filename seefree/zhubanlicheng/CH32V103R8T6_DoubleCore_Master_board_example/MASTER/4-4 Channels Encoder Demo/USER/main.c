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
int16 encoder_data[2];
int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    //����ı�������֧����������
    //CH32V103R8T6  ���������Ƽ��Ƕȱ�������˫�������Ƽ�������������������Ƽ�������ı�����
    printf("encoder example");

    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);


    EnableGlobalIRQ(0);
    for(;;)
    {

        encoder_data[0] = timer_quad_get(TIMER_2);      //������ȡֵ
        encoder_data[1] = timer_quad_get(TIMER_3);      //������ȡֵ
        timer_quad_clear(TIMER_2);                      //��ռ�����
        timer_quad_clear(TIMER_3);                      //��ռ�����


        printf("encoder_data[0] = %d\r\n", encoder_data[0]);
        printf("encoder_data[1] = %d\r\n", encoder_data[1]);


//        data_conversion(encoder_data[0], encoder_data[1], encoder_data[2], encoder_data[3], virtual_scope_data); //��������ʾ������������
//        uart_putbuff(UART_1, virtual_scope_data, 10);  //����ת����ɺ�ʹ�ô��ڷ��ͽ���������ݷ��ͳ�ȥ

        systick_delay_ms(10);                                                   //��ʱ10ms
    }


}
