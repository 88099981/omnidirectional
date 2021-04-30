/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ȫ������ ��д                                                               00**
**00             ��Ա ����گ�� ������ ʯ����                                                  00 **
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�  encoder                        00**
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
    LED_Ctrl(LED1,RVS);//LED��ת��˸
    ECPULSE1 = -Read_Encoder(2); //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    ECPULSE2 = Read_Encoder(4); //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ

#elif defined  DOUBLE_CORE_SLAVE
    ECPULSE3 = Read_Encoder(2); //�Һ��� ĸ���ϱ�����3��С��ǰ��Ϊ��ֵ
    ECPULSE4 = -Read_Encoder(4);
#endif
}
char txt[32];
void encoder_print(void)
{




    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(0, 2, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    sprintf(txt, "Enc2: %05d;", ECPULSE2);
    TFTSPI_P8X16Str(0, 3, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ

    sprintf(txt, "Enc3: %05d;", ECPULSE3);
    TFTSPI_P8X16Str(0, 4, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    sprintf(txt, "Enc4: %05d;", ECPULSE4);
    TFTSPI_P8X16Str(0, 5, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
}
