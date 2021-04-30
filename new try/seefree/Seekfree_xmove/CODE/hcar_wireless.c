/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ȫ������ ��д                                                               00**
**00             ��Ա ����گ�� ������ ʯ����                                                   00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�  wireless                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_wireless.h"
#include "zf_uart.h"
#include "Serial_oscilloscope.h"
#include "SEEKFREE_MT9V03X.h"
#include "hcar_init.h"
#include "hcar_uart.h"

void hcar_pid_wireless(void)
{
    encoder_str[0]=host_encoder_left;
    encoder_str[1]=host_encoder_right;
    encoder_str[2]=slave_encoder_right;
    encoder_str[3]=slave_encoder_left;
    encoder_str[4]=targetSpeed1;
    encoder_str[5]=targetSpeed2;
    encoder_str[6]=targetSpeed3;
    encoder_str[7]=targetSpeed4;
    Data_Send(WIRELESS_UART, encoder_str);
}

void hcar_camera_wireless(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
    uart_putchar(uartn,0x01);uart_putchar(uartn,0xFE);//����֡ͷ
    uart_putbuff(uartn, image, width*height);  //����ͼ��
    uart_putchar(uartn,0xFE);uart_putchar(uartn,0x01);//����֡β
}

