/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *e
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

    //A0的电平发生变化时候，就会触发外部中断0，中断服务函数。
    //其中断服务函数在ISR.C文件中
    gpio_init(B15, GPO, 1, GPIO_PIN_CONFIG);            //B15引脚初始化

    //使用A0,B0,C0,D0引脚 nvic_init里面应该填写EXTI0_IRQn，对应的中断函数为EXTI0_IRQHandler
    //使用A1,B1,C1,D1引脚 nvic_init里面应该填写EXTI1_IRQn，对应的中断函数为EXTI1_IRQHandler
    //使用A2,B2,C2,D2引脚 nvic_init里面应该填写EXTI2_IRQn，对应的中断函数为EXTI2_IRQHandler
    //使用A3,B3,C3,D3引脚 nvic_init里面应该填写EXTI3_IRQn，对应的中断函数为EXTI3_IRQHandler
    //使用A4,B4,C4,D4引脚 nvic_init里面应该填写EXTI4_IRQn，对应的中断函数为EXTI4_IRQHandler
    //使用A5-A9,B5-B9,C5-C9,D5-D9引脚 nvic_init里面应该填写EXTI9_5_IRQn，对应的中断函数为EXTI9_5_IRQHandler
    //使用A10-A15,B10-B15,C10-C15,D10-D15引脚 nvic_init里面应该填写EXTI15_10_IRQn，对应的中断函数为EXTI15_10_IRQHandler

    gpio_interrupt_init(A0, BOTH, GPIO_INT_CONFIG);     //A0初始化为GPIO 双边沿触发 中断
    nvic_init(EXTI0_IRQn, 0, 1, ENABLE);                //EXTI0优先级配置，抢占优先级0，次优先级1
    //在ISR.C文件中EXTI0_IRQHandler中断服务函数中，每触发一次A0的GPIO外部中断LED灯的引脚就翻转一次。

    EnableGlobalIRQ(0);
    while(1)
    {


    }


}



