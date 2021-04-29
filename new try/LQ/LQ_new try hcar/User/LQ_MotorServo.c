/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技SPIN27PS/CH32V103R8T6核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】CH32V103R8T6:MounRiver Studio及以上版本
 【IDE】MM32SPIN27PS:IAR7.8/MDK5.2及以上版本  
 【Target 】 SPIN27PS/CH32V103R8T6
 【SYS PLL】 80/96MHz
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

uint16_t BatVolt;         // 电池电压采集
unsigned char Camera_Flag; // 场标识

volatile uint8_t Game_Over = 0; // 小车完成全部任务，停车
int16_t ServoDuty = Servo_Center_Mid;
int16_t MotorDuty1 = 500;      // 电机驱动占空比数值
int16_t MotorDuty2 = 500;      // 电机驱动占空比数值
int32_t NowTime = 0;
uint16_t BatVolt = 0;           // 电池电压采集

// 四个轮子的期望速度
signed int targetSpeed1 = 0, targetSpeed2 = 0, targetSpeed3 = 0, targetSpeed4 = 0;
// 四个轮子的实际速度，另外一个核心板需要提供两路编码器速度
signed short  realSpeed1 = 0, realSpeed2 = 0, realSpeed3 = 0, realSpeed4 = 0;
pid_param_t pid1, pid2, pid3, pid4;
// 期望速度
volatile int16_t targetSpeed = 50;
//电机频率
#define MOTOR_FREQUENCY    PWM_DUTY_MAX//已修改为1000

#define USE7843or7971   //USEDRV8701 使用龙邱不同的驱动模块，选择对应的宏定义
//#define USEDRV8701
/*************************************************************************
*  函数名称：void MotorInit(u16 freq)
*  功能说明：电机PWM初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个电机  TIM8_PWM_Init(1000-1);  //1KHZ周期
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

  /*PidInit(&pid1);
  PidInit(&pid2);
  PidInit(&pid3);
  PidInit(&pid4);
  // PID 参数需要自行调节
  pid1.kp = 5;
  pid1.ki = 0;

  pid2.kp = 5;
  pid2.ki = 0;

  pid3.kp = 10;
  pid3.ki = 0;

  pid4.kp = 10;
  pid4.ki = 0; 已修改*/
}

