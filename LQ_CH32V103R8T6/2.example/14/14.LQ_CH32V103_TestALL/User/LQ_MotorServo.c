/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�SPIN27PS/CH32V103R8T6���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 �������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
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

uint16_t BatVolt;         // ��ص�ѹ�ɼ�
unsigned char Camera_Flag; // ����ʶ

volatile uint8_t Game_Over = 0; // С�����ȫ������ͣ��
int16_t ServoDuty = Servo_Center_Mid;
int16_t MotorDuty1 = 500;      // �������ռ�ձ���ֵ
int16_t MotorDuty2 = 500;      // �������ռ�ձ���ֵ
int32_t NowTime = 0;
uint16_t BatVolt = 0;           // ��ص�ѹ�ɼ�

// �ĸ����ӵ������ٶ�
signed short targetSpeed1 = 0, targetSpeed2 = 0, targetSpeed3 = 0, targetSpeed4 = 0;
// �ĸ����ӵ�ʵ���ٶȣ�����һ�����İ���Ҫ�ṩ��·�������ٶ�
signed short  realSpeed1 = 0, realSpeed2 = 0, realSpeed3 = 0, realSpeed4 = 0;
pid_param_t pid1, pid2, pid3, pid4;
// �����ٶ�
volatile int16_t targetSpeed = 50;
//���Ƶ��
#define MOTOR_FREQUENCY    PWM_DUTY_MAX

#define USE7843or7971   //USEDRV8701 ʹ������ͬ������ģ�飬ѡ���Ӧ�ĺ궨��
//#define USEDRV8701
/*************************************************************************
*  �������ƣ�void MotorInit(u16 freq)
*  ����˵�������PWM��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2�����  TIM8_PWM_Init(1000-1);  //1KHZ����
*************************************************************************/
void MotorInit(u16 freq)
{
  /*
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  */
  PIN_InitConfig(PC10, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PC11, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PC12, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PC13, GPO, 1,GPIO_Mode_Out_PP);

  PWM_InitConfig(PWM3_CH3_PC8,freq, 100);
  PWM_InitConfig(PWM3_CH4_PC9,freq, 100);
  PWM_InitConfig(PWM3_CH1_PC6,freq, 100);
  PWM_InitConfig(PWM3_CH2_PC7,freq, 100);

  PidInit(&pid1);
  PidInit(&pid2);
  PidInit(&pid3);
  PidInit(&pid4);
  /* PID ������Ҫ���е��� */
  pid1.kp = 150;
  pid1.ki = 5;

  pid2.kp = 150;
  pid2.ki = 5;

  pid3.kp = 150;
  pid3.ki = 5;

  pid4.kp = 150;
  pid4.ki = 5;
}

/*!
  * @brief    �ٶȿ��ƺ���
  *
  * @param    x   �� x�����ٶ�                          С�����ҷ�
  * @param    y   �� y�����ٶ�                          С����ǰ��
  * @param    z   �� ��ʱ�뷽���ٶ�
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      ��
  *
  * @date     2020/6/8
  */
void SpeedCtrl(signed short x, signed short y, signed short z)
{
    // ����С���ٶȴ�С�ͷ���  ����ÿ�����ӵ�����ת��

    targetSpeed1 =  -x + y - z;
    targetSpeed2 =   x + y + z;
    targetSpeed3 =  -x + y + z;
    targetSpeed4 =   x + y - z;

    // ��ȡ������ֵ
    realSpeed1 = Read_Encoder(4); //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    realSpeed2 = Read_Encoder(2); //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
    realSpeed3 = ECPULSE3; //����һ�����İ巢���ݹ���
    realSpeed4 = ECPULSE4; //����һ�����İ巢���ݹ���

    // PID����
    PidIncCtrl(&pid1, targetSpeed1 - realSpeed1);
    PidIncCtrl(&pid2, targetSpeed2 - realSpeed2);
    PidIncCtrl(&pid3, targetSpeed3 - realSpeed3);
    PidIncCtrl(&pid4, targetSpeed4 - realSpeed4);

    // �������
    MotorCtrl4w(pid1.out, pid2.out, pid3.out, pid4.out);

    // �������ݵ�������λ��������   �������PID����
    //  ANO_DT_send_int16(targetSpeed1, realSpeed1, (signed short)(pid1.out), 0, 0, 0, 0, 0);

}


