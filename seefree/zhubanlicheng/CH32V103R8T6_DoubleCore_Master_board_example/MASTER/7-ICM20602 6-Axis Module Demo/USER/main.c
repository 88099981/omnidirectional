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
    systick_delay_ms(300);
    //printf("this is icm20602 example\r\n");
    //本例程仅仅使用硬件SPI进行数据采集，在ICM20602文件内，有写好的硬件IIC、模拟IIC通讯方式
//    simiic_init();                //使用模拟IIC通讯
//    icm20602_init();              //使用模拟IIC通讯
    lcd_init();

    icm20602_init_spi();          //使用硬件SPI通讯




    EnableGlobalIRQ(0);
    while(1)
    {

        //硬件SPI采集
        get_icm20602_accdata_spi();
        get_icm20602_gyro_spi();

        //软件IIC采集
//        get_icm20602_accdata();
//        get_icm20602_gyro();



//        //通过在线调试功能，查看 icm_gyro_x icm_gyro_y icm_gyro_z icm_acc_x icm_acc_y icm_acc_z 的数值即可。
//        //使用方法，右键点击变量例如icm_gyro_x选择 添加查看表达式 即可在watch窗口看到变量实时值。

//        icm_gyro_x,icm_gyro_y,icm_gyro_z; 三轴陀螺仪值
//        icm_acc_x,icm_acc_y,icm_acc_z;    三轴加速度计值

                lcd_showstr(0,0,"icm_gyro_x:");
                lcd_showint16(12*8,0,icm_gyro_x);
                lcd_showstr(0,1,"icm_gyro_y:");
                lcd_showint16(12*8,1,icm_gyro_y);
                lcd_showstr(0,2,"icm_gyro_z:");
                lcd_showint16(12*8,2,icm_gyro_z);
                lcd_showstr(0,3,"icm_acc_x:");
                lcd_showint16(12*8,3,icm_acc_x);
                lcd_showstr(0,4,"icm_acc_y:");
                lcd_showint16(12*8,4,icm_acc_y);
                lcd_showstr(0,5,"icm_acc_z:");
                lcd_showuint16(12*8,5,icm_acc_z);

        systick_delay_ms(100);//延时


    }
}





