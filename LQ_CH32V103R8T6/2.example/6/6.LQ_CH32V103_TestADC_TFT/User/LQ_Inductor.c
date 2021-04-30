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
ʹ��˵����
����ѧ��ʾ���������ڵ�����ֻ������ֳ���
������ԴΪ��
ģ�飺����TC264DA���İ塢����ĸ�塢˫·ȫ�ŵ��������˫������512������TFT1.8��Ļ�����������·���ģ�飻
��ģ�����ֻ������־��ɣ�
��зֲ���
||----------��------------------------------------��--------------||
||---------��--------------------------------------��-------------||
||--------��----------------------------------------��------------||
||-------1----����2����� -------------�Ҳ��2����� -----1-----------||
||------��--------------------------------------------��----------||
||-----��----------------------------------------------��---------||
||----��------------------------------------------------��--------||
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

int16_t TempAngle_F = 0;        // ���ݵ��ƫ�������������ʱ���ֵ
int16_t TempAngle_B = 0;        // ���ݵ��ƫ�������������ʱ���ֵ
int16_t LastAngle = 0;        // ����������ǰ����Чƫ�Ʒ���
int16_t TempAngle_L = 0;        // ���ݵ��ƫ�������������ʱ���ֵ
int16_t TempAngle_R = 0;        // ���ݵ��ƫ�������������ʱ���ֵ

int16_t BackOffset = 0;
int16_t FrontOffset = 0;      // ���ݵ��ƫ�������������ʱ���ֵ

int16_t MotorDuty_FL=0;
int16_t MotorDuty_FR=0;
int16_t MotorDuty_BL=0;
int16_t MotorDuty_BR=0;

 //Front Back Vertical Parallel
int16_t LleftFP = 0, LleftFV = 0, LrightFV = 0, LrightFP = 0;  // ���ƫ����
int16_t LleftBP = 0, LleftBV = 0, LrightBV = 0, LrightBP = 0;  // ���ƫ����
int16_t LnowADC[8];           // ��е�ǰADC��ֵ

int16_t ad_max[8] = {2500, 2300, 999, 999, 2500, 2500, 2500, 2500}; // �°��ӷŵ������궨���ֵ,�ᱻ����ˢ��
int16_t ad_min[8] = { 120,  120, 120, 120,  120,  120,  120,  120}; // �°��Ӿ���Ҫ�궨��Сֵ,�ᱻ����ˢ��

u8 CircleNumber = 1;    // �뻷������0������Ĭ��1�� ;���ĸ���һ���ڱ���ǰ��������ʱ��֪����
u8 TangentPoint = 1;    // �е��ж�   0�е������Ĭ��1�����뻷����ȡ����Ϊ�뻷׼��
u8 EnterCircle = 0;     // �������  Ĭ�� 0���ɽ�����1���Խ���
u8 OutCircle = 0;       // �������   Ĭ��0���ɳ�����1���Գ���
u8 LeftRightCircle = 0; // ��໷�����Ҳ໷ Ĭ��0ԭʼ��1�󻷣�2�һ�

u8 Left_fork=0;
u8 Right_fork=0;
int32_t TangentPointpulse = 0; // ���������ʱ����1
int32_t EnterCirclePulse = 0;  // ���������ʱ����2
int32_t OutCirclePulse = 0;    // ���������ʱ����3
int32_t EnterCircleOKPulse = 0;// ���������ʱ����4
int16_t LowSpeed = 0;          // Բ��/ʮ�ֿڼ���

u16 MagneticField = 0;     // �ų�ǿ��    magnetic field intensity,�ж�Բ���Ƿ����
int16_t OffsetDelta = 0;
extern pid_param_t pid1;
extern int16_t ECPULSE1, ECPULSE2,ECPULSE3, ECPULSE4;

