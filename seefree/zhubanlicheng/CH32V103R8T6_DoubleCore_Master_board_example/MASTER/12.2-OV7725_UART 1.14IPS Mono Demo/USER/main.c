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

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    //IPS��Ļ����Ϊ����ģʽ,��Ļ���������SEEKFREE_SPI114_SPI.H��IPS114_DISPLAY_DIR�궨���޸�
    ips114_init();                 //1.14��IPS��Ļ��ʼ��
    ips114_showstr(0,0,(uint8 *)"SEEKFREE OV7725");
    ips114_showstr(0,1,(uint8 *)"Initializing...");
    //�����Ļû���κ���ʾ��������Ļ����

    ov7725_uart_init();              //����ͷ��ʼ��
    //����ɼ�����ͼ����ȷ:
    //1.��鿴����ͷ�����ô���������ͷ�����Ƿ���ȷ
    //2.���ʹ�����壬�����Ƿ�������OK?
    //3.���ͼ��ֻ�ɼ�һ�Σ����鳡�ź�(VSY)�Ƿ�����OK?

    ips114_showstr(0,1,(uint8 *)"     OK...     ");
    systick_delay_ms(500);

    //���ж������
    EnableGlobalIRQ(0);;
    while (1)
    {
        //�˴���д��Ҫѭ��ִ�еĴ���
        if(ov7725_uart_finish_flag)
        {
           ips114_displayimage7725(ov7725_uart_image_bin[0], OV7725_UART_W, OV7725_UART_H);
           ov7725_uart_finish_flag = 0;
        }
    }
}