/*************************************************************************
*  �������ƣ�void MotorCtrl(int16_t motor1, int16_t motor2)
*  ����˵�������ת�Ǻ���������С�����˷�Χ���ƣ���С
*  ����˵����
*    @param    motor1   �� ���1ռ�ձȣ���Χ��-10000��10000��
     @param    motor2   �� ���2ռ�ձȣ���Χ��-10000��10000��
     @param    motor3   �� ���3ռ�ձȣ���Χ��-10000��10000��
     @param    motor4   �� ���4ռ�ձȣ���Χ��-10000��10000��
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2���������ͨ����ֻ��Ҫһ���������
*************************************************************************/
#ifdef USE7843or7971
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4)
{
  //assert(motor1>TIM3_PWM_DUTY_MAX);
  //assert(motor2>TIM3_PWM_DUTY_MAX);
  //assert(motor3>TIM3_PWM_DUTY_MAX);
  //assert(motor4>TIM3_PWM_DUTY_MAX);
  
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM3_CH3_PC8,motor1);	     //C8
    GPIO_ResetBits(MTGPIO1p);    //C10
  }
  else
  {
    PWM_Set_Duty(PWM3_CH3_PC8,MOTOR_FREQUENCY+motor1);
    GPIO_SetBits(MTGPIO1p);
  }
  
  if (motor2 > 0)
  {
    PWM_Set_Duty(PWM3_CH1_PC6,motor2);	    //C6
    GPIO_ResetBits(MTGPIO2p);   //C11	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH1_PC6,MOTOR_FREQUENCY+motor2);
    GPIO_SetBits(MTGPIO2p);
  }
  
  if (motor3 > 0)
  {
    PWM_Set_Duty(PWM3_CH4_PC9,motor3);	  //C9
    GPIO_ResetBits(MTGPIO3p); //C12	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH4_PC9,MOTOR_FREQUENCY+motor3);
    GPIO_SetBits(MTGPIO3p);
  }
  
  if (motor4 > 0)
  {
    PWM_Set_Duty(PWM3_CH2_PC7,motor4);	 //C7
    GPIO_ResetBits(MTGPIO4p);//C13	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH2_PC7,MOTOR_FREQUENCY+motor4);
    GPIO_SetBits(MTGPIO4p);
  }
}
void MotorCtrl(int16_t motor1, int16_t motor2)
{
  //assert(motor1>TIM3_PWM_DUTY_MAX);
  //assert(motor2>TIM3_PWM_DUTY_MAX);
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM3_CH4_PC9,motor1);	  //C9
    GPIO_ResetBits(MTGPIO3p); //C12	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH4_PC9,MOTOR_FREQUENCY+motor1);
    GPIO_SetBits(MTGPIO3p);
  }
  
  if (motor2 > 0)
  {
    PWM_Set_Duty(PWM3_CH2_PC7,motor2);	 //C7
    GPIO_ResetBits(MTGPIO4p);//C13	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH2_PC7,MOTOR_FREQUENCY+motor2);
    GPIO_SetBits(MTGPIO4p);
  }
}
#else   //USEDRV8701    
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4)
{
  //assert(motor1>TIM3_PWM_DUTY_MAX);
  //assert(motor2>TIM3_PWM_DUTY_MAX);
  //assert(motor3>TIM3_PWM_DUTY_MAX);
  //assert(motor4>TIM3_PWM_DUTY_MAX);
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM3_CH3_PC8,motor1);	     //C8
    GPIO_ResetBits(MTGPIO1p);    //C10
  }
  else
  {
    PWM_Set_Duty(PWM3_CH3_PC8,0-motor1);
    GPIO_SetBits(MTGPIO1p);
  }
  
  if (motor2 > 0)
  {
    PWM_Set_Duty(PWM3_CH1_PC6,motor2);	    //C6
    GPIO_ResetBits(MTGPIO2p);   //C11	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH1_PC6,0-motor2);
    GPIO_SetBits(MTGPIO2p);
  }
  
  if (motor3 > 0)
  {
    PWM_Set_Duty(PWM3_CH4_PC9,motor3);	  //C9
    GPIO_ResetBits(MTGPIO3p); //C12	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH4_PC9,0-motor3);
    GPIO_SetBits(MTGPIO3p);
  }
  
  if (motor4 > 0)
  {
    PWM_Set_Duty(PWM3_CH2_PC7,motor4);	 //C7
    GPIO_ResetBits(MTGPIO4p);//C13	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH2_PC7,0-motor4);
    GPIO_SetBits(MTGPIO4p);
  }
}

