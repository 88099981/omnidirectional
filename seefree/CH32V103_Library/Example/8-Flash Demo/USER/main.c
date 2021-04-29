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

uint32 write_buff[10] = {0,1,2,3,4,5,6,7,8,9};
uint32 read_buff[10];

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口


    //将write_buff中的10个数据写入第15个扇区的第0页
    //flash_page_program(FLASH_SECTION_15, FLASH_PAGE_0, write_buff, 10);
    //将第15个扇区的第0页 读取到read_buff中
    flash_page_read(FLASH_SECTION_15, FLASH_PAGE_0, read_buff, 10);


    //使用在线调试查看 read_buff 值，可以判断flash操作是否成功。
    EnableGlobalIRQ(0);
    uint8 i=0;
    for(;;)
    {
        for(i=0;i<10;i++)
            printf("read_buff = %d\r\n", read_buff[i]);

        systick_delay_ms(1000);


    }


}



