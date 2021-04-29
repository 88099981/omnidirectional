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


uint8 test1[] = "seekfree wireless to uart test\n";
uint8 test2[] = "seekfree.taobao.com\n";

int main(void)
{

    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    systick_delay_ms(300);  //延时300ms，等待主板其他外设上电成功


    //无线转串口模块相关引脚定义在 wireless.h文件中
    seekfree_wireless_init();


    //对于如何接受无线转串口的数据，其实底层已经写好了，具体可以多看看wireless.c文件，主要采用中断接收的方式
    EnableGlobalIRQ(0);

    while(1)
    {

        seekfree_wireless_send_buff(test1,sizeof(test1)-1);//由于sizeof计算字符串的长度包含了最后一个0，因此需要减一
        seekfree_wireless_send_buff(test2,sizeof(test2)-1);
        systick_delay_ms(10);
    }
}







