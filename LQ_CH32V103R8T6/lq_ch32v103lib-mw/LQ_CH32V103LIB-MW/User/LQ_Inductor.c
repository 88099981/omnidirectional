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
u8 Normal=0;

int32_t TangentPointpulse = 0; // ���������ʱ����1
int32_t EnterCirclePulse = 0;  // ���������ʱ����2
int32_t OutCirclePulse = 0;    // ���������ʱ����3
int32_t EnterCircleOKPulse = 0;// ���������ʱ����4
int16_t LowSpeed = 0;          // Բ��/ʮ�ֿڼ���

u16 MagneticField = 0;     // �ų�ǿ��    magnetic field intensity,�ж�Բ���Ƿ����
u16 MagneticField_F=0;     // ǰ�ĸ���дų���ǿ��
u16 MagneticField_B=0;     // �ĸ���дų���ǿ��
u16 run=1;
int16_t OffsetDelta = 0;
extern pid_param_t pid1;
extern int16_t ECPULSE1, ECPULSE2,ECPULSE3, ECPULSE4;
int ting=0;
//����Բ����־λ
int16_t Enter_leftcircle_flag=0,Enter_leftcircle=0;
int16_t Out_leftcircle_flag=0,Out_leftcircle=0;
int16_t Enter_Rightcircle_flag=0,Enter_Rightcircle=0;
int16_t Out_Rightcircle_flag=0,Out_Rightcircle=0;
int16_t Right_Noin=0,left_Noin=0;
int16_t left_circle=0,Right_circle=0;
int16_t circleing=0;
//��������·�ڱ�־λ
int16_t Enter_leftfork_flag=0,Enter_rightfork_flag=0;
int16_t Enter_leftfork=0,Enter_rightfork=0;
int16_t Out_leftfork_flag=0,Out_rightfork_flag=0;
int16_t Out_leftfork=0,Out_rightfork=0;
int forking=0;
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
  LnowADC[1] = ADC_Read(1);  // ��ǰ���2����У��봹ֱ�������ɼ�������е�ADֵ
  LnowADC[2] = ADC_Read(4);  // ��ǰ���2����У��봹ֱ�������ɼ�������е�ADֵ
  LnowADC[3] = ADC_Read(5);  // ��ǰ���1����У�������ƽ�У��ɼ�������е�ADֵ
  LnowADC[4] = ADC_Read(6);  // �����1����У�������ƽ�У��ɼ�������е�ADֵ
  LnowADC[5] = ADC_Read(7);  // �����2����У���������ֱ���ɼ�������е�ADֵ
  LnowADC[6] = ADC_Read(8);  // �Һ���2����У���������ֱ���ɼ�������е�ADֵ
  LnowADC[7] = ADC_Read(9);  // �Һ���1����У�������ƽ�У��ɼ�������е�ADֵ
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
  MagneticField_F=LleftFP + LleftFV + LrightFV + LrightFP; // ǰ�ĸ���дų���ǿ��
  MagneticField_B=LleftBP + LleftBV + LrightBV + LrightBP; // ���ĸ���дų���ǿ��
  ECPULSE1 = -Read_Encoder(2)/10; //�ҵ�� ĸ���ϱ�����2
  ECPULSE2 = -Read_Encoder(4)/10; //���� ĸ���ϱ�����1
}

/*************************************************************************
 *  �������ƣ�void determine (void)
 *  ����˵����ʶ��Ԫ�ر�־λ
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��ʶ�����Ԫ�ر�־λ
 *************************************************************************/

void determine (void)
{
    if (MagneticField < 20)     // �ж��Ƿ�������
     {
       ting=1;
       run=0;
       Normal=0;
     }

    //������ʻ����������·��
    if(forking==0&&MagneticField_F<40&&MagneticField_B>80&&Out_leftfork_flag==0&&Enter_leftfork_flag==0)
    {
        Normal=0;
        run=0;
        ting=0;
        Enter_leftfork_flag=1;
     }
    if( Enter_leftfork_flag==1&&MagneticField_F>MagneticField_B&&120<MagneticField<160)
            {
                Enter_leftfork_flag=0;
                Enter_leftfork=1;
                run=0;
                Normal=0;
                ting=0;
            }
    //������·��
    if(Enter_leftfork==1&&MagneticField>160&&MagneticField_B>MagneticField_F)
    {
        run=0;
        Enter_leftfork=0;
        ting=0;
        Out_leftfork_flag=1;
    }
   if(Out_leftfork_flag==1)
    {
        Normal=1;
        run=0;
        ting=0;
        Out_leftfork_flag=0;
    }
 }
/*************************************************************************
 *  �������ƣ�void control (void)
 *  ����˵��������С������
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��ͨ��ʶ�𵽵ı�־λ����С��
 *************************************************************************/
