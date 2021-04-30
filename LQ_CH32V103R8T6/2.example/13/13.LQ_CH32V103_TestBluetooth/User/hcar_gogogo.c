/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               ����ȫ������ ��д                   00**
**00             ��Ա ����گ�� ������ ʯ����                                               00 **
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            �������ã�  go                           00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include"include.h"
#ifdef DOUBLE_CORE_HOST
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////˫��ͨ��֮���������������㣬�����ұ߿���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 temp;
u8 num = 0;
u8 function;
u8 rev_data[4];
extern int16_t ECPULSE3, ECPULSE4;

void hcar_gogogo(void)//˫��ͨ��֮���������������㣬�����ұ߿���
{

  // ϵͳ����ʱ������ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysTick_Init();
  hcar_init();
  MotorInit(12500);

  TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
  TFTSPI_P8X16Str(0,0,(unsigned char*)"Welcome, my lord",u16WHITE,u16BLACK);//�ַ�����ʾ
  Delay_Ms(100);
  TFTSPI_CLS(u16BLUE);
  //TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);              //�ַ�����ʾ
  //TFTSPI_CLS(u16BLACK);   //��ɫ��Ļ
  UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //��ʼ������1 ������ 115200 ����żУ�� 1ֹͣλ ʹ�ùܽ�
  UART_PutStr(USART1," UART1 LongQiu \r\n"); //�����ַ�������λ��

  TIMER_InitConfig(TIMER_1, 10);// ��ȡPID��ʱ�ж�10ms
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  while(1)//��Ҫ������Ƭ��ͬʱʹ����Ļ
  {
     encoder_print();
     // UART_PutChar(USART1,ECPULSE3);
     //UART_PutStr(USART1," UART1 LongQiu \r\n"); //�����ַ�������λ��
  }
}

#elif  defined DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 num = 0;
u8 function;
u8 rev_data[4];
u8 temp;
void hcar_gogogo(void)//˫��ͨ��֮�ӻ������ͱ������ͻ�����ͷ�㷨������ݣ�������߿���
{


  // ϵͳ����ʱ������ʼ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysTick_Init();
  hcar_init();
  //���ڳ�ʼ��Ϊ
  //UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //��ʼ������1 ������ 115200 ����żУ�� 1ֹͣλ ʹ�ùܽ�
  //UART_PutStr(USART1," UART1 LongQiu \r\n"); //�����ַ�������λ��


 //TFT��Ļ��ʼ��
 /* TFTSPI_Init(0);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
  TFTSPI_Show_Logo(0,37);//��ʾ����LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"�����������ܿƼ�",u16RED,u16BLUE);//�ַ�����ʾ
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);              //�ַ�����ʾ*/



  //TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  //TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ
  // ����ͷ��ʼ�� //
  //CAMERA_Init(50);
  //Test_CAMERA(); //PASS,����������������ͷ��Ƶ������96M

  TIMER_InitConfig(TIMER_1, 10);// ��ȡPID��ʱ�ж�10ms
  while(1)//��Ҫ������Ƭ��ͬʱʹ����Ļ
  {

    //Get_Bin_Image(0);
    // ��ʾ��������ͷͼ��
    //TFTSPI_BinRoad2((130-LCDW)/2, 0, LCDH, LCDW);
      int16_t tx=0;
      unsigned char sum = 0;

      tx = (int16_t)(ECPULSE4);//����ֱ���̫�󣬻ᳬ��100�Ļ������Գ���10��20...100

      UART_PutChar(USART2,0xAA);      //������֡ͷ
      UART_PutChar(USART2,0x04);      //������4
      UART_PutChar(USART2,tx);        //�Ͱ�λ
      UART_PutChar(USART2,tx>>8);    //�߰�λ
      UART_PutChar(USART2,0);
      UART_PutChar(USART2,0);
      sum = 0xAA + 0x04 + tx + (tx>>8); //���У��
      UART_PutChar(USART2, sum);
      sum = 0;

      tx = (int16_t)(ECPULSE3);//����ֱ���̫�󣬻ᳬ��100�Ļ������Գ���10��20...100

      UART_PutChar(USART2,0xAA);      //������֡ͷ
      UART_PutChar(USART2,0x02);      //������2
      UART_PutChar(USART2,tx);    //�Ͱ�λ
      UART_PutChar(USART2,tx>>8);    //�߰�λ
      UART_PutChar(USART2,0);
      UART_PutChar(USART2,0);
      sum = 0xAA + 0x02 + tx + (tx>>8); //���У��
      UART_PutChar(USART2, sum);
      sum = 0;

      LED_Ctrl(LEDALL,RVS);     //LED��ת��˸


  }
}
#endif
