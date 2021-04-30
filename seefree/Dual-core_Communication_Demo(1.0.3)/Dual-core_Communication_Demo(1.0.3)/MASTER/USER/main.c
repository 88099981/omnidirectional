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

#define E_START                 0       //׼��״̬
#define E_OK                    1       //�ɹ�
#define E_FRAME_HEADER_ERROR    2       //֡ͷ����
#define E_FRAME_RTAIL_ERROR     3       //֡β����

#define LINE_LEN                11      //���ݳ���
uint8   temp_buff[LINE_LEN];            //�������ڽ������ݵ�BUFF
vuint8  uart_flag;                      //�������ݱ�־λ

int16 slave_encoder_left;               //�ӻ��������ֵ
int16 slave_encoder_right;              //�ӻ��ұ�����ֵ
int16 slave_position;                   //�ӻ�ת��ֵ

vuint8 show_flag;                       //������ʾ��־λ
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ�ӻ�����
//  @param      data            ��������
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void get_slave_data(uint8 data)
{
    static uint8 uart_num = 0;
    temp_buff[uart_num++] = data;

    if(1 == uart_num)
    {
        //���յ��ĵ�һ���ַ���Ϊ0xD8��֡ͷ����
        if(0xD8 != temp_buff[0])
        {
            uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }

    if(LINE_LEN == uart_num)
    {
        uart_flag = E_OK;
        //���յ����һ���ֽ�Ϊ0xEE
        if(0xEE == temp_buff[LINE_LEN - 1])
        {
            uart_flag = E_OK;
        }
        else    //���յ����һ���ֽڲ���0xEE��֡β����
        {
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        uart_num = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����Э��Դӻ����͹��������ݣ��������ݽ���
//  @param      *line                           ���ڽ��յ�������BUFF
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void data_analysis(uint8 *line)
{
    if(line[1] == 0xB0)    slave_encoder_left  = ((int16)line[2] << 8) | line[3];
    if(line[4] == 0xB1)    slave_encoder_right = ((int16)line[5] << 8) | line[6];
    if(line[7] == 0xB2)    slave_position      = ((int16)line[8] << 8) | line[9];
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����3�жϷ�����
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void)
{
    uint8 dat;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); //������ڽ����жϱ�־λ
        dat = USART_ReceiveData(USART3);                //��ȡ��������
        get_slave_data(dat);                            //��ÿһ���ֽڵĴ������ݴ���temp_buff�С�
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ⲿ�ж�0�жϷ�����
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI0_IRQHandler(void)
{
    if(SET == EXTI_GetITStatus(EXTI_Line0))
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        if(uart_flag == E_OK)                           //�ȴ���������
        {
            data_analysis(temp_buff);                   //���ݽ���
            uart_flag = E_START;                        //��ձ�־λ
        }


        show_flag = 1;                                  //��Ļ��ʾ��־λ
    }
}

int main(void)
{
    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    ips114_init();

    ips114_showstr(0, 0, "master");
    //�˴���д�û�����(���磺ICM20602�� MPU6050������ͷ���������ɼ��� �����ʼ��)
    //�˴���д�û�����(���磺ICM20602�� MPU6050������ͷ���������ɼ��� �����ʼ��)
    //�˴���д�û�����(���磺ICM20602�� MPU6050������ͷ���������ɼ��� �����ʼ��)


    uart_init(UART_3, 460800, UART3_TX_B10, UART3_RX_B11);  //����3��ʼ����������460800
    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�1 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�
    //���ڵ���ռ���ȼ�һ��Ҫ���ⲿ�жϵ���ռ���ȼ��ߣ���������ʵʱ���մӻ�����
    //���ڵ���ռ���ȼ�һ��Ҫ���ⲿ�жϵ���ռ���ȼ��ߣ���������ʵʱ���մӻ�����
    //���ڵ���ռ���ȼ�һ��Ҫ���ⲿ�жϵ���ռ���ȼ��ߣ���������ʵʱ���մӻ�����

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0��ʼ��ΪGPIO �����ش���
    nvic_init(EXTI0_IRQn, 1, 1, ENABLE);                    //EXTI0���ȼ����ã���ռ���ȼ�1�������ȼ�1


    EnableGlobalIRQ(0);
    while(1)
    {
        if(show_flag)
        {
            //�����յ��Ĵӻ�������ʾ����Ļ�ϡ�
            ips114_showint16(0, 0, slave_encoder_left);
            ips114_showint16(0, 1, slave_encoder_right);
            ips114_showint16(0, 2, slave_position);
            show_flag = 0;
        }
    }
}



