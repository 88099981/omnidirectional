/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ȫ������ ��д                                                               00**
**00             ��Ա ����گ�� ������ ʯ����                                                   00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�  go                             00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_gogogo.h"
#include "headfile.h"

#ifdef DOUBLE_CORE_HOST
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////˫��ͨ��֮���������������㣬�����ұ߿���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hcar_gogogo(void)//˫��ͨ��֮���������������㣬�����ұ߿���
{

  // ϵͳ����ʱ������ʼ��
    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    systick_delay_ms(300);

    hcar_init();
    hcar_motor_init();

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0��ʼ��ΪGPIO �����ش���
    nvic_init(EXTI0_IRQn, 1, 1, ENABLE);                    //EXTI0���ȼ����ã���ռ���ȼ�1�������ȼ�1

    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�0 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�
    //hcar_adc_init();//adc

    seekfree_wireless_init();//���ߴ���

    //icm20602_init_spi();//������

    //mt9v03x_init();//����ͷ
    //get_cut_array();//�ı�ͼƬ�ߴ�

    EnableGlobalIRQ(0); //���ж������

    //hcar_start();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    while(1)
    {
        hcar_key_get();
//        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
        if(key1_flag)
        {
            key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            aim_y-=50;
        }

        if(key2_flag)
        {
            key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            aim_y=300;
        }

        if(key3_flag)
        {
            key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            aim_y+=50;
        }

//        hcar_adc_get();
//        hcar_stop();
        //get_icm20602_accdata_spi();
        //get_icm20602_gyro_spi();
        //��TFT����ʾ���Ա���
//        lcd_showstr(0,0,"icm_gyro_x:");
//        lcd_showint16(12*8,0,icm_gyro_x);
//        lcd_showstr(0,1,"icm_gyro_y:");
//        lcd_showint16(12*8,1,icm_gyro_y);
//        lcd_showstr(0,2,"icm_gyro_z:");
//        lcd_showint16(12*8,2,icm_gyro_z);
//        lcd_showstr(0,3,"icm_acc_x:");
//        lcd_showint16(12*8,3,icm_acc_x);
//        lcd_showstr(0,4,"icm_acc_y:");
//        lcd_showint16(12*8,4,icm_acc_y);
//        lcd_showstr(0,5,"icm_acc_z:");
//        lcd_showuint16(12*8,5,icm_acc_z);

        lcd_showstr(0,0,"aim_x:");
        lcd_showint16(12*8,0,aim_x);
        lcd_showstr(0,1,"aim_y:");
        lcd_showint16(12*8,1,aim_y);
        lcd_showstr(0,2,"aim_z:");
        lcd_showint16(12*8,2,aim_z);
        lcd_showstr(0,3,"kp:");
        lcd_showfloat(12*8,3,kp_host,2,2);
        lcd_showstr(0,4,"ki:");
        lcd_showfloat(12*8,4,ki_host,2,2);
//        lcd_showstr(0,5,"break_sign:");
//        lcd_showint8(12*8,5,break_flag);



        //�����յ��Ĵӻ�������ʾ����Ļ�ϡ�
//        lcd_showstr(0, 0, "host_encoder_left:");
//        lcd_showint16(0, 1, host_encoder_left);
//        lcd_showstr(0, 2, "host_encoder_right:");
//        lcd_showint16(0, 3, host_encoder_right);
//        lcd_showstr(0, 4, "slave_encoder_right:");
//        lcd_showint16(0, 5, slave_encoder_right);
//        lcd_showstr(0, 6, "slave_encoder_left:");
//        lcd_showint16(0, 7, slave_encoder_left);

//        lcd_showstr(0,2,"ad_value1:");
//        lcd_showuint16(12*8,2,ad_value1);
//        lcd_showstr(0,3,"ad_value2:");
//        lcd_showuint16(12*8,3,ad_value2);
//        lcd_showstr(0,4,"ad_value3:");
//        lcd_showuint16(12*8,4,ad_value3);
//        lcd_showstr(0,5,"ad_value4:");
//        lcd_showuint16(12*8,5,ad_value4);


    }
}

#elif  defined DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hcar_gogogo(void)//˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���
{


    // ϵͳ����ʱ������ʼ��
    DisableGlobalIRQ();
    board_init();           //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    hcar_init();

    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //ͬ�����ų�ʼ��
    timer_pit_interrupt_ms(TIMER_4, 20);                    //��ʱ��4��ʼ��

//    seekfree_wireless_init();
//    uint8 test[]="This is wireless application.";
//    seekfree_wireless_send_buff(test, sizeof(test)-1);

    EnableGlobalIRQ(0);
    while(1)
    {
        hcar_key_get();
//        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�
        if(key1_flag)
        {
            key1_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            kp_big+=50;
        }

        if(key2_flag)
        {
            key2_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            ki_big+=1;
        }

        if(key3_flag)
        {
            key3_flag = 0;//ʹ�ð���֮��Ӧ�������־λ
            kp_big=1500;
            ki_big=36;
        }
        //�����յ��Ĵӻ�������ʾ����Ļ�ϡ�
        lcd_showint16(0, 0, slave_encoder_left);
        lcd_showint16(0, 1, slave_encoder_right);
        lcd_showint16(0, 2, slave_position);
        lcd_showstr(0,3,"kp:");
        lcd_showfloat(12*8,3,kp_big,2,2);
        lcd_showstr(0,4,"ki:");
        lcd_showfloat(12*8,4,ki_big,2,2);


    }
}
#endif
