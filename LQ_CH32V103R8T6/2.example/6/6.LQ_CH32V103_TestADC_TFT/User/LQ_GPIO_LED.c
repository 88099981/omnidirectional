/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�SPIN27PS/CH32V103R8T6���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2020��10��28��
 �������Ϣ�ο����е�ַ��
 ����    վ��http://www.lqist.cn
 ���Ա����̡�http://longqiu.taobao.com
 ------------------------------------------------
 ��IDE��CH32V103R8T6:MounRiver Studio�����ϰ汾
 ��IDE��MM32SPIN27PS:IAR7.8/MDK5.2�����ϰ汾  
 ��Target �� SPIN27PS/CH32V103R8T6
 ��SYS PLL�� 80/96MHz
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

/*************************************************************************
*  �������ƣ�void LED_Init(void)
*  ����˵����GPIO��ʼ������ LED������PB12��PA4��PC13��PC14��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void GPIO_LED_Init(void)
{
  /*
  // �׿��
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  */
  //��װ��
  PIN_InitConfig(PB10, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PA3, GPO, 1,GPIO_Mode_Out_PP);

}

/*************************************************************************
*  �������ƣ�void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  ����˵����LED����
*  ����˵����LEDn_e LEDno���,LEDs_e sta״̬��/��/��ת
*  �������أ�����״̬��0/1
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
  switch(LEDno)
  {
  case LED0:
    if(sta==ON)        GPIO_ResetBits(LED0p);
    else if(sta==OFF)  GPIO_SetBits(LED0p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED0p))?(GPIO_ResetBits(LED0p)):(GPIO_SetBits(LED0p));
    break;
    
  case LED1:
    if(sta==ON)        GPIO_ResetBits(LED1p);
    else if(sta==OFF)  GPIO_SetBits(LED1p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED1p))?(GPIO_ResetBits(LED1p)):(GPIO_SetBits(LED1p));
    break;
    
  case LED2:
    if(sta==ON)        GPIO_ResetBits(LED2p);
    else if(sta==OFF)  GPIO_SetBits(LED2p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED2p))?(GPIO_ResetBits(LED2p)):(GPIO_SetBits(LED2p));
    break;
    
  case LED3:
    if(sta==ON)        GPIO_ResetBits(LED3p);
    else if(sta==OFF)  GPIO_SetBits(LED3p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED3p))?(GPIO_ResetBits(LED3p)):(GPIO_SetBits(LED3p));
    break;
  case LEDALL:
    if(sta==ON)
    {
      GPIO_ResetBits(LED0p);
      GPIO_ResetBits(LED1p);
      GPIO_ResetBits(LED2p);
      GPIO_ResetBits(LED3p);
    }
    else if(sta==OFF)
    {
      GPIO_SetBits(LED0p);
      GPIO_SetBits(LED1p);
      GPIO_SetBits(LED2p);
      GPIO_SetBits(LED3p);
    }
    else if(sta==RVS)
    {
      (GPIO_ReadOutputDataBit(LED0p))?(GPIO_ResetBits(LED0p)):(GPIO_SetBits(LED0p));
      (GPIO_ReadOutputDataBit(LED1p))?(GPIO_ResetBits(LED1p)):(GPIO_SetBits(LED1p));
      (GPIO_ReadOutputDataBit(LED2p))?(GPIO_ResetBits(LED2p)):(GPIO_SetBits(LED2p));
      (GPIO_ReadOutputDataBit(LED3p))?(GPIO_ResetBits(LED3p)):(GPIO_SetBits(LED3p));
    }
    break;
  default:
    break;
  }
}
/*************************************************************************
*  �������ƣ�void Test_GPIO_LED(void)
*  ����˵�������Գ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��LED��ת
*************************************************************************/
void Test_GPIO_LED(void)
{
  GPIO_LED_Init();
  while(1)
  {
     LED_Ctrl(LEDALL,RVS);
     delayms(200);
  }
}


