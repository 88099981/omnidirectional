/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                                                               00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                                  00 **
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  encoder                        00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include"include.h"


extern int16_t ECPULSE1;
extern int16_t ECPULSE2;
extern int16_t ECPULSE3;
extern int16_t ECPULSE4;
void encoder_get(void)
{
#ifdef DOUBLE_CORE_HOST
    LED_Ctrl(LED1,RVS);//LED翻转闪烁
    ECPULSE1 = -Read_Encoder(2); //左电机 母板上编码器1，小车前进为正值
    ECPULSE2 = Read_Encoder(4); //右电机 母板上编码器2，小车前进为正值

#elif defined  DOUBLE_CORE_SLAVE
    ECPULSE3 = Read_Encoder(2); //右后电机 母板上编码器3，小车前进为正值
    ECPULSE4 = -Read_Encoder(4);
#endif
}
char txt[32];
void encoder_print(void)
{




    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(0, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc2: %05d;", ECPULSE2);
    TFTSPI_P8X16Str(0, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示

    sprintf(txt, "Enc3: %05d;", ECPULSE3);
    TFTSPI_P8X16Str(0, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc4: %05d;", ECPULSE4);
    TFTSPI_P8X16Str(0, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
}
