/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�CH32V103R8T6���İ�
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��IDE��CH32V103R8T6:MounRiver Studio�����ϰ汾
��Target �� CH32V103R8T6
��SYS PLL�� 80MHz
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//////////////////////////////////���²���Ϊ���ܲ���/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
u8 RX2CHAR=0;
//extern signed short realSpeed3, realSpeed4;
//extern volatile int16_t targetSpeed;
void USART1_IRQHandler(void)
{
  //�����ж�
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)  != RESET)
  {
    USART_ClearFlag(USART1,USART_FLAG_RXNE);
    /* �û����� */
    /* ���ݽǶ���Ϣ  ����С��  */
       /*if(Game_Over==0)
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
              SpeedCtrl(0, targetSpeed*2, 0); //
              //SpeedCtrl((BackOffset+FrontOffset)/3, targetSpeed*2, (BackOffset-FrontOffset)/3);
          }
       }*/
    UART_PutChar(USART1,UART_GetChar(USART1));
  }
}
//˫��ͨ�ŵĽ��տ�
extern u8 num, rev_data[4], function;
extern u8 temp;
extern int16_t ECPULSE3, ECPULSE4;
void USART2_IRQHandler(void)
{
    u8 sum;
    //�����ж�
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)  != RESET)
    {
        USART_ClearFlag(USART2,USART_FLAG_RXNE);
        /* �û����� */
#ifdef DOUBLE_CORE_HOST
        temp = UART_GetChar(USART2);
        if( (temp == 0xAA) && (num == 0) )
        {
            num = 1;
            //UART_PutChar(USART1, temp);
        }
        else if( ( (temp == 0x01) || (temp == 0x02) || (temp == 0x03) || (temp == 0x04) ) && (num == 1) )
        {
            function = temp;
            num = 2;
            //UART_PutChar(USART1, function);
        }
        else if( num == 2 )
        {
            rev_data[0] = temp;
            num = 3;
            //UART_PutChar(USART1, rev_data[0]);
        }
        else if( num == 3 )
        {
            rev_data[1] = temp;
            num = 4;
            //UART_PutChar(USART1, rev_data[1]);
        }
        else if( num == 4 )
        {
            rev_data[2] = temp;
            num = 5;
            //UART_PutChar(USART1, rev_data[2]);
        }
        else if( num == 5 )
        {
            rev_data[3] = temp;
            num = 6;
            //UART_PutChar(USART1, rev_data[3]);
        }
        else if( num == 6 )
        {
            sum = 0xAA + function + rev_data[0] + rev_data[1] + rev_data[2] + rev_data[3];
            //UART_PutChar(USART1, temp);
            //UART_PutChar(USART1, sum);
            if( sum == temp )
            {
                switch(function)
                {
                    case 1 : ECPULSE1 = (rev_data[1]<<8) + rev_data[0]; break;
                    case 2 : ECPULSE3 = (rev_data[1]<<8) + rev_data[0]; break;
                    case 3 : ECPULSE2 = (rev_data[1]<<8) + rev_data[0]; break;
                    case 4 : ECPULSE4 = (rev_data[1]<<8) + rev_data[0]; break;
                    default : break;
                }
            }
            num = 0;
        }
        else
            num = 0;
#elif defined  DOUBLE_CORE_SLAVE

#endif
  }
}
void USART3_IRQHandler(void)
{
  //�����ж�
  if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)  != RESET)
  {
    USART_ClearFlag(USART3,USART_FLAG_RXNE);
    /* �û����� */
    UART_PutChar(USART3,UART_GetChar(USART3));
  }
}
/*************************************************************************
*  �������ƣ�void UART1_InitConfig(unsigned long baudrate)
*  ����˵��������ģ���ʼ��
*  ����˵����
* @param    RxPin   �� ���ڽ��չܽ�
* @param    TxPin   �� ���ڷ��͹ܽ�
* @param    baudrate�� ������
*  �������أ��ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��void UART1_InitConfig(115200);   //��ʼ������1 ������ 115200 ����żУ�� 1ֹͣλ ʹ�ùܽ�
*
//SPIN27PS���ڸ��ã�
//RX1 D1 A9 A10 C11     B7 B8
//TX1 D0 A10 A9 C10 C12 B6 B9
//RX2 A3 C5 A15
//TX2 A2 C4 A14
*
//CH32V103R8T6���ڸ��ã�
//RX1 A10 B7
//TX1 A9  B6
//RX2 A3
//TX2 A2
//RX3 B11 C11
//TX3 B10 C10
*************************************************************************/
void UART_PinConfig(UART_TX_t tx_pin, UART_RX_t rx_pin)
{
  if(tx_pin == UART1_TX_A9 )
  {
    PIN_InitConfig(PA9, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
  }
  else if(tx_pin == UART1_TX_B6)
  {
    PIN_InitConfig(PB6, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
  }
  else if(tx_pin == UART2_TX_A2)
  {
    PIN_InitConfig(PA2, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
  }
  else if(tx_pin == UART3_TX_B10)
  {
    PIN_InitConfig(PB10, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
  }
  else if(tx_pin == UART3_TX_C10)
  {
    PIN_InitConfig(PC10, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_Out_PP);
  }
  
  if(rx_pin == UART1_RX_A10)
  {
    PIN_InitConfig(PA10, GPI, 0, GPIO_Mode_IN_FLOATING);
  }
  else if(rx_pin == UART1_RX_B7)
  {
    PIN_InitConfig(PB7, GPI, 0, GPIO_Mode_IN_FLOATING);
  }
  else if(rx_pin == UART2_RX_A3)
  {
    PIN_InitConfig(PA3, GPI, 0, GPIO_Mode_IN_FLOATING);
  }
  else if(rx_pin == UART3_RX_B11)
  {
    PIN_InitConfig(PB11, GPI, 0, GPIO_Mode_IN_FLOATING);
  }
  else if(rx_pin == UART3_RX_C11)
  {
    PIN_InitConfig(PC11, GPI, 0, GPIO_Mode_IN_FLOATING);
  }
}
/*************************************************************************
*  �������ƣ�void UART_InitConfig(USART_TypeDef* USARTx,unsigned long baudrate,UART_TX_t tx_pin, UART_RX_t rx_pin)
*  ����˵��������ģ���ʼ��
*  ����˵����
* @param    RxPin   �� ���ڽ��չܽ�
* @param    TxPin   �� ���ڷ��͹ܽ�
* @param    baudrate�� ������
*  �������أ��ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_InitConfig(USAR1,9600,UART1_TX_A9,UART1_RX_A10); //��ʼ������1 ������ 115200 ����żУ�� 1ֹͣλ ʹ�ùܽ�
*************************************************************************/
void UART_InitConfig(USART_TypeDef* USARTx,unsigned long baudrate,UART_TX_t tx_pin, UART_RX_t rx_pin)
{
  USART_InitTypeDef USART_InitStructure;
  
  //��������ʹ��
  if(USART1 == USARTx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  else if(USART2 == USARTx) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  else if(USART3 == USARTx) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  //���ų�ʼ��
  UART_PinConfig(tx_pin, rx_pin);
  
  //���ڲ�������
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  
  //ʹ�ܴ���
  UART_RX_IRQ_Config(USARTx,ENABLE);
  USART_Init(USARTx, &USART_InitStructure);
  USART_Cmd(USARTx, ENABLE);
}

/*************************************************************************
*  �������ƣ�void UART_RX_IRQ_Config(UART_NAME_t UART_x,u8 status)
*  ����˵�����������ڽ����ж�
*  ����˵����
* @param    UART_x       ����ͨ��
* @param    status      ʹ�ܻ��ֹ
*  �������أ��ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_RX_IRQ_Config(UART_1, ENABLE);  //��������1�����ж�
*************************************************************************/
void UART_RX_IRQ_Config(USART_TypeDef* USARTx,u8 status)
{
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  if(USART1 == USARTx) nvic_init(USART1_IRQn, 0, 0, status);
  else if(USART2 == USARTx) nvic_init(USART2_IRQn, 0, 0, status);
  else if(USART3 == USARTx) nvic_init(USART3_IRQn, 0, 0, status);
}

/*************************************************************************
*  �������ƣ�void UART_RX_IRQ_Config(USART_TypeDef* USARTx,u8 status)
*  ����˵�����������ڽ����ж�
*  ����˵����
* @param    UART_x       ����ͨ��
* @param    status      ʹ�ܻ��ֹ
*  �������أ��ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_RX_IRQ_Config(UART_1, ENABLE);  //��������1�����ж�
*************************************************************************/
void UART_TX_IRQ_Config(USART_TypeDef* USARTx,u8 status)
{
  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
  if(USART1 == USARTx) nvic_init(USART1_IRQn, 0, 0, status);
  else if(USART2 == USARTx) nvic_init(USART2_IRQn, 0, 0, status);
  else if(USART3 == USARTx) nvic_init(USART3_IRQn, 0, 0, status);
}
/*************************************************************************
*  �������ƣ�void UART_PutChar(UART_t  uratn, char ch)
*  ����˵����UART�����ֽں���,ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ ch    �� Ҫ��ӡ���ַ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutChar(UART0, 'a');  //��ӡ�ַ�a
*************************************************************************/
void UART_PutChar(USART_TypeDef* USARTx,char ch)
{
  USART_SendData(USARTx, ch);
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

/*************************************************************************
*  �������ƣ�void UART_PutStr(UART_t  uratn, char *str)
*  ����˵����UART�����ַ�������(�� NULL ֹͣ����),ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ str   �� Ҫ��ӡ���ַ�����ַ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutStr(UART3, "123456789"); //����9���ֽ�
*************************************************************************/
void UART_PutStr(USART_TypeDef* USARTx,char *str)
{
  while(*str)
  {
    UART_PutChar(USARTx,*str++);
  }
}
/*************************************************************************
*  �������ƣ�char UART_GetChar(UART_t  uratn)
*  ����˵����UART��ȡ�ֽ� ʹ��ǰ���ȳ�ʼ����Ӧ���� ����ǰ��ȷ���н��յ����� �����ȴ����ݵ���
*  ����˵����uratn �� UART0 - UART3
*  �������أ���ȡ�ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_GetChar(UART3); //����һ���ַ�
*************************************************************************/
char UART_GetChar(USART_TypeDef* USARTx)
{  
  return (USART_ReceiveData(USARTx));
}


/*******************************************************************************
* Function Name  : _write
* Description    : Support Printf Function
* Input          : *buf: UART send Data.
*                  size: Data length
* Return         : size: Data length
*******************************************************************************/
int _write(int fd, char *buf, int size)
{
  int i;
  
  for(i=0; i<size; i++)
  {
#if (DEBUG == DEBUG_UART1)
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    UART_PutChar(USART1, *buf++);
#elif (DEBUG == DEBUG_UART2)
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    UART_PutChar(USART2, *buf++);
#elif (DEBUG == DEBUG_UART3)
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
    UART_PutChar(USART3, *buf++);
#endif
  }
  
  return size;
}

/*************************************************************************
*  �������ƣ�void Test_Bluetooth(void);
*  ����˵����UART���Ժ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void Test_Bluetooth(void)
{
  //UART_InitConfig(USART1,115200);
  UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //��ʼ������1 ������ 115200 ����żУ�� 1ֹͣλ ʹ�ùܽ�
  UART_PutStr(USART1," UART1 LongQiu \r\n"); //�����ַ�������λ��
  while(1)
  {
    UART_PutChar(USART1,'L');                     //���� �ֽڵ�UART��
    delayms(500);                                  //��ʱ�ȴ�
    UART_PutChar(USART1,'Q');                      //���� �ֽڵ�UART��
    
    UART_PutStr(USART1,"USART1 LongQiu \r\n");      //�����ַ�������λ��
    
    LED_Ctrl(LED0,RVS);        //��ƽ��ת,LED��˸
    delayms(500);              //��ʱ�ȴ�
  }
}

//
