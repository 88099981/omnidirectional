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
    board_init();               //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    systick_delay_ms(300);      //�ȴ������ϵ�ɹ�

    simiic_init();
    mpu6050_init();

    EnableGlobalIRQ(0);
    while(1)
    {
        mpu6050_get_accdata();
        mpu6050_get_gyro();
        //ͨ�����ߵ��Թ��ܣ��鿴 mpu_gyro_x mpu_gyro_y mpu_gyro_z mpu_acc_x mpu_acc_y mpu_acc_z ����ֵ���ɡ�
        //ʹ�÷������Ҽ������������mpu_gyro_xѡ�� ��Ӳ鿴���ʽ ������watch���ڿ�������ʵʱֵ��

//          mpu_gyro_x,mpu_gyro_y,mpu_gyro_z; ����������ֵ
//          mpu_acc_x,mpu_acc_y,mpu_acc_z;    ������ٶȼ�ֵ
        printf("mpu_gyro_x = %d\r\n", mpu_gyro_x);
        printf("mpu_gyro_y = %d\r\n", mpu_gyro_y);
        printf("mpu_gyro_z = %d\r\n", mpu_gyro_z);

        printf("mpu_acc_x = %d\r\n", mpu_acc_x);
        printf("mpu_acc_y = %d\r\n", mpu_acc_y);
        printf("mpu_acc_z = %d\r\n", mpu_acc_z);

        systick_delay_ms(100);//��ʱ



    }


}

