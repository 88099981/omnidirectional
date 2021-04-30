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
    systick_delay_ms(300);
    //printf("this is icm20602 example\r\n");
    //�����̽���ʹ��Ӳ��SPI�������ݲɼ�����ICM20602�ļ��ڣ���д�õ�Ӳ��IIC��ģ��IICͨѶ��ʽ
//    simiic_init();                //ʹ��ģ��IICͨѶ
//    icm20602_init();              //ʹ��ģ��IICͨѶ
    lcd_init();

    icm20602_init_spi();          //ʹ��Ӳ��SPIͨѶ




    EnableGlobalIRQ(0);
    while(1)
    {

        //Ӳ��SPI�ɼ�
        get_icm20602_accdata_spi();
        get_icm20602_gyro_spi();

        //���IIC�ɼ�
//        get_icm20602_accdata();
//        get_icm20602_gyro();



//        //ͨ�����ߵ��Թ��ܣ��鿴 icm_gyro_x icm_gyro_y icm_gyro_z icm_acc_x icm_acc_y icm_acc_z ����ֵ���ɡ�
//        //ʹ�÷������Ҽ������������icm_gyro_xѡ�� ��Ӳ鿴���ʽ ������watch���ڿ�������ʵʱֵ��

//        icm_gyro_x,icm_gyro_y,icm_gyro_z; ����������ֵ
//        icm_acc_x,icm_acc_y,icm_acc_z;    ������ٶȼ�ֵ

                lcd_showstr(0,0,"icm_gyro_x:");
                lcd_showint16(12*8,0,icm_gyro_x);
                lcd_showstr(0,1,"icm_gyro_y:");
                lcd_showint16(12*8,1,icm_gyro_y);
                lcd_showstr(0,2,"icm_gyro_z:");
                lcd_showint16(12*8,2,icm_gyro_z);
                lcd_showstr(0,3,"icm_acc_x:");
                lcd_showint16(12*8,3,icm_acc_x);
                lcd_showstr(0,4,"icm_acc_y:");
                lcd_showint16(12*8,4,icm_acc_y);
                lcd_showstr(0,5,"icm_acc_z:");
                lcd_showuint16(12*8,5,icm_acc_z);

        systick_delay_ms(100);//��ʱ


    }
}





