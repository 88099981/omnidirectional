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
#ifndef __LQ_UART_BT_H_
#define __LQ_UART_BT_H_

#include "include.h"


//L.Q UART RX �ܽ�ö��
typedef enum
{
  UART1_RX_A10=0,   //Ĭ������
  UART1_RX_B7,    //��ӳ��
  UART2_RX_A3,    //Ĭ������
  UART3_RX_B11,   //Ĭ������
  UART3_RX_C11    //������ӳ��
}UART_RX_t;

//L.Q UART TX �ܽ�ö��
typedef enum
{
  UART1_TX_A9= 0,  //Ĭ������
  UART1_TX_B6,         //��ӳ��
  UART2_TX_A2 ,  //Ĭ������
  UART3_TX_B10, //Ĭ������
  UART3_TX_C10,       //������ӳ��
}UART_TX_t;

void UART_RX_IRQ_Config(USART_TypeDef* USARTx,u8 status);
void UART_TX_IRQ_Config(USART_TypeDef* USARTx,u8 status);
//void UART_InitConfig(USART_TypeDef* USARTx,unsigned long baudrate);
void UART_InitConfig(USART_TypeDef* USARTx,unsigned long baudrate,UART_TX_t tx_pin, UART_RX_t rx_pin);
void UART_PutChar(USART_TypeDef* USARTx,char ch);
void UART_PutStr(USART_TypeDef* USARTx,char *str);
char UART_GetChar(USART_TypeDef* USARTx);
void Test_Bluetooth(void);

#endif
