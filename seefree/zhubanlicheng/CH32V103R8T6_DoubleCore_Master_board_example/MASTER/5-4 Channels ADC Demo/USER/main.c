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

#define ADC_CH1 ADC_IN4_A4
#define ADC_CH2 ADC_IN6_A6
#define ADC_CH3 ADC_IN8_B0
#define ADC_CH4 ADC_IN9_B1


uint16  ad_value1;
uint16  ad_value2;
uint16  ad_value3;
uint16  ad_value4;

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    adc_init(ADC_CH1);
    adc_init(ADC_CH2);
    adc_init(ADC_CH3);
    adc_init(ADC_CH4);

    EnableGlobalIRQ(0);
    for(;;)
    {
        //�ɼ�ADC����
        ad_value1 = adc_mean_filter(ADC_CH1, ADC_8BIT, 10);
        ad_value2 = adc_mean_filter(ADC_CH2, ADC_8BIT, 10);
        ad_value3 = adc_mean_filter(ADC_CH3, ADC_8BIT, 10);
        ad_value4 = adc_mean_filter(ADC_CH4, ADC_8BIT, 10);

        //ͨ�����ߵ��Թ��ܣ��鿴ad_value1 ad_value2 ad_value3 ad_value4����ֵ���ɡ�
        //ʹ�÷������Ҽ������������ad_value1ѡ����Ӳ鿴���ʽ������watch���ڿ�������ʵʱֵ��

        //���������ʣ�ΪɶADC������������ֵ�أ���Ӧ����0��
        //�ҵĻش��ǵģ����յ�ʱ������ֵ��������ֻ�����𰸣�����Ϊʲô����Լ��о�ADC�Ľṹ�������ס�

        systick_delay_ms(10);


    }


}


