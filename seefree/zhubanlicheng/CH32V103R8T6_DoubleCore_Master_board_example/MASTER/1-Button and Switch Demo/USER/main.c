/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
//整套推荐IO查看Projecct文件夹下的TXT文本

//打开新的工程或者工程移动了位置务必执行以下操作
//右键单击工程，选择刷新


// 本例程推荐使用1.8寸TFT显示屏观察效果，若您没有1.8寸TFT屏，可以通过调试时的Watch窗口观察按键状态。
// 通过在线调试功能，查看 key1_status key2_status key3_status  的数值即可。
// 在本例程中，当按键按下时，按键状态值就会加一。

#include "headfile.h"

//定义按键引脚
#define KEY1    C8
#define KEY2    C9
#define KEY3    B2


//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;


//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;


//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;


uint8 test1,test2,test3,test4;

int main(void)
{

    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    systick_delay_ms(300);  //延时300ms，等待主板其他外设上电成功


    //显示模式设置为2  横屏模式
    //显示模式在tft的h文件内的TFT_DISPLAY_DIR宏定义设置
    lcd_init();     //初始化TFT屏幕

    //如果屏幕没有变白，请检查屏幕接线
    //按键初始化
    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);

    EnableGlobalIRQ(0);
    lcd_display_chinese(0,0,16,chinese_test[0],4,RED);
    while(1)
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


        //标志位置位之后，可以使用标志位执行自己想要做的事件
        if(key1_flag)
        {
            key1_flag = 0;//使用按键之后，应该清除标志位
            test1++;
        }

        if(key2_flag)
        {
            key2_flag = 0;//使用按键之后，应该清除标志位
            test2++;
        }

        if(key3_flag)
        {
            key3_flag = 0;//使用按键之后，应该清除标志位
            test3++;
        }
test3++;
        //在TFT上显示测试变量
        lcd_showstr(0,3,"KEY1  TEST:");     lcd_showint32(12*8,3,test1,1);
        lcd_showstr(0,4,"KEY2  TEST:");     lcd_showint32(12*8,4,test2,1);
        lcd_showstr(0,5,"KEY3  TEST:");     lcd_showint32(12*8,5,test3,1);

        systick_delay_ms(10);//延时，按键程序应该保证调用时间不小于10ms

    }
}


