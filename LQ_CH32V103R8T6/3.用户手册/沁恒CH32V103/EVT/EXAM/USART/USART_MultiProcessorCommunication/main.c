/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main program body.
*******************************************************************************/
#include "debug.h"


/* Global typedef */

/* Global define */

/* Global Variable */


/*******************************************************************************
* Function Name  : USARTx_CFG
* Description    : Initializes the USART2 & USART3 peripheral.
* Input          : None
* Return         : None
*******************************************************************************/
void USARTx_CFG(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB , ENABLE);

  /* USART2 TX-->A.2   RX-->A.3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* USART3 TX-->B.10  RX-->B.11 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

  USART_Init(USART2, &USART_InitStructure);
	USART_Init(USART3, &USART_InitStructure);

  USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
  USART_SetAddress(USART2, 0x1);
  USART_SetAddress(USART3, 0x2);
  USART_WakeUpConfig(USART3, USART_WakeUp_AddressMark);
	USART_ReceiverWakeUpCmd(USART3,ENABLE);                       /* USART3 Into Silence */
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);

	printf("USART MultiProcessor TEST\r\n");
  USARTx_CFG();                                                 /* USART2 & USART3 Initializes */

	while(1)
  {
  	USART_SendData(USART2, 0x102);                              /* Send USART3??s addr */
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
		{
		}
		USART_SendData(USART2, 0xAA);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) /* waiting for sending finish */
		{
		}
		if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)
    {
      if(USART_ReceiveData(USART3) == 0xAA)
      {
        printf("USART3 Receive Data\r\n");
				Delay_Ms(1000);
      }
    }

	}
}