void MotorCtrl(int16_t motor1, int16_t motor2)
{
  //assert(motor1>TIM3_PWM_DUTY_MAX);
  //assert(motor2>TIM3_PWM_DUTY_MAX);
  
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM3_CH4_PC9,motor1);	  //C9
    GPIO_ResetBits(MTGPIO3p); //C12	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH4_PC9,0-motor1);
    GPIO_SetBits(MTGPIO3p);
  }
  
  if (motor2 > 0)
  {
    PWM_Set_Duty(PWM3_CH2_PC7,motor2);	 //C7
    GPIO_ResetBits(MTGPIO4p);//C13	
  }
  else
  {
    PWM_Set_Duty(PWM3_CH2_PC7,0-motor2);
    GPIO_SetBits(MTGPIO4p);
  }
}
#endif
/*************************************************************************
*  �������ƣ�TestMotor(void)
*  ����˵�������Ա궨���PWM���Ƶ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2�����
��ע�����ע�⣬һ��Ҫ�Զ����ǽ�������
ʹ������ĸ��������̣�
1.��ʹ�����ñ�������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
2.�Ӻ�ĸ�嵽��������ź��߼���Դ�ߣ�
3.�Ӻ������嵽����ĵ��ߣ�
4.��д�������У�ȷ�����������ת���󣬿����������Դ���أ�
5.����K0/K1ȷ�����ת���ٶȼ�����
6.������ַ�ת������K2�����ص���ģʽ������ֱ�ӹر��������Դ��
*************************************************************************/
void TestMotor (void)
{
  int16_t duty = 1500;
  char txt[16];
  
  TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);          //����
  TFTSPI_P8X16Str(2, 0, "LQ Motor Test", u16RED, u16BLUE);
  MotorInit(12500);
  while (1)
  {
    if (KEY_Read(KEY0) == 0)      //����KEY0����ռ�ձȼ�С
    {
      if (duty > -PWM_DUTY_MAX)
        duty -= 100;
    }
    if (KEY_Read(KEY2) == 0)      //����KEY2����ռ�ձȼӴ�
    {
      if (duty < PWM_DUTY_MAX) //��ռ�ձ�Ϊ10000
        duty += 100;
    }
    if (KEY_Read(KEY1) == 0)      //����KEY1����ռ�ձ���ֵ
    {
      duty = 1500;
    }
    
    //MotorCtrl(duty, duty);
    MotorCtrl4w(duty, duty, duty, duty);
    
    sprintf(txt, "PWM: %05d;", duty);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);       //�ַ�����ʾ
    //UART_PutStr(UART_1,txt);
    
    LED_Ctrl(LED0, RVS);       //��ƽ��ת,LED��˸
    delayms(200);              //��ʱ�ȴ�
  }
}
/*************************************************************************
*  �������ƣ�void ServoInit(void)
*  ����˵�������PWM��ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2����� TIM4_PWM_Init(100-1);  //100HZ����
*************************************************************************/
void ServoInit(u16 freq)
{
                              //���1�õ���C6 C8;���2�õ���C7 C9
    PWM_InitConfig(PWM3_CH1_PC6, 50, 1500);  //���Ĭ���õ���C6�ڣ�TIM3��ʱ��
                              //�������Ͷ��ͬʱʹ�ã��ܿ���ͻ
}

/*************************************************************************
*  �������ƣ�void ServoCtrl(uint32_t duty)
*  ����˵�������ת�Ǻ���������С�����˷�Χ���ƣ���С
*  ����˵����short duty��ռ�ձ�
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2���������ͨ����ֻ��Ҫһ���������
*************************************************************************/
void ServoCtrl (uint16_t duty)
{
  /*
  if (duty >= Servo_Left_Max)                  //���Ʒ�ֵ
    duty = Servo_Left_Max;
  else if (duty <= Servo_Right_Min)            //���Ʒ�ֵ
    duty = Servo_Right_Min;
  */
  PWM_Set_Duty(PWM3_CH1_PC6,duty);//C6;
}

