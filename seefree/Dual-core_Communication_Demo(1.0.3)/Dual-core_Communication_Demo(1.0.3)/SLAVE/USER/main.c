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

#define LINE_LEN                11              //���ݳ���
uint8 temp_buff[LINE_LEN];                      //�ӻ���������������BUFF

int16 slave_encoder_left;                       //�ӻ��������ֵ
int16 slave_encoder_right;                      //�ӻ��ұ�����ֵ
int16 slave_position;                           //�ӻ�ת��ֵ

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����������
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void get_sensor_data(void)
{
    //����������ṩһ��ģ������
    slave_encoder_left++;
    slave_encoder_right--;
    slave_position += 10;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����Э�鴦������
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void process_data(void)
{
    temp_buff[0] = 0xD8;                         //֡ͷ
    temp_buff[1] = 0xB0;                         //������
    temp_buff[2] = slave_encoder_left>>8;        //���ݸ�8λ
    temp_buff[3] = slave_encoder_left&0xFF;      //���ݵ�8λ

    temp_buff[4] = 0xB1;                         //������
    temp_buff[5] = slave_encoder_right>>8;       //���ݸ�8λ
    temp_buff[6] = slave_encoder_right&0xFF;     //���ݵ�8λ

    temp_buff[7] = 0xB2;                         //������
    temp_buff[8] = slave_position>>8;            //���ݸ�8λ
    temp_buff[9] = slave_position&0xFF;          //���ݵ�8λ

    temp_buff[10] = 0xEE;                        //֡β
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʱ��4�жϷ�����
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        get_sensor_data();                          //��ȡ���������ݡ�
        process_data();                             //����Э�鴦�����ݣ�������temp_buff�С�

        uart_putbuff(UART_3, temp_buff, LINE_LEN);  //ͨ������3�����ݷ��ͳ�ȥ��

        GPIO_PIN_SET(A0);                           //A0��������



        GPIO_PIN_RESET(A0);                         //A0��������
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //����жϱ�־λ
    }
}

int main(void)
{
    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    systick_delay_ms(3000);                                 //�ӻ���ʱ3���ӣ��ȴ�������ʼ���ɹ���
    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //ͬ�����ų�ʼ��
    uart_init(UART_3, 460800, UART3_TX_B10, UART3_RX_B11);  //����3��ʼ����������460800
    timer_pit_interrupt_ms(TIMER_4, 5);                     //��ʱ��4��ʼ��

    EnableGlobalIRQ(0);
    while(1)
    {

    }
}