/*************************************************************************
*  �������ƣ�void InductorInit (void)
*  ����˵�����ĸ����ADC��ʼ��������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��11��18��
*  ��    ע��
*************************************************************************/
void InductorInit (void)
{
  ADCInit(ADC_Channel_0);
  ADCInit(ADC_Channel_1);
  ADCInit(ADC_Channel_4);
  ADCInit(ADC_Channel_5);
  ADCInit(ADC_Channel_6);
  ADCInit(ADC_Channel_7);
  ADCInit(ADC_Channel_8);
  ADCInit(ADC_Channel_9);
  ADCInit(ADC_Channel_10);  
}
/*************************************************************************
*  �������ƣ�void InductorNormal(void)
*  ����˵�����ɼ���е�ѹ����һ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��11��18��
*  ��    ע��   ע��Ҫ�ȱ궨�˷ŵķŴ����������ĸ�һ�»��߽ӽ�
*************************************************************************/
void InductorNormal (void)
{
  LnowADC[0] = ADC_Read(0);  // ��ǰ���1����У�������ƽ�У��ɼ�������е�ADֵ
  LnowADC[1] = ADC_Read(1);  // ��ǰ���2����У��봹ֱ������
  LnowADC[2] = ADC_Read(4);  // ��ǰ���2����У��봹ֱ������
  LnowADC[3] = ADC_Read(5);  // ��ǰ���1����У�������ƽ�У�
  LnowADC[4] = ADC_Read(6);  // �����1����У�������ƽ�У�
  LnowADC[5] = ADC_Read(7);  // �����2����У���������ֱ
  LnowADC[6] = ADC_Read(8);  // �Һ���2����У���������ֱ��
  LnowADC[7] = ADC_Read(9);  // �Һ���1����У�������ƽ��
  BatVolt    = ADC_Read(10);  // ˢ�µ�ص�ѹ

  if (LnowADC[0] < ad_min[0])        ad_min[0] = LnowADC[0];     // ˢ����Сֵ
  else if (LnowADC[0] > ad_max[0])   ad_max[0] = LnowADC[0];     // ˢ�����ֵ

  if (LnowADC[1] < ad_min[1])        ad_min[1] = LnowADC[1];
  else if (LnowADC[1] > ad_max[1])   ad_max[1] = LnowADC[1];

  if (LnowADC[2] < ad_min[2])        ad_min[2] = LnowADC[2];
  else if (LnowADC[2] > ad_max[2])   ad_max[2] = LnowADC[2];

  if (LnowADC[3] < ad_min[3])        ad_min[3] = LnowADC[3];
  else if (LnowADC[3] > ad_max[3])   ad_max[3] = LnowADC[3];

  if (LnowADC[4] < ad_min[4])        ad_min[4] = LnowADC[4];
  else if (LnowADC[4] > ad_max[4])   ad_max[4] = LnowADC[4];

  if (LnowADC[5] < ad_min[5])        ad_min[5] = LnowADC[5];
  else if (LnowADC[5] > ad_max[5])   ad_max[5] = LnowADC[5];

  if (LnowADC[6] < ad_min[6])        ad_min[6] = LnowADC[6];
  else if (LnowADC[6] > ad_max[6])   ad_max[6] = LnowADC[6];

  if (LnowADC[7] < ad_min[7])        ad_min[7] = LnowADC[7];
  else if (LnowADC[7] > ad_max[7])   ad_max[7] = LnowADC[7];

  LleftFP  = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);     // ��ƫ������һ����0--100����
  LleftFV  = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
  LrightFV = (LnowADC[2] - ad_min[2]) * 100 / (ad_max[2] - ad_min[2]);
  LrightFP = (LnowADC[3] - ad_min[3]) * 100 / (ad_max[3] - ad_min[3]);
  
  LleftBP  = (LnowADC[4] - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);
  LleftBV  = (LnowADC[5] - ad_min[5]) * 100 / (ad_max[5] - ad_min[5]);
  LrightBV = (LnowADC[6] - ad_min[6]) * 100 / (ad_max[6] - ad_min[6]);
  LrightBP = (LnowADC[7] - ad_min[7]) * 100 / (ad_max[7] - ad_min[7]);
  
  MagneticField = LleftFP + LleftFV + LrightFV + LrightFP+LleftBP + LleftBV + LrightBV + LrightBP;// �ų�����ǿ��

  ECPULSE1 = -Read_Encoder(2)/10; //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
  ECPULSE2 = -Read_Encoder(4)/10; //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ


  if((LleftBP+LleftBV>LleftFP+LleftFV)&&(LleftFV<5||LrightFV<5)&&(LleftFV>LrightFV))
  {
     // if(LeftRightCircle==0)
      //{
          MotorDuty_FR=MtTargetDuty/2;
          MotorDuty_FL=-MtTargetDuty/2;
          MotorDuty_BL=-MtTargetDuty/2;
          MotorDuty_BR=MtTargetDuty/2;
     // }
  }
  else if((LrightBV+LrightBP>LrightFV+LrightFP)&&(LleftFV<5||LrightFV<5)&&(LleftFV<LrightFV))
  {
     // if(LeftRightCircle==0)
      //{
          MotorDuty_FR=-MtTargetDuty/2;
          MotorDuty_FL=MtTargetDuty/2;
          MotorDuty_BL=MtTargetDuty/2;
          MotorDuty_BR=-MtTargetDuty/2;
   //   }
  }
  else if((LeftRightCircle==0)|| EnterCircle==1)
  {
          if((LleftFV < 30)&&(LrightFV < 30))
            {
                 TempAngle_F=(LleftFV-LrightFV)*4+(LleftFV-LrightFV)/5; //ǰ�ֲ���
                 TempAngle_B=(LleftFV-LrightFV)*4+(LleftFV-LrightFV)/6; //���ֲ���
                 MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1/12;  //ǰ�ҵ��
                 MotorDuty_FL=MtTargetDuty-TempAngle_F*7/5-ECPULSE2/12;  //ǰ����
                 MotorDuty_BL=MtTargetDuty-TempAngle_B*7/5-ECPULSE3/12;  //������
                 MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4/12;  //���ҵ��
            }
         else
            {
                 LowSpeed=800;
                 TempAngle_F=(LleftFV-LrightFV)*11+(LleftFV-LrightFV)/4; //ǰ�ֲ���
                 TempAngle_B=(LleftFV-LrightFV)*11+(LleftFV-LrightFV)/5; //���ֲ���
                 MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed;  //ǰ�ҵ��
                 MotorDuty_FL=MtTargetDuty-TempAngle_F*7/5-ECPULSE2 /10-LowSpeed;  //ǰ����
                 MotorDuty_BL=MtTargetDuty-TempAngle_B*7/5-ECPULSE3 /10-LowSpeed;  //������
                 MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed;  //���ҵ��
            }


  }

}

