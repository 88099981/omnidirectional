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

#include "headfile.h"




int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    systick_delay_ms(300);  //延时300ms，等待主板其他外设上电成功


    oled_init();     //初始化OLED屏幕

    EnableGlobalIRQ(0);
    while(1)
    {
        oled_p6x8str(0,0,"seekfree.taobao.com");//显示字符串
        oled_printf_int32(0,1,5693,5);          //显示一个变量  去除无效0位

        oled_printf_float(0,2,56.356,5,2);      //显示一个浮点数 去除无效0位  整数位显示5位  小数位显示2位

        //汉字显示的X Y坐标与其他函数不一致，汉字显示函数坐标x y都是以像素为单位
        //这样可以使得位置调节更加随意
        oled_print_chinese(0,3,16,oled_16x16_chinese[0],4);


        systick_delay_ms(20);
    }


}