/*!
  * @brief    速度控制函数
  *
  * @param    x   ： x方向速度                          小车正右方
  * @param    y   ： y方向速度                          小车正前方
  * @param    z   ： 逆时针方向速度
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
unsigned short int bluetooth[20];
float speed_error1 = 0;
float speed_error_last1 = 0;
float speed_error_last_last1 = 0;
float speed_out1 = 0;
float speed_error2 = 0;
float speed_error_last2 = 0;
float speed_error_last_last2 = 0;
float speed_out2 = 0;
float speed_error3 = 0;
float speed_error_last3 = 0;
float speed_error_last_last3 = 0;
float speed_out3 = 0;
float speed_error4 = 0;
float speed_error_last4 = 0;
float speed_error_last_last4 = 0;
float speed_out4 = 0;
float speed_now_filter1=0.0, speed_now_filter2=0.0;
float speed_now_filter3=0.0, speed_now_filter4=0.0;

int speed_1[10]={0}, speed_2[10]={0};
int speed_sum1=0, speed_sum2=0;
int speed_now1=0,  speed_now2=0;
int S_i=0;
int speed_3[10]={0}, speed_4[10]={0};
int speed_sum3=0, speed_sum4=0;
int speed_now3=0,  speed_now4=0;


float kp1=2, ki1=0, kd1=0;
float kp2=2.4, ki2=0.1, kd2=0;
float kp3=7, ki3=0, kd3=0;
float kp4=7.4, ki4=0, kd4=0;
void SpeedCtrl(signed int x, signed int y, signed int z)
{
    // 根据小车速度大小和方向  计算每个轮子的期望转速

    targetSpeed1 =  -x + y - z;
    targetSpeed2 =   x + y + z;
    targetSpeed3 =  -x + y + z;
    targetSpeed4 =   x + y - z;

    extern int16_t ECPULSE1;
    extern int16_t ECPULSE2;
    extern int16_t ECPULSE3;
    extern int16_t ECPULSE4;
    // 获取编码器值
    realSpeed1 = ECPULSE1; //左电机 母板上编码器1，小车前进为负值
    realSpeed2 = ECPULSE2; //右电机 母板上编码器2，小车前进为正值
    realSpeed3 = ECPULSE3; //另外一个核心板发数据过来
    realSpeed4 = ECPULSE4; //另外一个核心板发数据过来
    //滤波
    speed_1[S_i] = realSpeed1;
    speed_2[S_i] = realSpeed2;
    speed_3[S_i] = realSpeed3;
    speed_4[S_i] = realSpeed4;
    //队列
    S_i++;
    if(S_i > 9)
      S_i = 0;
    //累和归零
    speed_sum1 = 0;
    speed_sum2 = 0;
    speed_sum3 = 0;
    speed_sum4 = 0;
    //累和
    for(uint8_t j=0;j<10;j++)
    {
      speed_sum1 += speed_1[j];
    }
      for(uint8_t j=0;j<10;j++)
    {
      speed_sum2 += speed_2[j];
    }
      for(uint8_t j=0;j<10;j++)
      {
        speed_sum3 += speed_3[j];
      }
        for(uint8_t j=0;j<10;j++)
      {
        speed_sum4 += speed_4[j];
      }
      //平均
    //平均
    speed_now_filter1 = speed_sum1/10;
    speed_now_filter2 = speed_sum2/10;
    speed_now_filter3 = speed_sum3/10;
    speed_now_filter4 = speed_sum4/10;

    // PID处理
    /*speed1=PidIncCtrl(&pid1, targetSpeed1 - realSpeed1);
    speed2=PidIncCtrl(&pid2, targetSpeed2 - realSpeed2);
    speed3=PidIncCtrl(&pid3, targetSpeed3 - realSpeed3);
    speed4=PidIncCtrl(&pid4, targetSpeed4 - realSpeed4);已修改*/
    speed_error1 = (float)(targetSpeed1 - speed_now_filter1);
    speed_out1 += kp1 * (speed_error1 - speed_error_last1) + ki1 * speed_error1 +
                kd1*(speed_error1 - 2*speed_error_last1 + speed_error_last_last1);

    speed_error_last_last1 = speed_error_last1;
    speed_error_last1 = speed_error1;

    speed_error2 = (float)(targetSpeed2 - speed_now_filter2);
    speed_out2 += kp2 * (speed_error2 - speed_error_last2) + ki2 * speed_error2 +
                kd2*(speed_error2 - 2*speed_error_last2 + speed_error_last_last2);

    speed_error_last_last2 = speed_error_last2;
    speed_error_last2 = speed_error2;

    speed_error3 = (float)(targetSpeed3 - speed_now_filter3);
    speed_out3 += kp3 * (speed_error3 - speed_error_last3) + ki3 * speed_error3 +
                kd3*(speed_error3 - 2*speed_error_last3 + speed_error_last_last3);

    speed_error_last_last3 = speed_error_last3;
    speed_error_last3 = speed_error3;

    speed_error4 = (float)(targetSpeed4 - speed_now_filter4);
    speed_out4 += kp4 * (speed_error4 - speed_error_last4) + ki4 * speed_error4 +
                kd4*(speed_error4 - 2*speed_error_last4 + speed_error_last_last4);

    speed_error_last_last4 = speed_error_last4;
    speed_error_last4 = speed_error4;

    if(speed_out1>900)
    {
        speed_out1=900;
    }
    else if(speed_out1<-900)
    {
        speed_out1=-900;
    }
    if(speed_out2>900)
    {
        speed_out2=900;
    }
    else if(speed_out2<-900)
    {
        speed_out2=-900;
    }
    if(speed_out3>900)
    {
        speed_out3=900;
    }
    else if(speed_out3<-900)
    {
        speed_out3=-900;
    }
    if(speed_out4>900)
    {
        speed_out4=900;
    }
    else if(speed_out4<-900)
    {
        speed_out4=-900;
    }
    // 电机控制
    MotorCtrl4w(0, speed_out2, speed_out3, speed_out4);

    // 发送数据到匿名上位机画波形   方便调试PID参数

    bluetooth[0]=speed_out1;
    bluetooth[1]=speed_error1;
    bluetooth[2]=realSpeed1;
    bluetooth[3]=speed_out2;
    bluetooth[4]=speed_error2;
    bluetooth[5]=realSpeed2;
    Data_Send(USART1, bluetooth);

}


