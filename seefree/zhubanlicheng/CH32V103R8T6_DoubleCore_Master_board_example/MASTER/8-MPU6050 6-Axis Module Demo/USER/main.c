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
    board_init();               //务必保留，本函数用于初始化MPU 时钟 调试串口
    systick_delay_ms(300);      //等待外设上电成功

    simiic_init();
    mpu6050_init();

    EnableGlobalIRQ(0);
    while(1)
    {
        mpu6050_get_accdata();
        mpu6050_get_gyro();
        //通过在线调试功能，查看 mpu_gyro_x mpu_gyro_y mpu_gyro_z mpu_acc_x mpu_acc_y mpu_acc_z 的数值即可。
        //使用方法，右键点击变量例如mpu_gyro_x选择 添加查看表达式 即可在watch窗口看到变量实时值。

//          mpu_gyro_x,mpu_gyro_y,mpu_gyro_z; 三轴陀螺仪值
//          mpu_acc_x,mpu_acc_y,mpu_acc_z;    三轴加速度计值
        printf("mpu_gyro_x = %d\r\n", mpu_gyro_x);
        printf("mpu_gyro_y = %d\r\n", mpu_gyro_y);
        printf("mpu_gyro_z = %d\r\n", mpu_gyro_z);

        printf("mpu_acc_x = %d\r\n", mpu_acc_x);
        printf("mpu_acc_y = %d\r\n", mpu_acc_y);
        printf("mpu_acc_z = %d\r\n", mpu_acc_z);

        systick_delay_ms(100);//延时



    }


}

