/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ȫ������ ��д                                                               00**                                            00**
**00             ��Ա ����گ�� ������ ʯ����                                                  00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�         ����ͨ��                                                           00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_uart.h"
#include "headfile.h"

uint8   temp_buff[LINE_LEN]={0};            //�������ڽ������ݵ�BUFF
vuint8  uart_flag=0;                      //�������ݱ�־λ

vint16 slave_encoder_left=0;               //�ӻ��������ֵ
vint16 slave_encoder_right=0;              //�ӻ��ұ�����ֵ
vint16 slave_position=0;                   //�ӻ�ת��ֵ

vuint8 show_flag=0;                        //������ʾ��־λ
//vuint8 test_getdata=0;
//����
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ�ӻ�����
//  @param      data            ��������
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void get_slave_data(uint8 data)
{
    static uint8 uart_num = 0;
    temp_buff[uart_num++] = data;

    if(1 == uart_num)
    {
        //���յ��ĵ�һ���ַ���Ϊ0xD8��֡ͷ����
        if(0xD8 != temp_buff[0])
        {
            uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }

    if(LINE_LEN == uart_num)
    {
        uart_flag = E_OK;
        //���յ����һ���ֽ�Ϊ0xEE
        if(0xEE == temp_buff[LINE_LEN - 1])
        {
            uart_flag = E_OK;
        }
        else    //���յ����һ���ֽڲ���0xEE��֡β����
        {
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        uart_num = 0;
    }
    //lcd_showint16(0, 4, slave_encoder_left);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����Э��Դӻ����͹��������ݣ��������ݽ���
//  @param      *line                           ���ڽ��յ�������BUFF
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void data_analysis(uint8 *line)
{
    if(line[1] == 0xB0)    slave_encoder_left  = ((int16)line[2] << 8) | line[3];
    if(line[4] == 0xB1)    slave_encoder_right = ((int16)line[5] << 8) | line[6];
    if(line[7] == 0xB2)    slave_position      = ((int16)line[8] << 8) | line[9];
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����������
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage: ��������ʹ��
//-------------------------------------------------------------------------------------------------------------------
void get_sensor_data(void)
{
    //����������ṩһ��ģ������
//    slave_encoder_left++;
//    slave_encoder_right--;
    //slave_position += 10;

    slave_encoder_left=timer_quad_get(TIMER_2);//��У׼
    slave_encoder_right=-timer_quad_get(TIMER_3);//��У׼
    timer_quad_clear(TIMER_2);                  //��ռ�����
    timer_quad_clear(TIMER_3);                  //��ռ�����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����Э�鴦������
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void process_data(void)
{
    temp_buff[0] = 0xD8;                         //֡ͷ
    temp_buff[1] = 0xB0;                         //������
    temp_buff[2] = slave_encoder_left>>8;        //���ݸ�8λ
    temp_buff[3] = slave_encoder_left&0xFF;      //���ݵ�8λ

    temp_buff[4] = 0xB1;                         //������
    temp_buff[5] = slave_encoder_right>>8;       //���ݸ�8λ
    temp_buff[6] = slave_encoder_right&0xFF;     //���ݵ�8λ

    temp_buff[7] = 0xB2;                         //������
    temp_buff[8] = slave_position>>8;            //���ݸ�8λ
    temp_buff[9] = slave_position&0xFF;          //���ݵ�8λ

    temp_buff[10] = 0xEE;                        //֡β
}