/*************************************************************************
*  函数名称：void MotorCtrl(int16_t motor1, int16_t motor2)
*  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
*  参数说明：
*    @param    motor1   ： 电机1占空比，范围【-10000，10000】
     @param    motor2   ： 电机2占空比，范围【-10000，10000】
     @param    motor3   ： 电机3占空比，范围【-10000，10000】
     @param    motor4   ： 电机4占空比，范围【-10000，10000】
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机，普通四轮只需要一个舵机即可
*************************************************************************/
#ifdef USE7843or7971
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4)
{
  //assert(motor1>TIM3_PWM_DUTY_MAX);
  //assert(motor2>TIM3_PWM_DUTY_MAX);
  //assert(motor3>TIM3_PWM_DUTY_MAX);
  //assert(motor4>TIM3_PWM_DUTY_MAX);
  //motor1=-motor1;
  motor2=-motor2;
  motor3=-motor3;
  motor4=-motor4;
  int16_t temp=0;
  temp=motor1;
  motor1=motor2;
  motor2=temp;

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
*  函数名称：TestMotor(void)
*  功能说明：测试标定输出PWM控制电机
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个电机
【注意事项】注意，一定要对舵机打角进行限制
使用龙邱母板测试流程：
1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
2.接好母板到驱动板的信号线及电源线；
3.接好驱动板到电机的导线；
4.烧写程序并运行，确定电机能正常转动后，开启驱动板电源开关；
5.按键K0/K1确定电机转动速度及方向；
6.如果出现疯转，按下K2键返回低速模式，或者直接关闭驱动板电源！
*************************************************************************/
void TestMotor (void)
{
  int16_t duty = 1500;
  char txt[16];
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);          //清屏
  TFTSPI_P8X16Str(2, 0, "LQ Motor Test", u16RED, u16BLUE);
  MotorInit(12500);
  while (1)
  {
    if (KEY_Read(KEY0) == 0)      //按下KEY0键，占空比减小
    {
      if (duty > -PWM_DUTY_MAX)
        duty -= 100;
    }
    if (KEY_Read(KEY2) == 0)      //按下KEY2键，占空比加大
    {
      if (duty < PWM_DUTY_MAX) //满占空比为10000
        duty += 100;
    }
    if (KEY_Read(KEY1) == 0)      //按下KEY1键，占空比中值
    {
      duty = 1500;
    }
    
    //MotorCtrl(duty, duty);
    MotorCtrl4w(duty, duty, duty, duty);
    
    sprintf(txt, "PWM: %05d;", duty);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);       //字符串显示
    //UART_PutStr(UART_1,txt);
    
    LED_Ctrl(LED0, RVS);       //电平翻转,LED闪烁
    delayms(200);              //延时等待
  }
}
/*************************************************************************
*  函数名称：void ServoInit(void)
*  功能说明：舵机PWM初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机 TIM4_PWM_Init(100-1);  //100HZ周期
*************************************************************************/
void ServoInit(u16 freq)
{
                              //电机1用的是C6 C8;电机2用的是C7 C9
    PWM_InitConfig(PWM3_CH1_PC6, 50, 1500);  //舵机默认用的是C6口，TIM3定时器
                              //如果电机和舵机同时使用，避开冲突
}

/*************************************************************************
*  函数名称：void ServoCtrl(uint32_t duty)
*  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
*  参数说明：short duty，占空比
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机，普通四轮只需要一个舵机即可
*************************************************************************/
void ServoCtrl (uint16_t duty)
{
  /*
  if (duty >= Servo_Left_Max)                  //限制幅值
    duty = Servo_Left_Max;
  else if (duty <= Servo_Right_Min)            //限制幅值
    duty = Servo_Right_Min;
  */
  PWM_Set_Duty(PWM3_CH1_PC6,duty);//C6;
}

