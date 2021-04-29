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
#include "ch32v10x_misc.h"
#include "ch32v10x_flash.h"
#include "LQ_FLASH.h"
/*************************************************************************
*  函数名称：uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  功能说明：校验FLASH是否有数据
*  参数说明：sector_no  需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
*            page_no   当前扇区页的编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
*  函数返回：返回1有数据，返回0没有数据，如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
*  修改时间：2020年3月10日
*  备    注：Flash_Check(FLASH_SECTION_00,FLASH_PAGE_0);
*************************************************************************/
uint8_t Flash_Check (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no)
{
    uint16_t temp_loop;
    uint32_t flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no)); // 提取当前 Flash 地址

    for(temp_loop = 0; temp_loop < FLASH_PAGE_SIZE; temp_loop+=4)      // 循环读取 Flash
    {
        if( (*(__IO uint32_t*) (flash_addr+temp_loop)) != 0xFFFFFFFF ) //  0xFFFFFFFF为空
            return 1;
    }
    return 0;
}
/*************************************************************************
*  函数名称：uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  功能说明：擦除一页数据
*  参数说明：sector_no 需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
*            page_no   当前扇区页的编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
*  函数返回：返回1有表示失败  返回0表示成功
*  修改时间：2020年3月10日
*  备    注：Flash_Erase_Page(FLASH_SECTION_00,FLASH_PAGE_0);
*************************************************************************/
uint8_t Flash_Erase_Page (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32_t flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no));     // 当前 Flash 地址

    FLASH_Unlock();                                                                                     // 解锁 Flash
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);                           // 清除操作标志
    gFlashStatus = FLASH_ErasePage(flash_addr);                                                         // 擦除
    FLASH_ClearFlag(FLASH_FLAG_EOP );                                                                   // 清楚操作标志
    FLASH_Lock();                                                                                       // 锁定 Flash
    if(gFlashStatus != FLASH_COMPLETE)                                                                  // 判断操作是否成功
        return 1;
    return 0;
}
/*************************************************************************
*  函数名称：uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  功能说明：读取一页数据
*  参数说明：sector_no 需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
*            page_no   当前扇区页的编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
*            buf             需要写入的数据地址   传入的数组类型必须为u32
*            len             需要写入的数据长度   参数范围 1-256
*  函数返回：返回1有表示失败  返回0表示成功
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void Flash_Page_Read (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no, uint32_t *buf, uint16_t len)
{
    uint16_t temp_loop = 0;
    uint32_t flash_addr = 0;
    //flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no));            // 计算当前 Flash地址
    flash_addr = ((uint32_t)0x0800F000);                                                              // 起始地址

    for(temp_loop = 0; temp_loop < len; temp_loop++)                                                  // 根据指定长度读取
    {
        *buf++ = *(__IO uint32_t*)(flash_addr+temp_loop*4);                                           // 循环读取 Flash 的值
    }
}
/*************************************************************************
*  函数名称：uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  功能说明：写入一页数据
*  参数说明：sector_no 需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
*           page_no  当前扇区页的编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
*           buf      需要写入的数据地址   传入的数组类型必须为u32
*           len      需要写入的数据长度   参数范围 1-256
*  函数返回：返回1有表示失败  返回0表示成功
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
uint8_t Flash_Page_Write (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no, const uint32_t *buf, uint16_t len)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32_t flash_addr = 0;

    flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no));            // 计算当前 Flash地址

    if(Flash_Check(sector_no, page_no))                                                               // 判断是否有数据 这里是冗余的保护 防止有人没擦除就写入
        Flash_Erase_Page(sector_no, page_no);                                                         // 擦除这一页

    FLASH_Unlock();                                                                                   // 解锁 Flash
    while(len--)                                                                                      // 根据长度
    {
        gFlashStatus = FLASH_ProgramWord(flash_addr, *buf++);                                         // 按字 32bit 写入数据
        if(gFlashStatus != FLASH_COMPLETE)                                                            // 反复确认操作是否成功
            return 1;
        flash_addr += 4;                                                                              // 地址自增
    }
    FLASH_Lock();                                                                                     // 锁定 Flash
    return 0;
}










/* Global define */


/* Global Variable */
uint32_t EraseCounter = 0x0, Address = 0x0;
uint16_t Data = 0xAAAA;
uint32_t WRPR_Value = 0xFFFFFFFF, ProtectedPages = 0x0;
uint32_t NbrOfPage;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

volatile TestStatus MemoryProgramStatus = PASSED;
volatile TestStatus MemoryEraseStatus = PASSED;
/*******************************************************************************
* Function Name  : Flash_Test
* Description    : Flash Program Test.
* Input          : None
* Return         : None
*******************************************************************************/
void Test_EEPROM(void)
{
  FLASH_Unlock();
  WRPR_Value = FLASH_GetWriteProtectionOptionByte();

  NbrOfPage = (PAGE_WRITE_END_ADDR - PAGE_WRITE_START_ADDR) / FLASH_PAGE_SIZE;
/*
  if ( (WRPR_Value & FLASH_PAGES_TO_BE_PROTECTED) != 0x00)
  {
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP|FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);

    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
      FLASHStatus = FLASH_ErasePage(PAGE_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
      if(FLASHStatus != FLASH_COMPLETE)
      {
        printf("FLASH Erase ERR at Page%d\r\n",EraseCounter+60);
      }
      printf("FLASH Erase Page%d...\r\n",EraseCounter+60);
    }

    Address = PAGE_WRITE_START_ADDR;
    printf("Erase Cheking...\r\n");
    while((Address < PAGE_WRITE_END_ADDR) && (MemoryEraseStatus != FAILED))
    {
      if((*(__IO uint16_t*) Address) != 0xFFFF)
      {
        MemoryEraseStatus = FAILED;
       }
        Address += 2;
    }
        if(MemoryEraseStatus == FAILED)
    {
      printf("Erase Flash FAIL!\r\n");
      printf("\r\n");
    }
    else
    {
      printf("Erase Flash PASS!\r\n");
      printf("\r\n");
    }

    Address = PAGE_WRITE_START_ADDR;
    printf("Programing...\r\n");
    while((Address < PAGE_WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramHalfWord(Address, Data);
      Address = Address + 2;
    }

    Address = PAGE_WRITE_START_ADDR;
    printf("Program Cheking...\r\n");
    while((Address < PAGE_WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
    {
      if((*(__IO uint16_t*) Address) != Data)
      {
        MemoryProgramStatus = FAILED;
       }
        Address += 2;
     }
     if(MemoryProgramStatus == FAILED)
     {
       printf("Memory Program FAIL!\r\n");
       printf("\r\n");
     }
     else
     {
       printf("Memory Program PASS!\r\n");
       printf("\r\n");
     }

   }
   else
   {
     MemoryProgramStatus = FAILED;
     printf("Error to program the flash : The desired pages are write protected\r\n");
   }
*/
   FLASH_Lock();

}



