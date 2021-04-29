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


    //显示模式设置为2  横屏模式
    //显示模式在IPS的h文件内的IPS_DISPLAY_DIR宏定义设置
    ips114_init();     //初始化IPS屏幕

    EnableGlobalIRQ(0);

    while(1)
    {

        ips114_clear(WHITE);                                    //清屏
        ips114_showstr(0,0,"seekfree.taobao.com");              //显示字符串
        ips114_showuint16(0,1,666);                             //显示一个16位无符号整数
        ips114_showint32(0,2,666,3);                            //显示一个32位有符号数  并去除无效0
        ips114_showint32(0,3,-666,3);                           //显示一个32位有符号数  并去除无效0
        ips114_showfloat(0,4,56.35,3,1);                        //显示一个浮点数        并去除整数部分无效0
        //以上函数x坐标设置是以像素为单位   y坐标是以字符为单位

        //汉字显示的X Y坐标与其他函数不一致，汉字显示函数坐标x y都是以像素为单位
        //这样可以使得位置调节更加随意
        ips114_display_chinese(0,5*16,16,chinese_test[0],4,RED);

        //以下为色彩测试，红绿蓝全屏各显示一秒。
        systick_delay_ms(2000);                                 //延时1秒
        ips114_clear(RED);
        systick_delay_ms(1000);                                 //延时1秒
        ips114_clear(GREEN);
        systick_delay_ms(1000);                                 //延时1秒
        ips114_clear(BLUE);
        systick_delay_ms(1000);                                 //延时1秒

    }
}


