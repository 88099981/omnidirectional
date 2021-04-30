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

#ifdef DOUBLE_CORE_HOST
int main(void)
{
    DisableGlobalIRQ();
    board_init();       //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    systick_delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�
    hcar_init();

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0��ʼ��ΪGPIO �����ش���
    nvic_init(EXTI0_IRQn, 0, 1, ENABLE);                    //EXTI0���ȼ����ã���ռ���ȼ�1�������ȼ�1

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
    get_cut_array();
    EnableGlobalIRQ(0);
    while(1)
    {

        if(mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;
            cut_image_to_img();
            binary_img();
            //zebra_crossing();
            get_new_edge();
            set_middline_black();
            road_seek();
            //ʹ��������ʾ����������ԭʼͼ���С �Լ�������Ҫ��ʾ�Ĵ�С�Զ��������Ż��߷Ŵ���ʾ
            lcd_displayimage032_zoom(img[0], 80, 60, 160, 128);

        }

    }
}

#elif  defined DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)//˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���
{


    // ϵͳ����ʱ������ʼ��
    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    hcar_init();
    lcd_init();
    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //ͬ�����ų�ʼ��
    timer_pit_interrupt_ms(TIMER_4, 20);                    //��ʱ��4��ʼ��

//    seekfree_wireless_init();
//    uint8 test[]="This is wireless application.";
//    seekfree_wireless_send_buff(test, sizeof(test)-1);
    //int16 time=0;
    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�0 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�

    EnableGlobalIRQ(0);
    while(1)
    {
//        time++;
        //�����յ��Ĵӻ�������ʾ����Ļ�ϡ�
//        lcd_showint16(0, 0, slave_encoder_left);
//        lcd_showint16(0, 1, slave_encoder_right);
//        lcd_showint16(0, 2, slave_position);
//        lcd_showint16(0, 3, time);

        lcd_showstr(0,0,"x_sum1:");
        lcd_showint16(12*8,0,sum_x1);
        lcd_showstr(0,1,"x_sum2:");
        lcd_showint16(12*8,1,sum_x2);
        lcd_showstr(0,2,"r_turn_flag:");
        lcd_showuint8(12*8,2,r_turn_flag);
        lcd_showstr(0,3,"l_turn_flag:");
        lcd_showuint8(12*8,3,l_turn_flag);
        lcd_showstr(0,4,"cross_flag:");
        lcd_showuint8(12*8,4,cross_flag);
//        lcd_showstr(0,2,"time:");
//        lcd_showint16(12*8, 2, slave_position);

    }
}
#endif
