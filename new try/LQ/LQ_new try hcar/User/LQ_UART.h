/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技CH32V103R8T6核心板
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【IDE】CH32V103R8T6:MounRiver Studio及以上版本
【Target 】 CH32V103R8T6
【SYS PLL】 80MHz
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_UART_BT_H_
#define __LQ_UART_BT_H_

#include "include.h"


//L.Q UART RX 管脚枚举
typedef enum
{
  UART1_RX_A10=0,   //默认引脚
  UART1_RX_B7,    //重映射
  UART2_RX_A3,    //默认引脚
  UART3_RX_B11,   //默认引脚
  UART3_RX_C11    //部分重映射
}UART_RX_t;

//L.Q UART TX 管脚枚举
typedef enum
{
  UART1_TX_A9= 0,  //默认引脚
  UART1_TX_B6,         //重映射
  UART2_TX_A2 ,  //默认引脚
  UART3_TX_B10, //默认引脚
  UART3_TX_C10,       //部分重映射
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
