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
    systick_delay_ms(500);

    hcar_init();
    lcd_init();
    hcar_motor_init();

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0��ʼ��ΪGPIO �����ش���
    nvic_init(EXTI0_IRQn, 1, 1, ENABLE);                    //EXTI0���ȼ����ã���ռ���ȼ�1�������ȼ�1

    uart_rx_irq(UART_3, ENABLE);                            //Ĭ����ռ���ȼ�0 �����ȼ�0��
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //������3����ռ���ȼ�����Ϊ��ߣ������ȼ�����Ϊ��ߡ�
    hcar_adc_init();//adc

    seekfree_wireless_init();//���ߴ���

    //icm20602_init_spi();//������

    //mt9v03x_init();//����ͷ
    //get_cut_array();//�ı�ͼƬ�ߴ�

    //gpio_init(A4, GPO, 0, GPIO_PIN_CONFIG);//������

    EnableGlobalIRQ(0); //���ж������
    //aim_y=200;
    //hcar_start();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    while(1)
    {
        hcar_key_get();
        //��־λ��λ֮�󣬿���ʹ�ñ�־λִ���Լ���Ҫ�����¼�


//        hcar_adc_get();
//        hcar_stop();
 /*       if(mt9v03x_finish_flag==1&&turn_sign==0)
        {
            mt9v03x_finish_flag = 0;
            //ʹ��������ʾ����������ԭʼͼ���С �Լ�������Ҫ��ʾ�Ĵ�С�Զ��������Ż��߷Ŵ���ʾ
            //hcar_camera_wireless(DEBUG_UART, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);


            //GPIO_PIN_SET(A4);
            cut_image_to_img();
            binary_img();

            get_new_edge();
            set_middline_black();
            road_seek();
            //GPIO_PIN_RESET(A4);
//��7ms
        }*/

        //Ԫ��ʶ��
        //zebra_crossing();
        //three_fork();
        if(three_fork_flag==1)
        {
            hcar_threefork30();
        }

        //��TFT����ʾ���Ա���
//        lcd_showstr(0,0,"icm_gyro_x:");
//        lcd_showint16(12*8,0,icm_gyro_x);
//        lcd_showstr(0,1,"icm_gyro_y:");
//        lcd_showint16(12*8,1,icm_gyro_y);
//        lcd_showstr(0,2,"icm_gyro_z:");
//        lcd_showint16(12*8,2,icm_gyro_z);
//        lcd_showstr(0,6,"turn_sum:");
//        lcd_showint32(12*8,6,turn_sum,6);
//        lcd_showstr(0,7,"turn_over:");
//        lcd_showuint8(12*8,7,turn_over);

//        lcd_showstr(0,0,"aim_x:");
//        lcd_showint16(12*8,0,aim_x);
        lcd_showstr(0,1,"aim_y:");
        lcd_showint16(12*8,1,aim_y);
//        lcd_showstr(0,2,"aim_z:");
//        lcd_showint16(12*8,2,aim_z);
//        lcd_showstr(0,3,"x_sum1:");
//        lcd_showint8(12*8,3,sum_x1);
//        lcd_showstr(0,4,"x_sum2:");
//        lcd_showint8(12*8,4,sum_x2);
//        lcd_showstr(0,5,"z_error:");
//        lcd_showint16(12*8,5,z_error);
//        lcd_showstr(0,6,"r_turn_flag:");
//        lcd_showint8(12*8,6,r_turn_flag);
//        lcd_showstr(0,7,"l_turn_flag:");
//        lcd_showint8(12*8,7,l_turn_flag);



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
    lcd_init();
    systick_delay_ms(1000);
    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //ͬ�����ų�ʼ��
    timer_pit_interrupt_ms(TIMER_4, 10);                    //��ʱ��4��ʼ��

//    seekfree_wireless_init();
//    uint8 test[]="This is wireless application.";
//    seekfree_wireless_send_buff(test, sizeof(test)-1);

    EnableGlobalIRQ(0);
    while(1)
    {

        //�����յ��Ĵӻ�������ʾ����Ļ�ϡ�
        lcd_showint16(0, 0, slave_encoder_left);
        lcd_showint16(0, 1, slave_encoder_right);
        //lcd_showint16(0, 2, slave_position);


    }
}
#endif
