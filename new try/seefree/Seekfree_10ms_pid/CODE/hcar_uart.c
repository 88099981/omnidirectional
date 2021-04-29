/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                                                               00**                                            00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                                  00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：         主从通信                                                           00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_uart.h"
#include "headfile.h"

uint8   temp_buff[LINE_LEN]={0};            //主机用于接收数据的BUFF
vuint8  uart_flag=0;                      //接收数据标志位

vint16 slave_encoder_left=0;               //从机左编码器值
vint16 slave_encoder_right=0;              //从机右编码器值
vint16 slave_position=0;                   //从机转角值

vuint8 show_flag=0;                        //数据显示标志位
//vuint8 test_getdata=0;
//函数
//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取从机数据
//  @param      data            串口数据
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
        //接收到的第一个字符不为0xD8，帧头错误
        if(0xD8 != temp_buff[0])
        {
            uart_num = 0;
            uart_flag = E_FRAME_HEADER_ERROR;
        }
    }

    if(LINE_LEN == uart_num)
    {
        uart_flag = E_OK;
        //接收到最后一个字节为0xEE
        if(0xEE == temp_buff[LINE_LEN - 1])
        {
            uart_flag = E_OK;
        }
        else    //接收到最后一个字节不是0xEE，帧尾错误
        {
            uart_flag = E_FRAME_RTAIL_ERROR;
        }
        uart_num = 0;
    }
    //lcd_showint16(0, 4, slave_encoder_left);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据协议对从机发送过来的数据，进行数据解析
//  @param      *line                           串口接收到的数据BUFF
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
//  @brief      获取传感器数据
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage: 仅供副核使用
//-------------------------------------------------------------------------------------------------------------------
void get_sensor_data(void)
{
    //这里仅仅是提供一个模拟数据
//    slave_encoder_left++;
//    slave_encoder_right--;
    //slave_position += 10;

    slave_encoder_left=timer_quad_get(TIMER_2);//已校准
    slave_encoder_right=-timer_quad_get(TIMER_3);//已校准
    timer_quad_clear(TIMER_2);                  //清空计数器
    timer_quad_clear(TIMER_3);                  //清空计数器
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据协议处理数据
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void process_data(void)
{
    temp_buff[0] = 0xD8;                         //帧头
    temp_buff[1] = 0xB0;                         //功能字
    temp_buff[2] = slave_encoder_left>>8;        //数据高8位
    temp_buff[3] = slave_encoder_left&0xFF;      //数据低8位

    temp_buff[4] = 0xB1;                         //功能字
    temp_buff[5] = slave_encoder_right>>8;       //数据高8位
    temp_buff[6] = slave_encoder_right&0xFF;     //数据低8位

    temp_buff[7] = 0xB2;                         //功能字
    temp_buff[8] = slave_position>>8;            //数据高8位
    temp_buff[9] = slave_position&0xFF;          //数据低8位

    temp_buff[10] = 0xEE;                        //帧尾
}

