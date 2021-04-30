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
*  �������ƣ�void GPIO_KEY_Init(void)
*  ����˵����GPIO��ʼ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void GPIO_KEY_Init(void)
{
/*
    //�׿��
    GPIO_InitTypeDef GPIO_InitStructure;
    //ʹ��GPIOA,GPIOB,GPIOCʱ��   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB,ENABLE);// ����GPIOA��ȫ����ʱ��
    //PA8��12,15 K1
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    //���ó���������   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    //��ʼ��GPIOC13 
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //PB345
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
    //���ó���������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    //��ʼ��GPIOB  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
*/
    //��װ��
    PIN_InitConfig(PC1, GPI, 1,GPIO_Mode_IPU);
    PIN_InitConfig(PC2, GPI, 1,GPIO_Mode_IPU);
    PIN_InitConfig(PC3, GPI, 1,GPIO_Mode_IPU);
    PIN_InitConfig(PB5, GPI, 1,GPIO_Mode_IPU);
    PIN_InitConfig(PB4, GPI, 1,GPIO_Mode_IPU);
    PIN_InitConfig(PB3, GPI, 1,GPIO_Mode_IPU);
}

/*************************************************************************
*  �������ƣ�unsigned char KEY_Read(KEYn_e KEYno)
*  ����˵������ȡ����״̬
*  ����˵����KEYn_e KEYno�������
*  �������أ�����״̬��0/1
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned char KEY_Read(KEYn_e KEYno)
{
    switch(KEYno)
    {
      case KEY0:
        return GPIO_ReadInputDataBit(KEY0p);//ĸ���ϰ���0
      //break;

      case KEY1:
        return GPIO_ReadInputDataBit(KEY1p);//ĸ���ϰ���1
      //break;

      case KEY2:
        return GPIO_ReadInputDataBit(KEY2p);//ĸ���ϰ���2
      //break;

      case DSW0:
        return GPIO_ReadInputDataBit(DSW0p);//ĸ���ϲ��뿪��0
      //break;

      case DSW1:
        return GPIO_ReadInputDataBit(DSW1p);//ĸ���ϲ��뿪��1
      //break;
      default:
        return 0XFF;
    }
    //return 0;
}


/*************************************************************************
*  �������ƣ�unsigned char KEY_Read_All(void)
*  ����˵������ȡȫ������״̬
*  ����˵������
*  �������أ��������״̬��0--7����״̬
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע����ȡ��������״̬��������ϼ�ʹ��
*************************************************************************/
unsigned char KEY_Read_All(void)
{
   unsigned char tm=0;

   tm = (GPIO_ReadInputDataBit(KEY0p)|(GPIO_ReadInputDataBit(KEY1p)<<1)|(GPIO_ReadInputDataBit(KEY2p)<<2));//��ȡ��������״̬������
   if(tm==0x07)
	{
	   return 0;
	}
//   while(tm == (GPIO_ReadInputDataBit(KEY0p)|(GPIO_ReadInputDataBit(KEY1p)<<1)|(GPIO_ReadInputDataBit(KEY2p)<<2)));//�ȴ������ͷ�

   return  (~tm)&0X07;
}


/*************************************************************************
*  �������ƣ�void Test_GPIO_KEY(void)
*  ����˵�������Գ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע������KEY0/1/2����� LEDȫ��
*************************************************************************/
void Test_GPIO_KEY(void)
 {
	unsigned char k=0xFF;
	GPIO_LED_Init();
	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2);

 		if(k==0) LED_Ctrl(LEDALL, ON); //����KEY0/1/2����� LEDȫ��
 		else     LED_Ctrl(LEDALL, OFF);//�ɿ�KEY0/1/2����� LEDȫ��

 		delayms(100);                //��ʱ�ȴ�
 	}
 }


/*************************************************************************
*  �������ƣ�void Test_ComKEY_Tft(void)
*  ����˵����������ϰ������ڲ�����ʾ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע������״̬������ʾ
*************************************************************************/
void Test_ComKEY_Tft(void)
 {
	unsigned char k=0xFF;

	TFTSPI_Init(0);        //LCD��ʼ��  0:����  1������
	TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
	TFTSPI_Show_Logo(0,37);//��ʾ����LOGO
	TFTSPI_P16x16Str(0,0,(unsigned char*)"�����������ܿƼ�",u16RED,u16BLUE);		//�ַ�����ʾ
	TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);		//�ַ�����ʾ
	delayms(1000);              //��ʱ�ȴ�
	TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
	TFTSPI_CLS(u16BLACK);   //��ɫ��Ļ
	TFTSPI_P8X16Str(0,1,"KEY Pressed:  ",u16WHITE,u16BLACK);		//�ַ�����ʾ

	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read_All();
 		switch(k)
 		{
 		  case NOKEYDOWN:
 			 //TFTSPI_P8X16Str(35,3,"NO key!  ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY0DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0!    ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY1DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY1!    ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY2DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY2!    ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY01DOWN:
 			 TFTSPI_P8X16Str(35,3,"KEY0&1!  ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY02DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0&2!  ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY12DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY1&2!  ",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  case KEY012DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0&1&2!",u16WHITE,u16BLACK);		//�ַ�����ʾ
 		  break;
 		  default:
 		  break;
 		 }
 		LED_Ctrl(LED0,RVS);        //�ĸ�LEDͬʱ��˸
 		delayms(100);              //��ʱ�ȴ�
 	}
 }