/*************************************************************************
*  函数名称：Test_Servo(void)
*  功能说明：舵机PWM初始化，测试标定输出PWM控制SD5/S3010舵机
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机
【注意事项】注意，一定要对舵机打角进行限制
使用龙邱母板测试流程：
1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
2.然后确定舵机供电电压，SD5舵机用5V供电，S3010用6-7V供电！！！
3.把舵机的舵盘去掉，让舵机可以自由转动；
4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
*************************************************************************/
void TestServo (void)
{
  char txt[16] = "X:";
  signed short duty = Servo_Center_Mid;
  
  TFTSPI_CLS(u16BLUE);          //清屏
  TFTSPI_P8X16Str(2, 0, "LQ Servo Test", u16RED, u16BLUE);
  ServoInit(100);
  ServoCtrl(Servo_Center_Mid);      //中值
  while (1)
  {
    if (!KEY_Read(KEY0))
    {
      if (duty > 10)  //防止duty超
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
    TFTSPI_P8X16Str(1, 2, txt, u16BLACK, u16GREEN); //显示出库实际脉冲数，以便灵活调整
    LED_Ctrl(LED1, RVS);        //四个LED同时闪烁;
    delayms(100);
  }
}


/*************************************************************************
*  函数名称：uint8 SetCircleNum (void)
*  功能说明：设置需要进入圆环的个数；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
uint8_t SetCircleNum (void)
{
  char txt[16] = " ";
  uint8_t num = 1;
  
  TFTSPI_CLS(u16BLACK);            // 清屏
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
*  函数名称：void OutInGarage(uint8_t inout, uint8_t lr)
*  功能说明：出入库
*  参数说明：uint8 inout:0出库，1入库；
*          uint8_t lr：0左出入库；1右出入库
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：   OutInGarage(1,0); // 右侧出库
*************************************************************************/

void OutInGarage (uint8_t inout, uint8_t lr)
{
  int32_t ps = 0;
  
  if (lr)           // 1右出入库
  {
    if (inout)    // 1右入库
    {
      //NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // 获取STM0 当前时间，得到毫秒
      // 2020年新加出库元素，此处为盲走入库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(2000, 2000);        // 左后倒车
      while (RAllPulse < ps + 2000) // 继续前进大约35cm
      {
        delayms(10);
      }
      MotorCtrl(-2500, -2500);      // 刹车
      delayms(300);
      
      ps = RAllPulse;
      ServoCtrl(Servo_Right_Min);   // 舵机向右打死为出库做准备
      MotorCtrl(-3000, -2000);      // 右后倒车，左轮快，右轮慢，
      while (RAllPulse > ps - 2000) // 从停车位出库，大约要512编码器2000个脉冲，龙邱512带方向编码器1米5790个脉冲
      {   // 右电机 母板上编码器1，小车后退为正值，并累加到出库为止
        delayms(10);
      }
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(-2500, -2500);      // 左后倒车，右轮快，左轮慢
      while (RAllPulse > ps - 800)  // 右电机 母板上编码器1，小车后退为正值，并累加到出库为止
      {
        delayms(10);
      }
      //DisableInterrupts;  // 关闭所有中断
      MotorCtrl(3000, 3000);
      delayms(300);                // 电机反转刹车，防止滑出赛道，时间根据速度调整
      MotorCtrl(0, 0);             // 停车
      while (1);                   // 入库完毕，永久停车
    }
    else  // 0右出库
    {
      // 2020年新加出库元素，此处为盲走出库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid); // 直行大约10cm
      MotorCtrl(2500, 2500);       //
      while (RAllPulse < ps + 600)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Right_Min); // 舵机向右打死为出库做准备
      MotorCtrl(3500, 3000);       // 右转，左轮快，右轮慢，
      while (RAllPulse < ps + 1200)
      {
        delayms(10);
      }
    }
  }
  else // 0：左出入库；
  {
    if (inout) // 1左入库
    {
      //NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // 获取STM0 当前时间，得到毫秒
      // 2020年新加出库元素，此处为盲走入库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(2000, 2000);        // 左后倒车
      while (RAllPulse < ps + 2500) // 继续前进大约35cm
      {
        delayms(10);
      }
      MotorCtrl(-2500, -2500);      // 刹车
      delayms(300);
      
      ps = RAllPulse;
      ServoCtrl(Servo_Left_Max);    // 舵机向右打死为出库做准备
      MotorCtrl(-2000, -3000);      // 左后倒车，右轮快，左轮慢
      while (RAllPulse > ps - 3000)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(-2500, -2500);      // 左后倒车，右轮快，左轮慢
      while (RAllPulse > ps - 1500)
      {
        delayms(10);
      }
      //DisableInterrupts; // 关闭所有中断
      MotorCtrl(3000, 3000);
      delayms(300);               // 电机反转刹车，防止滑出赛道，时间根据速度调整
      MotorCtrl(0, 0);            // 停车
      while (1) ;                 // 入库完毕，永久停车
    }
    else  // 0左出库
    {
      // 2020年新加出库元素，此处为盲走出库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid); // 直行大约10cm
      MotorCtrl(2500, 2500);       //
      while (RAllPulse < ps + 600)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Left_Max); // 舵机向左打死为出库做准备
      MotorCtrl(3000, 3500);     // 左转，右轮快，左轮慢，
      while (RAllPulse < ps + 1800)
      {
        delayms(10);
      }
    }
  }
}

/*************************************************************************
*  函数名称：uint8 ReadOutInGarageMode(void)
*  功能说明：读取拨码开关设置出入库模式
*  参数说明：无
*  函数返回：出入库模式,0左出入库；默认1右出入库
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
uint8_t ReadOutInGarageMode (void)
{
  return (KEY_Read(DSW0));
}
//
