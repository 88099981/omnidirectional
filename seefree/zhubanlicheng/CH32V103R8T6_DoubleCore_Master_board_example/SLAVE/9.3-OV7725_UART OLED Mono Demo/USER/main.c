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


    //OLED��Ļ����Ϊ����ģʽ,��Ļ���������SEEKFREE_OLED.H��OLED_DISPLAY_DIR�궨���޸�
    oled_init();    //oled��ʼ��
    oled_p6x8str(0,0,"SEEKFREE OV7725");
    oled_p6x8str(0,1,"Initializing...");
    //�����Ļû���κ���ʾ��������Ļ����

    ov7725_uart_init();  //С����ʼ��
    //����ɼ�����ͼ����ȷ:
    //1.��鿴OV7725_IDCode�Ƿ����0x21���������ڣ�����SCL��SDA�Ƿ�Ӻ�
    //2.���ʹ�����壬�����Ƿ�������OK?
    //3.���ͼ��ֻ�ɼ�һ�Σ����鳡�ź�(VSY)�Ƿ�����OK?

    oled_p6x8str(0,1,"     OK...     ");
    systick_delay_ms(500);
    oled_p6x8str(0,0,"               ");    //����
    oled_p6x8str(0,1,"               ");    //����

    EnableGlobalIRQ(0);
    while(1)
    {
        if(ov7725_uart_finish_flag)
        {
            ov7725_uart_finish_flag = 0;
            Image_Decompression(ov7725_uart_image_bin[0], ov7725_uart_image_dec[0]);     //��ѹͼ��
            oled_dis_bmp(OV7725_UART_H, OV7725_UART_W, ov7725_uart_image_dec[0], 0x7F);  //��ʾͼ��
        }
    }
}