/*************************************************************************
*  �������ƣ�Test_Servo(void)
*  ����˵�������PWM��ʼ�������Ա궨���PWM����SD5/S3010���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��4��1��
*  ��    ע������2�����
��ע�����ע�⣬һ��Ҫ�Զ����ǽ�������
ʹ������ĸ��������̣�
1.��ʹ�����ñ�������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
2.Ȼ��ȷ����������ѹ��SD5�����5V���磬S3010��6-7V���磡����
3.�Ѷ���Ķ���ȥ�����ö����������ת����
4.��д�������У��ö��ת������ֵ���������û��Ӧ�ظ�1-2�������ߵ��������PWMƵ�ʼ�ռ�ձȣ����ܿ�Ϊ׼��
5.����ܿغ�������ת�������֨֨�죬�Կ�ת������ʱ����װ�϶��̣�
6.����K0/K1ȷ�����������ת�����ޣ�������������Ϊ�����޷���ֹ�����ת�ջ٣�
*************************************************************************/
void TestServo (void)
{
  char txt[16] = "X:";
  signed short duty = Servo_Center_Mid;
  
  TFTSPI_CLS(u16BLUE);          //����
  TFTSPI_P8X16Str(2, 0, "LQ Servo Test", u16RED, u16BLUE);
  ServoInit(100);
  ServoCtrl(Servo_Center_Mid);      //��ֵ
  while (1)
  {
    if (!KEY_Read(KEY0))
    {
      if (duty > 10)  //��ֹduty��
      {
        duty -= 10;
      }
    }
    if (!KEY_Read(KEY1))
    {
      duty = Servo_Center_Mid;
    }
    if (!KEY_Read(KEY2))
    {
      duty += 10;
    }
    ServoCtrl(duty); 
    sprintf(txt, "Servo duty:%04d ", duty);
    TFTSPI_P8X16Str(1, 2, txt, u16BLACK, u16GREEN); //��ʾ����ʵ�����������Ա�������
    LED_Ctrl(LED1, RVS);        //�ĸ�LEDͬʱ��˸;
    delayms(100);
  }
}


/*************************************************************************
*  �������ƣ�uint8 SetCircleNum (void)
*  ����˵����������Ҫ����Բ���ĸ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��11��18��
*  ��    ע��
*************************************************************************/
uint8_t SetCircleNum (void)
{
  char txt[16] = " ";
  uint8_t num = 1;
  
  TFTSPI_CLS(u16BLACK);            // ����
  TFTSPI_P8X16Str(2, 1, "LQ Smart Car", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 3, "K2 num +", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 4, "K1 set OK", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 5, "K0 num -", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 7, "Ring num:  ", u16RED, u16BLUE);
  
  while (KEY_Read(KEY1))
  {
    if (KEY_Read(KEY2) == 0)
    {
      if (num < 10)
        num++;
    }
    else if (KEY_Read(KEY0) == 0)
    {
      if (num > 0)
        num--;
    }
    sprintf(txt, "Ring num: %d ", num);
    TFTSPI_P8X16Str(2, 7, txt, u16WHITE, u16BLUE);
    
    delayms(100);
  }
  return num;
}

/*************************************************************************
*  �������ƣ�void OutInGarage(uint8_t inout, uint8_t lr)
*  ����˵���������
*  ����˵����uint8 inout:0���⣬1��⣻
*          uint8_t lr��0�����⣻1�ҳ����
*  �������أ���
*  �޸�ʱ�䣺2020��11��18��
*  ��    ע��   OutInGarage(1,0); // �Ҳ����
*************************************************************************/

