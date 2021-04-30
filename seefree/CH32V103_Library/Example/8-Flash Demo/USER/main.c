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

uint32 write_buff[10] = {0,1,2,3,4,5,6,7,8,9};
uint32 read_buff[10];

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���


    //��write_buff�е�10������д���15�������ĵ�0ҳ
    //flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 10);
    //����15�������ĵ�0ҳ ��ȡ��read_buff��
    flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff, 10);


    //ʹ�����ߵ��Բ鿴 read_buff ֵ�������ж�flash�����Ƿ�ɹ���
    EnableGlobalIRQ(0);
    uint8 i=0;
    for(;;)
    {
        for(i=0;i<10;i++)
            printf("read_buff = %d\r\n", read_buff[i]);

        systick_delay_ms(1000);


    }


}