/*************************************************************************
 *  �������ƣ�void CircleDetect (void)
 *  ����˵����ʶ�𲢽���Բ���ĸ�����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void CircleDetect (void)
{

}

/*************************************************************************
*  �������ƣ�void TFT_Show_EleMag_Info(void)
*  ����˵������ʾ����������Ϣ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��11��18��
*  ��    ע��
*************************************************************************/
void TFT_Show_EleMag_Info(void)
{
  char txt[16] = "X:";
  uint16_t bat=0;
  // ������Ϣ
  sprintf(txt, "%04d %04d %03d ", FrontOffset, BackOffset,(int16_t)(pid1.out)); // ��ʾ����Ƕ���ֵ�����ռ�ձ���ֵ����������ֵ
  TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);      // �ַ�����ʾ
  //��ʾ����й�һ�����ƫ����  ��ǰ����е�ѹֵ ����п�������ʷ��Сֵ ����п�������ʷ���ֵ
  sprintf(txt, "%04d %04d %05d ", LleftFP, LleftBP,ECPULSE1);
  TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LleftFV, LleftBV,ECPULSE2);
  TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFV, LrightBV,ECPULSE3);
  TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFP, LrightBP,ECPULSE4);
  TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
  
  sprintf(txt, "Ring num: %d ", CircleNumber);
  TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
  sprintf(txt, "M:%03d Q:%d J:%d ", MagneticField, TangentPoint, EnterCircle);
  TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
  if (LeftRightCircle == 1)
    TFTSPI_P8X16Str(0, 8, "Left Ring ", u16WHITE, u16BLACK);
  else if (LeftRightCircle == 2)
    TFTSPI_P8X16Str(0, 8, "Right Ring", u16WHITE, u16BLACK);
  else
    TFTSPI_P8X16Str(0, 8, "No Ring   ", u16WHITE, u16BLACK);
  
  bat = BatVolt * 11 / 25;  // x/4095*3.3*100*5.7
  sprintf(txt, "B:%d.%02dV ", bat / 100, bat % 100);  // *3.3/4095*3
  TFTSPI_P8X16Str(0, 9, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
}
/*************************************************************************
*  �������ƣ�void ElectroMagneticCar(void)
*  ����˵������ų�˫������ٿ���
-->1.�����㷨���򵥵ķֶα��������㷨����ѧ��ʾ�����㷨��
---2.�����㷨��PID����Ӧ�ÿ����㷨����ѧ��ʾ�����㷨��
---3.�߶��㷨���ݶ��Ľ�����ȺЭͬ�����㷨��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��10��28��
*  ��    ע������2�����
*************************************************************************/
void ElectroMagneticMWCar (void)
{

  InductorInit();       // �����ADC��ʼ��
  EncInit();            // ��ʼ������������
  MotorInit(12500);     // ���������PWMΪ12.5Khz
  TIMER_InitConfig(TIMER_1, 10);// ��ȡPID��ʱ�ж�10ms
  
  TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLACK);            // ����

  TFTSPI_P8X16Str(0,2,"PRESS K2 to show...",u16WHITE,u16BLACK);              //�ַ�����ʾ
  while (1)
  {
    InductorNormal();           // �ɼ���е�ѹ������һ����

    if (MagneticField < 20)     // �ж��Ƿ�������
      {
        MotorCtrl4w(0, 0, 0, 0);
      }
    else
    {
        MotorCtrl4w(MotorDuty_FR,MotorDuty_FL,MotorDuty_BL,MotorDuty_BR);
    }


   if (KEY_Read (KEY2)==0)
    {// ������ʾ������Ϣ
           TFT_Show_EleMag_Info();// ��������Ϣ
   } // key

  } // WHILE(1)
} // MAIN()