void OutInGarage (uint8_t inout, uint8_t lr)
{
  int32_t ps = 0;
  
  if (lr)           // 1�ҳ����
  {
    if (inout)    // 1�����
    {
      //NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // ��ȡSTM0 ��ǰʱ�䣬�õ�����
      // 2020���¼ӳ���Ԫ�أ��˴�Ϊä�����
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // ���е���
      MotorCtrl(2000, 2000);        // ��󵹳�
      while (RAllPulse < ps + 2000) // ����ǰ����Լ35cm
      {
        delayms(10);
      }
      MotorCtrl(-2500, -2500);      // ɲ��
      delayms(300);
      
      ps = RAllPulse;
      ServoCtrl(Servo_Right_Min);   // ������Ҵ���Ϊ������׼��
      MotorCtrl(-3000, -2000);      // �Һ󵹳������ֿ죬��������
      while (RAllPulse > ps - 2000) // ��ͣ��λ���⣬��ԼҪ512������2000�����壬����512�����������1��5790������
      {   // �ҵ�� ĸ���ϱ�����1��С������Ϊ��ֵ�����ۼӵ�����Ϊֹ
        delayms(10);
      }
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // ���е���
      MotorCtrl(-2500, -2500);      // ��󵹳������ֿ죬������
      while (RAllPulse > ps - 800)  // �ҵ�� ĸ���ϱ�����1��С������Ϊ��ֵ�����ۼӵ�����Ϊֹ
      {
        delayms(10);
      }
      //DisableInterrupts;  // �ر������ж�
      MotorCtrl(3000, 3000);
      delayms(300);                // �����תɲ������ֹ����������ʱ������ٶȵ���
      MotorCtrl(0, 0);             // ͣ��
      while (1);                   // �����ϣ�����ͣ��
    }
    else  // 0�ҳ���
    {
      // 2020���¼ӳ���Ԫ�أ��˴�Ϊä�߳���
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid); // ֱ�д�Լ10cm
      MotorCtrl(2500, 2500);       //
      while (RAllPulse < ps + 600)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Right_Min); // ������Ҵ���Ϊ������׼��
      MotorCtrl(3500, 3000);       // ��ת�����ֿ죬��������
      while (RAllPulse < ps + 1200)
      {
        delayms(10);
      }
    }
  }
  else // 0�������⣻
  {
    if (inout) // 1�����
    {
      //NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // ��ȡSTM0 ��ǰʱ�䣬�õ�����
      // 2020���¼ӳ���Ԫ�أ��˴�Ϊä�����
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // ���е���
      MotorCtrl(2000, 2000);        // ��󵹳�
      while (RAllPulse < ps + 2500) // ����ǰ����Լ35cm
      {
        delayms(10);
      }
      MotorCtrl(-2500, -2500);      // ɲ��
      delayms(300);
      
      ps = RAllPulse;
      ServoCtrl(Servo_Left_Max);    // ������Ҵ���Ϊ������׼��
      MotorCtrl(-2000, -3000);      // ��󵹳������ֿ죬������
      while (RAllPulse > ps - 3000)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // ���е���
      MotorCtrl(-2500, -2500);      // ��󵹳������ֿ죬������
      while (RAllPulse > ps - 1500)
      {
        delayms(10);
      }
      //DisableInterrupts; // �ر������ж�
      MotorCtrl(3000, 3000);
      delayms(300);               // �����תɲ������ֹ����������ʱ������ٶȵ���
      MotorCtrl(0, 0);            // ͣ��
      while (1) ;                 // �����ϣ�����ͣ��
    }
    else  // 0�����
    {
      // 2020���¼ӳ���Ԫ�أ��˴�Ϊä�߳���
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid); // ֱ�д�Լ10cm
      MotorCtrl(2500, 2500);       //
      while (RAllPulse < ps + 600)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Left_Max); // ����������Ϊ������׼��
      MotorCtrl(3000, 3500);     // ��ת�����ֿ죬��������
      while (RAllPulse < ps + 1800)
      {
        delayms(10);
      }
    }
  }
}

/*************************************************************************
*  �������ƣ�uint8 ReadOutInGarageMode(void)
*  ����˵������ȡ���뿪�����ó����ģʽ
*  ����˵������
*  �������أ������ģʽ,0�����⣻Ĭ��1�ҳ����
*  �޸�ʱ�䣺2020��11��18��
*  ��    ע��
*************************************************************************/
uint8_t ReadOutInGarageMode (void)
{
  return (KEY_Read(DSW0));
}
//