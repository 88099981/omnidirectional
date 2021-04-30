/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *e
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



int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    //A0�ĵ�ƽ�����仯ʱ�򣬾ͻᴥ���ⲿ�ж�0���жϷ�������
    //���жϷ�������ISR.C�ļ���
    gpio_init(B15, GPO, 1, GPIO_PIN_CONFIG);            //B15���ų�ʼ��

    //ʹ��A0,B0,C0,D0���� nvic_init����Ӧ����дEXTI0_IRQn����Ӧ���жϺ���ΪEXTI0_IRQHandler
    //ʹ��A1,B1,C1,D1���� nvic_init����Ӧ����дEXTI1_IRQn����Ӧ���жϺ���ΪEXTI1_IRQHandler
    //ʹ��A2,B2,C2,D2���� nvic_init����Ӧ����дEXTI2_IRQn����Ӧ���жϺ���ΪEXTI2_IRQHandler
    //ʹ��A3,B3,C3,D3���� nvic_init����Ӧ����дEXTI3_IRQn����Ӧ���жϺ���ΪEXTI3_IRQHandler
    //ʹ��A4,B4,C4,D4���� nvic_init����Ӧ����дEXTI4_IRQn����Ӧ���жϺ���ΪEXTI4_IRQHandler
    //ʹ��A5-A9,B5-B9,C5-C9,D5-D9���� nvic_init����Ӧ����дEXTI9_5_IRQn����Ӧ���жϺ���ΪEXTI9_5_IRQHandler
    //ʹ��A10-A15,B10-B15,C10-C15,D10-D15���� nvic_init����Ӧ����дEXTI15_10_IRQn����Ӧ���жϺ���ΪEXTI15_10_IRQHandler

    gpio_interrupt_init(A0, BOTH, GPIO_INT_CONFIG);     //A0��ʼ��ΪGPIO ˫���ش��� �ж�
    nvic_init(EXTI0_IRQn, 0, 1, ENABLE);                //EXTI0���ȼ����ã���ռ���ȼ�0�������ȼ�1
    //��ISR.C�ļ���EXTI0_IRQHandler�жϷ������У�ÿ����һ��A0��GPIO�ⲿ�ж�LED�Ƶ����žͷ�תһ�Ρ�

    EnableGlobalIRQ(0);
    while(1)
    {


    }


}



