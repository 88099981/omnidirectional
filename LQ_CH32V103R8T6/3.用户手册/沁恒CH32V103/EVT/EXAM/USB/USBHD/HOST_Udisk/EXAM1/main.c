/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main program body.
*******************************************************************************/
#include "debug.h"
#include "string.h"
#include "CHRV3UFI.h"
/* Global define */


/* Global Variable */
__attribute__ ((aligned(4))) UINT8  RxBuffer[MAX_PACKET_SIZE] ;      // IN, must even address
__attribute__ ((aligned(4))) UINT8  TxBuffer[MAX_PACKET_SIZE] ;      // OUT, must even address
__attribute__ ((aligned(4))) UINT8  Com_Buffer[128];

UINT8  buf[100];                                  //长度可以根据应用自己指定

/* 检查操作状态,如果错误则显示错误代码并停机 */
void mStopIfError( UINT8 iError )
{
	if ( iError == ERR_SUCCESS )
	{
		return;    /* 操作成功 */
	}
	printf( "Error: %02x\r\n", (UINT16)iError );  /* 显示错误 */
	/* 遇到错误后,应该分析错误码以及CHRV3DiskStatus状态,例如调用CHRV3DiskReady查询当前U盘是否连接,如果U盘已断开那么就重新等待U盘插上再操作,
		 建议出错后的处理步骤:
		 1、调用一次CHRV3DiskReady,成功则继续操作,例如Open,Read/Write等
		 2、如果CHRV3DiskReady不成功,那么强行将从头开始操作 */
	while(1)
	{  }
}
/*******************************************************************************
* Function Name  : USBHD_ClockCmd
* Description    : Set USB clock.
* Input          : None
* Return         : None
*******************************************************************************/
void USBHD_ClockCmd(UINT32 RCC_USBCLKSource,FunctionalState NewState)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, NewState);
	EXTEN->EXTEN_CTR |= EXTEN_USBHD_IO_EN;
	RCC_USBCLKConfig(RCC_USBCLKSource);             //USBclk=PLLclk/1.5=48Mhz
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHD,NewState);
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Return         : None
*******************************************************************************/
int main()
{
	UINT8 s, c, i;
	UINT16 TotalCount;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);

	printf("USBHD HOST Test\r\n");
	USBHD_ClockCmd(RCC_USBCLKSource_PLLCLK_1Div5,ENABLE);
	pHOST_RX_RAM_Addr = RxBuffer;
	pHOST_TX_RAM_Addr = TxBuffer;
	USB_HostInit();
	CHRV3LibInit();
	printf( "Wait Device In\r\n" );

	while(1)
	{
		s = ERR_SUCCESS;

		if ( R8_USB_INT_FG & RB_UIF_DETECT )
		{
			R8_USB_INT_FG = RB_UIF_DETECT ;

			s = AnalyzeRootHub( );
			if ( s == ERR_USB_CONNECT )
			{
				printf( "New Device In\r\n" );
				FoundNewDev = 1;
			}
			if( s == ERR_USB_DISCON )
			{
				printf( "Device Out\r\n" );
			}
		}

		if ( FoundNewDev || s == ERR_USB_CONNECT )
		{
			FoundNewDev = 0;
			Delay_Ms( 200 );
			s = InitRootDevice( Com_Buffer );
			if ( s == ERR_SUCCESS )
			{
				// U盘操作流程：USB总线复位、U盘连接、获取设备描述符和设置USB地址、可选的获取配置描述符，之后到达此处，由CH103子程序库继续完成后续工作
				CHRV3DiskStatus = DISK_USB_ADDR;
				for ( i = 0; i != 10; i ++ )
				{
					printf( "Wait DiskReady\r\n" );
					s = CHRV3DiskReady( );                                 //等待U盘准备好
					if ( s == ERR_SUCCESS )
					{
						break;
					}
					else
					{
						printf("%02x\r\n",(UINT16)s);
					}
					Delay_Ms( 50 );
				}

			  if ( CHRV3DiskStatus >= DISK_MOUNTED )
				{
					/* 读文件 */
					strcpy( (PCHAR)mCmdParam.Open.mPathName, "/C51/NEWFILE.C" );     //设置将要操作的文件路径和文件名/C51/NEWFILE.C
					s = CHRV3FileOpen( );                                      //打开文件
					if ( s == ERR_MISS_DIR || s == ERR_MISS_FILE ) {           //没有找到文件
						//创建文件演示
						printf( "Find No File And Create\r\n" );
						strcpy( (PCHAR)mCmdParam.Create.mPathName, "/NEWFILE.TXT" );          /* 新文件名,在根目录下,中文文件名 */
						s = CHRV3FileCreate( );                                        /* 新建文件并打开,如果文件已经存在则先删除后再新建 */
						mStopIfError( s );
						printf( "ByteWrite\n" );
						//实际应该判断写数据长度和定义缓冲区长度是否相符，如果大于缓冲区长度则需要多次写入
						i = sprintf( (PCHAR)buf,"Note: \xd\xa这个程序是以字节为单位进行U盘文件读写,简单演示功能。\xd\xa");  /*演示 */
						for(c=0; c<10; c++)
						{
							mCmdParam.ByteWrite.mByteCount = i;                          /* 指定本次写入的字节数 */
							mCmdParam.ByteWrite.mByteBuffer = buf;                       /* 指向缓冲区 */
							s = CHRV3ByteWrite( );                                       /* 以字节为单位向文件写入数据 */
							mStopIfError( s );
							printf("成功写入 %02X次\r\n",(UINT16)c);
						}
						//演示修改文件属性
						printf( "Modify\r\n" );
						mCmdParam.Modify.mFileAttr = 0xff;   //输入参数: 新的文件属性,为0FFH则不修改
						mCmdParam.Modify.mFileTime = 0xffff;   //输入参数: 新的文件时间,为0FFFFH则不修改,使用新建文件产生的默认时间
						mCmdParam.Modify.mFileDate = MAKE_FILE_DATE( 2015, 5, 18 );  //输入参数: 新的文件日期: 2015.05.18
						mCmdParam.Modify.mFileSize = 0xffffffff;  // 输入参数: 新的文件长度,以字节为单位写文件应该由程序库关闭文件时自动更新长度,所以此处不修改
						i = CHRV3FileModify( );   //修改当前文件的信息,修改日期
						mStopIfError( i );
						printf( "Close\r\n" );
						mCmdParam.Close.mUpdateLen = 1;     /* 自动计算文件长度,以字节为单位写文件,建议让程序库关闭文件以便自动更新文件长度 */
						i = CHRV3FileClose( );
						mStopIfError( i );

						/* 删除某文件 */
//          printf( "Erase\n" );
//          strcpy( (PCHAR)mCmdParam.Create.mPathName, "/OLD" );  //将被删除的文件名,在根目录下
//          i = CHRV3FileErase( );  //删除文件并关闭
//          if ( i != ERR_SUCCESS ) printf( "Error: %02X\n", (UINT16)i );  //显示错误
					}
					else
					{
						//如果希望将新数据添加到原文件的尾部,可以移动文件指针
					  ///////////一、写入文件/////////////////////////////////////////
						printf( "ByteWrite\r\n" );
						mCmdParam.ByteLocate.mByteOffset = 0xffffffff;  //移到文件的尾部
						CHRV3ByteLocate( );
					  //实际应该判断写数据长度和定义缓冲区长度是否相符，如果大于缓冲区长度则需要多次写入
						i = sprintf( (PCHAR)buf,"Note: \xd\xa这个程序是以字节为单位进行U盘文件读写,简单演示功能。\xd\xa");  /*演示 */
						for(c=0; c<10; c++)
						{
							mCmdParam.ByteWrite.mByteCount = i;                          /* 指定本次写入的字节数 */
							mCmdParam.ByteWrite.mByteBuffer = buf;                       /* 指向缓冲区 */
							s = CHRV3ByteWrite( );                                       /* 以字节为单位向文件写入数据 */
							mStopIfError( s );
							printf("成功写入 %02X次\r\n",(UINT16)c);
						}

						///////////二、读取文件前N字节/////////////////////////////////////////
						TotalCount = 100;                                      //设置准备读取总长度100字节
						printf( "读出的前%d个字符是:\r\n",TotalCount );
						while ( TotalCount ) {                                 //如果文件比较大,一次读不完,可以再调用CH103ByteRead继续读取,文件指针自动向后移动
							if ( TotalCount > (MAX_PATH_LEN-1) ) c = MAX_PATH_LEN-1;/* 剩余数据较多,限制单次读写的长度不能超过 sizeof( mCmdParam.Other.mBuffer ) */
							else c = TotalCount;                                 /* 最后剩余的字节数 */
							mCmdParam.ByteRead.mByteCount = c;                   /* 请求读出几十字节数据 */
							mCmdParam.ByteRead.mByteBuffer= &buf[0];
							s = CHRV3ByteRead( );                                /* 以字节为单位读取数据块,单次读写的长度不能超过MAX_BYTE_IO,第二次调用时接着刚才的向后读 */
							TotalCount -= mCmdParam.ByteRead.mByteCount;         /* 计数,减去当前实际已经读出的字符数 */
							for ( i=0; i!=mCmdParam.ByteRead.mByteCount; i++ ) printf( "%c", mCmdParam.ByteRead.mByteBuffer[i] );  /* 显示读出的字符 */
							if ( mCmdParam.ByteRead.mByteCount < c ) {           /* 实际读出的字符数少于要求读出的字符数,说明已经到文件的结尾 */
								printf( "\r\n" );
								printf( "文件已经结束\r\n" );
								break;
							}
						}

					  ///////////三、从指定位置读取文件N字节/////////////////////////////////////////
						printf( "Close\r\n" );
						i = CHRV3FileClose( );                                    /* 关闭文件 */
						mStopIfError( i );
					}
				}
			}
		}
		Delay_Ms( 100 );  // 模拟单片机做其它事
		SetUsbSpeed( 1 );  // 默认为全速
	}
}


