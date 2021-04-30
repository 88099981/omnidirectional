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

extern unsigned char Image_Data[LCDH+2][LCDW];
const u32 PORTX[4] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE};

void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
// �ɻɹ��ⲿ�ж�
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
  { /*���EXTI12��·����λ*/
    EXTI_ClearITPendingBit(EXTI_Line0);
    //LED_Ctrl(LED1,RVS);
  }
}
void EXTI1_IRQHandler(void)
{
  //���EXTI1��·����λ//
  EXTI_ClearFlag(EXTI_Line1);    //����ͷ��B1
}
// �ɻɹ��ⲿ�ж�
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
  { /*���EXTI12��·����λ*/
    EXTI_ClearITPendingBit(EXTI_Line2);  //�ɻɹ���D2,
    LED_Ctrl(LED1,RVS);
  }
}
#ifdef USETIMER1DMA
void EXTI4_IRQHandler(void)
{
  //���EXTI4��·����λ//
  EXTI_ClearFlag(EXTI_Line4); //����ͷ���ж���B4

  MT9V034_DMA_START();
  //MT9V034_DMA_CH->CNTR = LCDH*MT9V034_IMAGEW;             //���õ�ǰDMA�����ʣ�����������µݼ����üĴ���ֻ����DMA������ʱ���ġ�
  //MT9V034_DMA_CH->CFGR |= DMA_CFGR1_EN;
  //LED_Ctrl(LED1,RVS);
}

