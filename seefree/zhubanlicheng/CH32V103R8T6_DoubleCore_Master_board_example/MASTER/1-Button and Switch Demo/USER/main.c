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

//���尴������
#define KEY1    C8
#define KEY2    C9
#define KEY3    B2


//����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;


//��һ�ο���״̬����
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;


//���ر�־λ
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;


uint8 test1,test2,test3,test4;

int main(void)
{

    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���

    systick_delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�


    //��ʾģʽ����Ϊ2  ����ģʽ
    //��ʾģʽ��tft��h�ļ��ڵ�TFT_DISPLAY_DIR�궨������
    lcd_init();     //��ʼ��TFT��Ļ

    //�����Ļû�б�ף�������Ļ����
    //������ʼ��
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);

    EnableGlobalIRQ(0);
    lcd_display_chinese(0,0,16,chinese_test[0],4,RED);
    while(1)
    {

        //ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
        //���水��״̬
        key1_last_status = key1_status;
        key2_last_status = key2_status;
        key3_last_status = key3_status;

        //��ȡ��ǰ����״̬
        key1_status = gpio_get(KEY1);
        key2_status = gpio_get(KEY2);
        key3_status = gpio_get(KEY3);


        //��⵽��������֮��  ���ſ���λ��־λ
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;


        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
        if(key1_flag)
        {
            key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test1++;
        }

        if(key2_flag)
        {
            key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test2++;
        }

        if(key3_flag)
        {
            key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            test3++;
        }
test3++;
        //��TFT����ʾ���Ա���
        lcd_showstr(0,3,"KEY1  TEST:");     lcd_showint32(12*8,3,test1,1);
        lcd_showstr(0,4,"KEY2  TEST:");     lcd_showint32(12*8,4,test2,1);
        lcd_showstr(0,5,"KEY3  TEST:");     lcd_showint32(12*8,5,test3,1);

        systick_delay_ms(10);//��ʱ����������Ӧ�ñ�֤����ʱ�䲻С��10ms

    }
}


