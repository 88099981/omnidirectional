/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main program body.
*******************************************************************************/
#include "debug.h"

/*******************************************************************************
* Function Name  : BKP_Tamper_Init
* Description    : Initializes the BKP Tamper.
* Input          : None
* Return         : None
*******************************************************************************/
void BKP_Tamper_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );

	BKP_TamperPinCmd( DISABLE );
  PWR_BackupAccessCmd( ENABLE );

	BKP_WriteBackupRegister( BKP_DR1, 0x9880 );
	BKP_WriteBackupRegister( BKP_DR2, 0x5678 );
	BKP_WriteBackupRegister( BKP_DR3, 0xABCD );
	BKP_WriteBackupRegister( BKP_DR4, 0x3456 );

	printf( "BKP_DR1:%08x\r\n", BKP->DATAR1 );
	printf( "BKP_DR2:%08x\r\n", BKP->DATAR2 );
	printf( "BKP_DR3:%08x\r\n", BKP->DATAR3 );
  printf( "BKP_DR4:%08x\r\n", BKP->DATAR4 );

	BKP_TamperPinLevelConfig( BKP_TamperPinLevel_High );  //TPAL:0??PC13 set input-pull-down
//	BKP_TamperPinLevelConfig( BKP_TamperPinLevel_Low );	 //TPAL:1??PC13 input-pull-up

	NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	BKP_ITConfig( ENABLE );
	BKP_TamperPinCmd( ENABLE );
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);

	BKP_Tamper_Init();

	while(1);
}


