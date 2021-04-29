#ifndef __LQ_Config_H__
#define __LQ_Config_H__
#include "include.h"

//通过一下两个宏定义选择不同的核心板
#define DOUBLE_CORE_HOST     //双机通信之主机，完成整体计算，插在右边卡槽
//#define DOUBLE_CORE_SLAVE    //双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽

//#define USEOLED        //使用OLED或者TFT1.8模块，默认为TFT1.8
#define USETFT           //默认为TFT1.8

//#define USETIMER1DMA   //使用TIMER1+DMA
#define USEREAD          //连续读取方式摄像头数据

#define LQMT9V034        //默认为神眼摄像头
#define USELQMT9V034     //使用神眼摄像头

#define IMAGEH  MT9V034_IMAGEH   //摄像头采集高度
#define IMAGEW  MT9V034_IMAGEW   //摄像头采集宽度

// UART Printf Definition
#define DEBUG_UART1    1
#define DEBUG_UART2    2
#define DEBUG_UART3    3

// DEBUG UATR Definition
#define DEBUG   DEBUG_UART1
// #define DEBUG   DEBUG_UART2
// #define DEBUG   DEBUG_UART3

#endif