#else
// �ɻɹܺ�����ͷ��Ҫ�õ���
void EXTI4_IRQHandler(void)
{
  //���EXTI4��·����λ//
  EXTI_ClearFlag(EXTI_Line4); //����ͷ���ж���B4

  hang= 0;
  hangcnt= 0;
  Camera_Flag = 0;
  //LED_Ctrl(LED1,RVS);
}
#endif
void EXTI3_IRQHandler(void)
{    //���EXTI3��·����λ//
  EXTI_ClearFlag(EXTI_Line3);    //����ͷ����C3
  if(hang%2==0)
  {
    for(lie=0;lie<LCDW;lie++)
    {
      Image_Data[hangcnt][lie]=(u8)((GPIOC->INDR)>>8);
    }
    if(++hangcnt>LCDH)
    {
      hangcnt=LCDH;
      Camera_Flag=1;
    }
  }
  ++hang;
  LED_Ctrl(LED1,RVS);
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
  { /*���EXTI12��·����λ*/
    EXTI_ClearITPendingBit(EXTI_Line5);
    //LED_Ctrl(LED1,RVS);
  }
}
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
  { /*���EXTI12��·����λ*/
    EXTI_ClearITPendingBit(EXTI_Line12);
    LED_Ctrl(LED1,RVS);
  }
}
/*************************************************************************
*  �������ƣ�void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
*  ����˵����GPIO��ʼ������
*  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���
*            dir     �� GPIO ģʽ   lq_gpio.h�еĺ궨����ѡ��
*            output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
*            u32 pinconf :�ܽ�����,�ο�ch32v10x_gpio.h�е�ö��GPIO_Mode_AF_PP��
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ���� PIN_InitConfig(PA3, GPO, 1,GPIO_Mode_Out_PP);  //PA3��ʼ��������� �ߵ�ƽ
*************************************************************************/
void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode dir, unsigned char output,u32 pinconf)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    u8 port_index = pin >> 4;

    //GPIO����ʱ��ʹ��
         if(0 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(1 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(2 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(3 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(4 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(5 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(6 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    //���ùܽ�����
    GPIO_InitStructure.GPIO_Pin = (u16)(1 << (pin & 0x0F));

    if(GPO == dir)
    {  //����GPIO�ٶ�
        if((u16)pinconf >> 8 == 0) GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)(GPIO_Speed_50MHz);  //��ֹû����д�������,Ĭ������50Mhz
        else GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)((u16)pinconf >> 8);                       //�Ѿ���д�˸ò���������ֵ������
    }

    //����ģʽ �����롢��������������á����ա�AD��
    GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)((u16)pinconf & 0xff);
    GPIO_Init((GPIO_TypeDef *)(PORTX[port_index]), &GPIO_InitStructure);

    if(GPO == dir)//��������ߵ͵�ƽ���ߵ�ƽĬ�ϣ���Ϊ����ڲ�����
    {
        GPIO_WriteBit((GPIO_TypeDef *)(PORTX[port_index]), (u16)(1 << (pin & 0x0F)), output);
    }
}

/*************************************************************************
*  �������ƣ�unsigned char PIN_Read(GPIO_Name_t pin)
*  ����˵������ȡGPIO��ƽ����
*  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���*
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ����uint8_t pinState = PIN_Read(P00_0);  //��ȡP00_0 �ĵ�ƽ״̬
*************************************************************************/
unsigned char PIN_Read(GPIO_Name_t pin)
{
    if((((GPIO_TypeDef *)(PORTX[pin>>4]))->INDR & (1<<(pin & 0x0F))) != 0)
        {
            return 1;
        }
        return 0;
}

/*************************************************************************
*  �������ƣ�void PIN_Write(GPIO_Name_t pin, unsigned char output)
*  ����˵��������GPIO���״̬
*  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���*
*            output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ����PIN_Write(P00_0, 1);//P00_0��Ϊ�ߵ�ƽ
*************************************************************************/
void PIN_Write(GPIO_Name_t pin, unsigned char output)
{

    if (output == 1)
             ((GPIO_TypeDef *)(PORTX[pin>>4]))->BSHR = (1<<(pin & 0x0F));
        else
             ((GPIO_TypeDef *)(PORTX[pin>>4]))->BCR = (1<<(pin & 0x0F));
}

/*************************************************************************
*  �������ƣ�void PIN_Reverse(GPIO_Name_t pin)
*  ����˵����GPIO��ƽ��ת������ʹ��ǰ���ȳ�ʼ��
*  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���*
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ����PIN_Reverse(P00_0); //��תP00_0�ܽŵĵ�ƽ״̬
*************************************************************************/
void PIN_Reverse(GPIO_Name_t pin)
{
    ((GPIO_TypeDef *)(PORTX[pin>>4]))->OUTDR ^= (u16)(1<<(pin & 0x0F));
}

/*************************************************************************
*  �������ƣ�void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf)
*  ����˵����GPIO�ⲿ�жϳ�ʼ��
*  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���
*            dir     �� GPIO ģʽ   lq_gpio.h�еĺ궨����ѡ��*
*            u32 pinconf :�ܽ�����,�ο�ch32v10x_gpio.h�е�ö��GPIO_Mode_AF_PP��
*            u8 prior �ж����ȼ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ����PIN_Exti(PA12, EXTI_Trigger_Falling,GPIO_Mode_IPU);//����PA12�½��ش����ж�
*  ��    ע��ch32v103ֻ�м������̶���GPIO֧���ⲿ�ж�
*************************************************************************/
void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf,u8 prior)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    PIN_InitConfig(pin, GPI, 0, pinconf);
    GPIO_EXTILineConfig(pin>>4, pin & 0x0F);  //�ܽ��жϺ�����
    EXTI_InitStructure.EXTI_Line = 1<<(pin & 0x0F);         //�ܽź�����
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //����ģʽ
    EXTI_InitStructure.EXTI_Trigger = edge;                 //������ʽ
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //ʹ�ܻ���ʧ��
    EXTI_Init(&EXTI_InitStructure);

    if(pin & 0x0F < 5)
    {        //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn + pin & 0x0F;
    }
    else if(pin & 0x0F < 10)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    }
    else
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    }
    //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prior;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //ʹ���ⲿ�ж�ͨ��//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���//
    NVIC_Init(&NVIC_InitStructure);

}
