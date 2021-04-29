/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                                                               00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                              00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  总init                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "zf_uart.h"
#include "zf_qtimer.h"
#include "SEEKFREE_18TFT.h"
#include "hcar_init.h"
#include "hcar_key.h"
//vuint8 ok_sign=0;
vuint8 turn_sign=0;

int16_t host_encoder_left=0;//1轮
int16_t host_encoder_right=0;//2轮
int16 targetSpeed1=0, targetSpeed2=0, targetSpeed3=0, targetSpeed4=0;
int16 aim_x=0;
int16 aim_y=0;
int16 aim_z=0;
float kp_host=0.0, ki_host=0.0;

uint16 encoder_str[10]={0};


void hcar_init(void)
{
    //按键初始化
    hcar_key_init();


    lcd_init();

    uart_init(UART_3, 460800, UART3_TX_B10, UART3_RX_B11);  //串口3初始化，波特率460800
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);//两个编码器
    //adc_init();


}
