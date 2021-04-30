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
=================================================================
 ________________________________________________________________
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  ��    ע��
 CH32V103R8T6��4��ͨ����TIM��1��2��3��4
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
extern int16_t ECPULSE3, ECPULSE4;
/*************************************************************************
*  �������ƣ�int Read_Encoder(u8 TIMX)
*  ����˵������ȡ����������
*  ����˵����u8 TIMX��ʱ��
*  �������أ���������
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��ECPULSE1 = Read_Encoder(1); //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
*************************************************************************/
int Read_Encoder(u8 TIMX)
{
  int Encoder_TIM;    
  switch(TIMX)
  {
  case 1:  Encoder_TIM= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;//Ĭ�϶�ʱ����
  case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;//Ĭ�ϱ�������
  case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;//Ĭ�ϵ����
  case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;//Ĭ�ϱ�������
  default:  Encoder_TIM=0;
  }
  return Encoder_TIM;
}

/*************************************************************************
*  �������ƣ�void EncInit (void)
*  ����˵������������ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2��������
*************************************************************************/
void EncInit (void)
{    
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                            //ʹ��AFIO���ù���ģ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE );

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB6,7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���룬����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB6,7
  GPIO_Init( GPIOB, &GPIO_InitStructure);

  // GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
  //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE );
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //Ԥ��Ƶ��
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM���ϼ���
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //�ظ�������
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);       //��ʼ���ṹ��
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI2 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ
  TIM_Cmd(TIM4, ENABLE);                                //��ʱ��ʹ��

  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PA15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���룬����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //PB3
  GPIO_Init( GPIOB, &GPIO_InitStructure);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //Ԥ��Ƶ��
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //�趨�������Զ���װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //ѡ��ʱ�ӷ�Ƶ������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM���ϼ���
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //�ظ�������
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       //��ʼ���ṹ��
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI2 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ
  TIM_Cmd(TIM2, ENABLE);                                //��ʱ��ʹ��
}

/*************************************************************************
*  �������ƣ�void TestEncoder(void)
*  ����˵�������Գ���OLED��ʾ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��10��
*  ��    ע��
*************************************************************************/
void TestEncoderOLED(void)
{
  char txt[32];
  OLED_Init();
  OLED_CLS();   //����
  OLED_P6x8Str(0, 0, "Test Encoder");      //�ַ�����ʾ
  EncInit();
  while (1)
  {
    /* ��ȡ������ֵ */
    ECPULSE1 = Read_Encoder(4); //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    ECPULSE3 = Read_Encoder(2); //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
    
    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    OLED_P6x8Str(0, 2, txt);       //�ַ�����ʾ
    sprintf(txt, "Enc2: %05d;", ECPULSE3);
    OLED_P6x8Str(0, 3, txt);       //�ַ�����ʾ
    

    //LED_Ctrl(LEDALL,RVS); //LED��ת��˸
    //LED_Ctrl(LED0, RVS);        //��ƽ��ת,LED��˸
    //delayms(200);              //��ʱ�ȴ�
  }
}
/*************************************************************************
*  �������ƣ�void TestEncoder(void)
*  ����˵�������Գ���TFT1.8��ʾ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��10��
*  ��    ע��
*************************************************************************/
void TestEncoderTFT(void)
{
  char txt[32];
  
  TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
  TFTSPI_P8X16Str(3, 2,"LQ encoder", u16PURPLE, u16BLACK);   // �ַ�����ʾ
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
  EncInit();
  MotorInit(12500);
  MotorCtrl4w(1200,1200,1200,1200);
  while (1)
  {
    /* ��ȡ������ֵ */

    ECPULSE1 = -Read_Encoder(2)/1; //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
    ECPULSE2 = -Read_Encoder(4)/1; //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    
    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(0, 2, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    sprintf(txt, "Enc2: %05d;", ECPULSE2);
    TFTSPI_P8X16Str(0, 3, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    
    sprintf(txt, "Enc3: %05d;", ECPULSE3);
    TFTSPI_P8X16Str(0, 4, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    sprintf(txt, "Enc4: %05d;", ECPULSE4);
    TFTSPI_P8X16Str(0, 5, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ

   // LED_Ctrl(LED0, RVS);        //��ƽ��ת,LED��˸
   // delayms(200);              //��ʱ�ȴ�
  }
}
