/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技SPIN27PS/CH32V103R8T6核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】CH32V103R8T6:MounRiver Studio及以上版本
 【IDE】MM32SPIN27PS:IAR7.8/MDK5.2及以上版本  
 【Target 】 SPIN27PS/CH32V103R8T6
 【SYS PLL】 80/96MHz
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

/*************************************************************************
 *  函数名称：void TFT_Show_Camera_Info(void)
 *  功能说明：显示各种所需信息
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：      四轮摄像头车
 *************************************************************************/
void TFT_Show_Camera_Info (void)
{
    char txt[16] = "X:";

    int16_t mps = 0;    // 速度：m/s,毫米数值
    int16_t pulse100 = 0;
    uint16_t bat = 0;
    
    pulse100 = (int16_t) (RAllPulse / 100);
    sprintf(txt, "AP:%05d00", pulse100);           //
    TFTSPI_P8X16Str(3, 4, txt, u16RED, u16BLUE);   // 显示赛道偏差参数

    // TFTSPI_Road(18, 0, LCDH, LCDW, (unsigned char *)Image_Use); // TFT1.8动态显示摄像头灰度图像
    TFTSPI_BinRoad(18, 0, LCDH, LCDW, (unsigned char *) Bin_Image);  // TFT1.8动态显示摄像头二进制图像
    sprintf(txt, "%04d,%04d,%04d", OFFSET0, OFFSET1, OFFSET2);
    TFTSPI_P8X16Str(0, 5, txt, u16RED, u16BLUE);       // 显示赛道偏差参数
    BatVolt       = ADC_Read(6);  // 刷新电池电压
    bat = BatVolt * 11 / 25;  // x/4095*3.3*100*5.7
    sprintf(txt, "B:%d.%02dV %d.%02dm/s", bat / 100, bat % 100, mps / 1000, (mps / 10) % 100);  // *3.3/4095*3
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLUE);   // 字符串显示
    // 电机和舵机参数显示
    sprintf(txt, "Sv:%04d Rno:%d", ServoDuty, CircleNumber);
    TFTSPI_P8X16Str(1, 7, txt, u16RED, u16BLUE);     // 显示舵机，电机1，编码器1数值
    sprintf(txt, "M1:%04d, M2:%04d ", MotorDuty1, MotorDuty2);
    TFTSPI_P8X16Str(0, 8, txt, u16RED, u16BLUE);     // 电机1-2数值
    sprintf(txt, "E1:%04d, E2:%04d ", ECPULSE1, ECPULSE2);
    TFTSPI_P8X16Str(0, 9, txt, u16RED, u16BLUE);     // 编码器1-2数值
}
/*************************************************************************
 *  函数名称：void CameraCar(void)
 *  功能说明：电磁车双电机差速控制
 -->1.入门算法：简单的分段比例控制算法，教学演示控制算法；
 2.进阶算法：PID典型应用控制算法，教学演示控制算法；
 3.高端算法：改进粒子群协同控制算法；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年10月28日
 *  备    注：驱动2个电机
 *************************************************************************/
void CameraCar (void)
{
    // 摄像头初始化
    CAMERA_Init(50);
    TFTSPI_P8X16Str(2, 3, "LQ 9V034 Car", u16RED, u16GREEN);
    TFTSPI_P8X16Str(1, 5, "K2 Show Video", u16RED, u16GREEN);
    delayms(500);
    TFTSPI_CLS(u16BLUE);            // 清屏
    RAllPulse = 0;                  // 全局变量，脉冲计数总数
  
    while (1)
    {
        LED_Ctrl(LED1, RVS);     // LED闪烁 指示程序运行状态
        //if (Camera_Flag == 2)
        {
            Camera_Flag = 0;     // 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据
            Get_Use_Image();     // 取出赛道及显示所需图像数据
            Get_Bin_Image(2);    // 转换为01格式数据，0、1原图；2、3边沿提取
            Bin_Image_Filter();  // 滤波，三面被围的数据将被修改为同一数值
            Seek_Road();         // 通过黑白区域面积差计算赛道偏差值

            // 计算赛道偏差值，系数越大打角越早，数值跟舵机的范围有关，此处为±160左右，默认为7，
            ServoDuty = Servo_Center_Mid - (OFFSET1 + OFFSET2 + OFFSET2) * 1 / 7;

            // 圆环处理，如果面积为负数，数值越大说明越偏左边；
            if((OFFSET2 < -300)||(OFFSET2 > 300))
                ServoDuty = Servo_Center_Mid - OFFSET2 / 7;

            ServoCtrl(ServoDuty);     // 舵机PWM输出，转向

            // SPEED正负标识方向，负数为正向
            MotorDuty1 = MtTargetDuty + ECPULSE1 * 4 - (OFFSET1 + OFFSET2 + OFFSET2) / 10;        // 电机PWM
            MotorDuty2 = MtTargetDuty - ECPULSE2 * 4 + (OFFSET1 + OFFSET2 + OFFSET2) / 10;        // 双电机差分，需要去掉abs

            MotorCtrl(MotorDuty1, MotorDuty2);        // 四轮电机驱动
            // MotorCtrl(2500, 2500); // 电机PWM固定功率输出
        }

    }
}
