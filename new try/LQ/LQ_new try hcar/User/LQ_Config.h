#ifndef __LQ_Config_H__
#define __LQ_Config_H__
#include "include.h"

//ͨ��һ�������궨��ѡ��ͬ�ĺ��İ�
#define DOUBLE_CORE_HOST     //˫��ͨ��֮���������������㣬�����ұ߿���
//#define DOUBLE_CORE_SLAVE    //˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���

//#define USEOLED        //ʹ��OLED����TFT1.8ģ�飬Ĭ��ΪTFT1.8
#define USETFT           //Ĭ��ΪTFT1.8

//#define USETIMER1DMA   //ʹ��TIMER1+DMA
#define USEREAD          //������ȡ��ʽ����ͷ����

#define LQMT9V034        //Ĭ��Ϊ��������ͷ
#define USELQMT9V034     //ʹ����������ͷ

#define IMAGEH  MT9V034_IMAGEH   //����ͷ�ɼ��߶�
#define IMAGEW  MT9V034_IMAGEW   //����ͷ�ɼ����

// UART Printf Definition
#define DEBUG_UART1    1
#define DEBUG_UART2    2
#define DEBUG_UART3    3

// DEBUG UATR Definition
#define DEBUG   DEBUG_UART1
// #define DEBUG   DEBUG_UART2
// #define DEBUG   DEBUG_UART3

#endif









