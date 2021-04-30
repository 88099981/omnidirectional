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
*  �������ƣ�void Reed_Init(void)
*  ����˵�����ɻɹ�GPIO���жϳ�ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��11��21��
*  ��    ע��  GPIOA12
*          
*************************************************************************/
void Reed_Init(void)
{
    /*
	//��ʼ��,�׿��
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);// ����GPIOA��ȫ����ʱ��
    //PA12
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    //���ó���������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    //��ʼ��GPIOA12
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //GPIOA12 �ж����Լ��жϳ�ʼ������
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    //�½��ش���
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    EXTI_Init(&EXTI_InitStructure);						                        
    
    //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    //ʹ���ⲿ�ж�ͨ��//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���//
    NVIC_Init(&NVIC_InitStructure);
    */
    //��װ��
    PIN_Exti(PA12, EXTI_Trigger_Falling,GPIO_Mode_IPU,3);
}

/*************************************************************************
*  �������ƣ�void Test_GPIO_LED(void)
*  ����˵�������Գ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע�����İ��ϵ�LED����˸--�ĸ�LEDͬʱ��˸
*************************************************************************/
void Test_GPIO_EXIT_Init(void)
 {
	GPIO_LED_Init();
	Reed_Init();
 	while(1)
 	{
 		LED_Ctrl(LED0,RVS);        //LEDͬʱ��˸
 		delayms(200);                //��ʱ�ȴ�
 	}
 }