void control (void)
{
    if(ting == 1)               //�������ͣ��
     {
       MotorCtrl4w(0, 0, 0, 0);
       delayms(200);
     }
    //���·����
    else if(Enter_leftfork_flag==1)                                 //ʶ���·�ڣ����Ž������·
     {
        // ���ݵ���������е����ĸ����ٶ�
        MotorDuty_FR=3000;   //ǰ�ҵ��
        MotorDuty_FL=-3000;  //ǰ����
        MotorDuty_BL=2800;   //������
        MotorDuty_BR=-2800;  //���ҵ��
     }
    else if(Enter_leftfork==1&&Out_leftfork_flag==0)                //�����·�ڣ��ı���̬��
    {

      if((MagneticField_B-MagneticField_F)>50)
       {
         MotorDuty_FR=-2400;
         MotorDuty_FL=-2400;
         MotorDuty_BL=-2400;
         MotorDuty_BR=-2400;

       }
      else if((MagneticField_F-MagneticField_B)>50)
       {
         MotorDuty_FR=2400;
         MotorDuty_FL=2400;
         MotorDuty_BL=2400;
         MotorDuty_BR=2400;
       }
      else
       {
         if(LleftFP>LleftBP)
          {
            TempAngle_F=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //ǰ�ֲ���
            TempAngle_B=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //���ֲ���
            MotorDuty_FR=  MtTargetDuty+TempAngle_F*8/5-ECPULSE1 /10+600;   //ǰ�ҵ��
            MotorDuty_FL=-(MtTargetDuty+TempAngle_B*8/5-ECPULSE2 /10+600);  //ǰ����
            MotorDuty_BL=  MtTargetDuty-TempAngle_B*7/5-ECPULSE3 /10+300;   //������
            MotorDuty_BR=-(MtTargetDuty-TempAngle_F*7/5-ECPULSE4 /10+300);  //���ҵ��
          }
         else if(LleftFP<LleftBP)
          {
            TempAngle_F=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //ǰ�ֲ���
            TempAngle_B=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //���ֲ���
            MotorDuty_FR=  MtTargetDuty+TempAngle_F*8/5-ECPULSE1 /10+600;   //ǰ�ҵ��
            MotorDuty_FL=-(MtTargetDuty+TempAngle_B*8/5-ECPULSE2 /10+600);  //ǰ����
            MotorDuty_BL=  MtTargetDuty-TempAngle_B*7/5-ECPULSE3 /10+300;   //������
            MotorDuty_BR=-(MtTargetDuty-TempAngle_F*7/5-ECPULSE4 /10+300);  //���ҵ��
          }
       }
    }
    else if(run==1)                                        //������ʻ
     {
        Normal=0;
        if((LleftFV-LrightFV)>50)
        {
            MotorDuty_FR= 2200;
            MotorDuty_FL=-2200;
            MotorDuty_BL=-2200;
            MotorDuty_BR= 2200;
            LastAngle=1;

        }
        else if((LrightFV-LleftFV)>50)
        {
            MotorDuty_FR=-2200;
            MotorDuty_FL= 2200;
            MotorDuty_BL= 2200;
            MotorDuty_BR=-2200;

        }
        else if((LleftFV < 30)&&(LrightFV < 30))
        {
          LowSpeed=0;
          TempAngle_F=(LleftFV-LrightFV)*6+(LleftFV-LrightFV)/6; //ǰ�ֲ���
          TempAngle_B=(LleftFV-LrightFV)*6+(LleftFV-LrightFV)/6; //���ֲ���
          MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed;  //ǰ�ҵ��
          MotorDuty_FL=MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed;  //ǰ����
          MotorDuty_BL=MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed;  //������
          MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed;  //���ҵ��
        }
       else
        {
          LowSpeed=800;
          TempAngle_F=(LleftFV-LrightFV)*10+(LleftFV-LrightFV)/7; //ǰ�ֲ���
          TempAngle_B=(LleftFV-LrightFV)*10+(LleftFV-LrightFV)/7; //���ֲ���
          MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed;  //ǰ�ҵ��
          MotorDuty_FL=MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed;  //ǰ����
          MotorDuty_BL=MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed;  //������
          MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed;  //���ҵ��
       }
    }

    else if(Normal==1)                                     //������ʻ
         {
            run=0;
            Enter_leftfork=0;
            if((LleftBV-LrightBV)>50)
            {
                MotorDuty_FR= 2200;
                MotorDuty_FL=-2200;
                MotorDuty_BL= 2200;
                MotorDuty_BR=-2200;
                LastAngle=1;
            }
            else if((LrightBV-LleftBV)>50)
            {
                MotorDuty_FR=-2200;
                MotorDuty_FL= 2200;
                MotorDuty_BL=-2200;
                MotorDuty_BR= 2200;
            }
            if((LleftBV < 30)&&(LrightBV < 30))
            {
              LowSpeed=0;
              TempAngle_F=(LleftBV-LrightBV)*6+(LleftBV-LrightBV)/6; //ǰ�ֲ���
              TempAngle_B=(LleftBV-LrightBV)*6+(LleftBV-LrightBV)/6; //���ֲ���
              MotorDuty_FR=-(MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed);  //ǰ�ҵ��
              MotorDuty_FL=-(MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed);  //ǰ����
              MotorDuty_BL=-(MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed);  //������
              MotorDuty_BR=-(MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed);  //���ҵ��
            }
           else
            {
              LowSpeed=800;
              TempAngle_F=(LleftBV-LrightBV)*10+(LleftBV-LrightBV)/7; //ǰ�ֲ���
              TempAngle_B=(LleftBV-LrightBV)*10+(LleftBV-LrightBV)/7; //���ֲ���
              MotorDuty_FR=-(MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed);  //ǰ�ҵ��
              MotorDuty_FL=-(MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed);  //ǰ����
              MotorDuty_BL=-(MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed);  //������
              MotorDuty_BR=-(MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed);  //���ҵ��
           }
        }
    else
    {
     ting=0;
    }

}
/*************************************************************************
*  �������ƣ�void Bluetooth(void)
*  ����˵����������λ����ʾ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��10��28��
*  ��    ע��ͨ��������λ����ӡ������״̬
*************************************************************************/
void Bluetooth(void)
{
    if(ting == 1)
    {
      UART_PutStr(USART1,"ͣ�� \r\n ");      //���ͱ�־����λ��
      delayms(1000);
    }
    else if(run==1)
    {
      UART_PutStr(USART1,"���� \r\n ");      //���ͱ�־����λ��
    }
    else if(Normal==1)
    {
      UART_PutStr(USART1,"���� \r\n ");      //���ͱ�־����λ��
    }
    else if(Enter_leftfork_flag==1)
    {
        UART_PutStr(USART1,"������· \r\n ");      //���ͱ�־����λ��
    }
    else if(Enter_leftfork==1)
    {
        UART_PutStr(USART1,"�������· \r\n ");      //���ͱ�־����λ��
    }
    else if(Out_leftfork_flag==1)
    {
        UART_PutStr(USART1,"�жϳ���· \r\n ");      //���ͱ�־����λ��
    }
    else if(Out_leftfork==1)
    {
        UART_PutStr(USART1,"����· \r\n ");      //���ͱ�־����λ��
    }
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
  // ������Ϣ
  sprintf(txt, "%04d %04d %05d ", LleftFP, LleftBP,ECPULSE1);          //��ʾ��1ǰ���ֵ����1����ֵ��������1ֵ����ǰ��
  TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LleftFV, LleftBV,ECPULSE2);          //��ʾ��2ǰ���ֵ����2����ֵ��������1ֵ����ǰ��
  TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFV, LrightBV,ECPULSE3);        //��ʾ��2ǰ���ֵ����2����ֵ��������1ֵ�����
  TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFP, LrightBP,ECPULSE4);        //��ʾ��1ǰ���ֵ����1����ֵ��������1ֵ���Һ�
  TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
  
  sprintf(txt, "M1: %d ", MagneticField_F);                            //��ʾǰ4�������ֵ
  TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
  sprintf(txt, "M:%03d M2:%d", MagneticField, MagneticField_B);        //��ʾǰ��8�������ֵ     ��ʾ��4�������ֵ
  TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
  if (run == 1)
    TFTSPI_P8X16Str(0, 8, "run ", u16WHITE, u16BLACK);
  else if (Normal == 1)
    TFTSPI_P8X16Str(0, 8, "Normal", u16WHITE, u16BLACK);
  else
    TFTSPI_P8X16Str(0, 8, "ting ", u16WHITE, u16BLACK);
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
  InductorInit();                                               // �����ADC��ʼ��
  EncInit();                                                    // ��ʼ������������
  TIMER_InitConfig(TIMER_1, 10);                                // ��ȡPID��ʱ�ж�10ms
  UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10);      //��ʼ������1 ������ 115200 ����żУ�� 1ֹͣλ ʹ�ùܽ�
  TFTSPI_Init(1);                                               //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLACK);                                         // ����
  TFTSPI_P8X16Str(0,2,"PRESS K2 to show...",u16WHITE,u16BLACK); //�ַ�����ʾ
  while (1)
  {
    InductorNormal();                                           // �ɼ���е�ѹ����һ����
    determine();                                                //ʶ��Ԫ�ر�־λ
    control();                                                  //ʶ��Ԫ�غ�ı���̬������С������
    Bluetooth();                                                //�������ݵ���λ��
    MotorCtrl4w(MotorDuty_FR,MotorDuty_FL,MotorDuty_BL,MotorDuty_BR);//��ֵ
    //TFT_Show_EleMag_Info();// ��������Ϣ
   if (KEY_Read (KEY2)==0)
    {// ������ʾ������Ϣ
      TFT_Show_EleMag_Info();// ��������Ϣ
    }
  }
}

