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
#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

// GPIO 枚举端口编号
typedef enum //
{
        PA0 = 0,  PA1,  PA2,  PA3,  PA4,  PA5,  PA6,  PA7,
        PA8,  PA9,  PA10, PA11, PA12, PA13, PA14, PA15,

        PB0,  PB1,  PB2,  PB3,  PB4,  PB5,  PB6,  PB7,
        PB8,  PB9,  PB10, PB11, PB12, PB13, PB14, PB15,

        PC0,  PC1,  PC2,  PC3,  PC4,  PC5,  PC6,  PC7,
        PC8,  PC9,  PC10, PC11, PC12, PC13, PC14, PC15,

        PD0,  PD1,  PD2,  PD3,  PD4,  PD5,  PD6,  PD7,
        PD8,  PD9,  PD10, PD11, PD12, PD13, PD14, PD15,
        NOPIN
}GPIO_Name_t;
typedef enum
{
    GPI = 0, //管脚输入
    GPO = 1, //管脚输出
} IfxPort_Mode;

/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(pin) (GPIO_TypeDef *)PORTX[pin/16]

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(pin)   (u8)(pin%16)

void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode dir, u8 output,u32 pinconf);
u8 PIN_Read(GPIO_Name_t pin);
void PIN_Write(GPIO_Name_t pin, u8 output);
void PIN_Dir(GPIO_Name_t pin, u8 dir);
void PIN_Reverse(GPIO_Name_t pin);
void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf,u8 prior);


#endif /* 0_APPSW_TRICORE_APP_LQ_GPIO_H_ */
