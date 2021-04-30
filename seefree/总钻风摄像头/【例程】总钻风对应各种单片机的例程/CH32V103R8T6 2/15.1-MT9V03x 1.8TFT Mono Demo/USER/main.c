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
    board_init();       //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    systick_delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�

    //��ʾģʽ����Ϊ3  ����ģʽ
    //��ʾģʽ��SEEKFREE_18TFT.h�ļ��ڵ�TFT_DISPLAY_DIR�궨������
    lcd_init();         //��ʼ��TFT��Ļ
    lcd_showstr(0,0,"SEEKFREE MT9V03x");
    lcd_showstr(0,1,"Initializing...");
    //�����Ļû���κ���ʾ��������Ļ����

    //mt9v03x_init(); //��ʼ������ͷ ʹ��CSI�ӿ�
    //�����Ļһֱ��ʾ��ʼ����Ϣ����������ͷ����
    //���ʹ�����壬һֱ����while(!uart_receive_flag)�������Ƿ�������OK?
    //���ͼ��ֻ�ɼ�һ�Σ����鳡�ź�(VSY)�Ƿ�����OK?
    mt9v03x_init();


    lcd_showstr(0,1,"     OK...     ");
    systick_delay_ms(500);

    EnableGlobalIRQ(0);
    while(1)
    {
        if(mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;
            //ʹ��������ʾ����������ԭʼͼ���С �Լ�������Ҫ��ʾ�Ĵ�С�Զ��������Ż��߷Ŵ���ʾ
            lcd_displayimage032_zoom(mt9v03x_image[0], MT9V03X_W, MT9V03X_H, 160, 128);

        }
    }
}


