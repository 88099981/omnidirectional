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

const u32 TIMERx[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE};
int time=0;//���Զ�ʱ��
int16_t ECPULSE1 = 0;          // �ٶ�ȫ�ֱ���
int16_t ECPULSE2 = 0;          // �ٶ�ȫ�ֱ���
int16_t ECPULSE3 = 0;          // �ٶ�ȫ�ֱ���
int16_t ECPULSE4 = 0;          // �ٶ�ȫ�ֱ���
volatile int32_t RAllPulse = 0;// �ٶ�ȫ�ֱ���
extern volatile uint8_t Game_Over;    // С�����ȫ������ͣ��
extern volatile int16_t targetSpeed;
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//��ʱ��1�жϷ������
void TIM1_UP_IRQHandler(void)
{ 	
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
    //�û�����    
    LED_Ctrl(LED1,RVS);//LED��ת��˸
    //encoder_get();
    //SpeedCtrl(0,90,0);
    /* ���ݽǶ���Ϣ  ����С��  */
      /*  if(Game_Over==0)
        {
          if(FrontOffset > 50)
          {
            SpeedCtrl(0, 0, targetSpeed*2/3);  // ˳ʱ��ת��
          }
          else if(FrontOffset < -50)
          {
            SpeedCtrl(0, 0, -targetSpeed*2/3); // ��ʱ��ת��
          }
          else
          {
            SpeedCtrl(0, targetSpeed*2, 0);    //ǰ������
            //SpeedCtrl((BackOffset+FrontOffset)/3, targetSpeed*2, (BackOffset-FrontOffset)/3);
          }

          //���²�����Ҫ�ϴ�ĵ��������Ҫ��������
          if(((LleftFP + LleftFV +LleftBP + LleftBV)-(LrightFV + LrightFP+ LrightBV + LrightBP))>100)
          {
          if(BackOffset-FrontOffset > 100)
          SpeedCtrl(0, 0, targetSpeed);  // ˳ʱ��ת��
                else if(BackOffset-FrontOffset < -100)
          SpeedCtrl(0, 0, -targetSpeed); // ��ʱ��ת��
        }
            else if((BackOffset+FrontOffset) > 100) // ����ƫ��̫��
          {
          if(BackOffset+FrontOffset > 100)
          SpeedCtrl(-targetSpeed, 0, 0);  // ����ƽ��
                else if(BackOffset+FrontOffset < -100)
          SpeedCtrl(targetSpeed, 0, 0); // ����ƽ��
        }
            else
          {
          SpeedCtrl(0, targetSpeed*2, 0); //
          //SpeedCtrl((BackOffset+FrontOffset)/3, targetSpeed*2, (BackOffset-FrontOffset)/3);
        }
        }*/

  }
}

/*************************************************************************
*  �������ƣ�void TIMER_InitConfig(TIMER_Name_t timern, u16 ms)
*  ����˵�������PWM��ʼ��
*  ����˵����
//  @param      timern      ��ʱ��ͨ��
//  @param      ms          ��ʱ����pch,PWMͨ������Ӧ�ĵĶ�ʱ�����ܽ�

*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��TIMER_InitConfig(TIMER_1, 5); ʹ�ö�ʱ��1��Ϊ5msһ�ε������ж�
*************************************************************************/
void TIMER_InitConfig(TIMER_Name_t timern, u16 ms)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

         if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //ʱ��ʹ��
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = ms<<1;                               //�����Զ���װ�ؼĴ�������
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/2000);           //����Ϊ2Khz����һ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //����ʱ��Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //�ظ�����������Ϊ0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERx[timern], &TIM_TimeBaseStructure);//��ʼ��TIMxʱ��

    TIM_ITConfig((TIM_TypeDef *)TIMERx[timern],TIM_IT_Update,ENABLE );      //ʹ��TIM�ж�

    //�ж����ȼ�����
         if(TIMER_1 == timern)   nvic_init(TIM1_UP_IRQn, 0, 2, ENABLE);
    else if(TIMER_2 == timern)   nvic_init(TIM2_IRQn, 0, 2, ENABLE);
    else if(TIMER_3 == timern)   nvic_init(TIM3_IRQn, 0, 2, ENABLE);
    else if(TIMER_4 == timern)   nvic_init(TIM4_IRQn, 0, 2, ENABLE);

    TIM_Cmd((TIM_TypeDef *)TIMERx[timern], ENABLE);  //ʹ��TIMx

}
