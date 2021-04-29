/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                                                               00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                              00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：     key                          00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "zf_gpio.h"
#include "hcar_key.h"
#include "hcar_init.h"
//开关状态变量
vuint8 key1_status = 1;
vuint8 key2_status = 1;
vuint8 key3_status = 1;

//上一次开关状态变量
vuint8 key1_last_status;
vuint8 key2_last_status;
vuint8 key3_last_status;

//开关标志位
vuint8 key1_flag;
vuint8 key2_flag;
vuint8 key3_flag;

void hcar_key_init(void)
{
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);
}
void hcar_key_get(void)
{
    //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
    //保存按键状态
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;

    //读取当前按键状态
    key1_status = gpio_get(KEY1);
    key2_status = gpio_get(KEY2);
    key3_status = gpio_get(KEY3);


    //检测到按键按下之后  并放开置位标志位
    if(key1_status && !key1_last_status)    key1_flag = 1;
    if(key2_status && !key2_last_status)    key2_flag = 1;
    if(key3_status && !key3_last_status)    key3_flag = 1;
    if(key1_flag)
    {
        key1_flag = 0;//使用按键之后，应该清除标志位
        aim_y-=50;
    }

    if(key2_flag)
    {
        key2_flag = 0;//使用按键之后，应该清除标志位
        aim_y=100;
    }

    if(key3_flag)
    {
        key3_flag = 0;//使用按键之后，应该清除标志位
        aim_y+=50;
    }
}
