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

#define MOTOR1_A   C10              //����1�������ת����
#define MOTOR1_B   PWM4_CH1_B6      //����1���PWM����

#define MOTOR2_A   C11               //����2�������ת����
#define MOTOR2_B   PWM4_CH2_B7      //����2���PWM����

#define MOTOR3_A   B12              //����3�������ת����
#define MOTOR3_B   PWM4_CH3_B8      //����3���PWM����

#define MOTOR4_A   A8              //����4�������ת����
#define MOTOR4_B   PWM4_CH4_B9      //����4���PWM����

int32 speed1_power;
int32 speed2_power;
int32 speed3_power;
int32 speed4_power;
int main(void)
{

    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    systick_delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�


    //��ʼ�����PWM���źͷ�������

    //�����������У�����������Ƶ��ѡ��13K-17K
    //���ռ�ձ�ֵPWM_DUTY_MAX ������fsl_pwm.h�ļ����޸� Ĭ��Ϊ50000
    //����һ��PWMģ�� ����������ͨ��ֻ�����Ƶ��һ�� ռ�ձȲ�һ���� PWM CH32V103R8T6ֻ���ĸ�PWMģ�� ÿ��ģ����4��ͨ��
    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B,17000,0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B,17000,0);
    gpio_init(MOTOR3_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR3_B,17000,0);
    gpio_init(MOTOR4_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR4_B,17000,0);

    EnableGlobalIRQ(0);


    //����һ��8��ͨ�����ֱ����4�����������ת
    //������Ҫע�⣬��Ƭ�����޷�ֱ����������ģ���Ƭ��ֻ�ܸ��������źţ��������źŸ�����ģ�飬����ģ�������ѹʹ�õ��ת��
    //�ɲ鿴���ǵ���MOS������ģ��


    //����Ĭ���ٶ�  Ҳ����ͨ�����ߵ���ֱ���޸Ĵ�ֵ  �仯����ٶ�
    speed1_power = 1000;
    speed2_power = 1000;
    speed3_power = 1000;
    speed4_power = 1000;

    while(1)
    {
        //��ο��Ƶ��������ת
        //ÿ������������������źţ���ʵ���ǿ��������������źŵ�ռ�ձȣ����ߵ�ƽʱ�䣩
        //���籾������ʹ�ö�ʱ��1��0��1ͨ����������������0ռ�ձ�Ϊ50%��1ͨ��Ϊ%0,��ô�����ת
        //����0ռ�ձ�Ϊ0%��1ͨ��Ϊ%50,��ô�����ת�������Ϊ0������ֹͣ

        if(0<=speed1_power) //���1   ��ת ����ռ�ձ�Ϊ �ٷ�֮ (1000/TIMER1_PWM_DUTY_MAX*100)
        {
            gpio_set(MOTOR1_A, 1);
            pwm_duty(MOTOR1_B, speed1_power);
        }
        else                //���1   ��ת
        {
            gpio_set(MOTOR1_A, 0);
            pwm_duty(MOTOR1_B, -speed1_power);
        }

        if(0<=speed2_power) //���2   ��ת
        {
            gpio_set(MOTOR2_A, 1);
            pwm_duty(MOTOR2_B, speed2_power);
        }
        else                //���2   ��ת
        {
            gpio_set(MOTOR2_A, 0);
            pwm_duty(MOTOR2_B, -speed2_power);
        }

        if(0<=speed3_power) //���3   ��ת
        {
            gpio_set(MOTOR3_A, 1);
            pwm_duty(MOTOR3_B, speed3_power);
        }
        else                //���3   ��ת
        {
            gpio_set(MOTOR3_A, 0);
            pwm_duty(MOTOR3_B, -speed3_power);
        }

        if(0<=speed4_power) //���3   ��ת
        {
            gpio_set(MOTOR4_A, 1);
            pwm_duty(MOTOR4_B, speed4_power);
        }
        else                //���3   ��ת
        {
            gpio_set(MOTOR4_A, 0);
            pwm_duty(MOTOR4_B, -speed4_power);
        }
    }
}




