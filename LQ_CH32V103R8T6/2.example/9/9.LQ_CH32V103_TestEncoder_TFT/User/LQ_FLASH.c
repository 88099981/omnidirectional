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
#include "ch32v10x_misc.h"
#include "ch32v10x_flash.h"
#include "LQ_FLASH.h"
/*************************************************************************
*  �������ƣ�uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  ����˵����У��FLASH�Ƿ�������
*  ����˵����sector_no  ��Ҫд����������   ������Χ FLASH_SECTION_00-FLASH_SECTION_15
*            page_no   ��ǰ����ҳ�ı��        ������Χ FLASH_PAGE_0-FLASH_PAGE_3
*  �������أ�����1�����ݣ�����0û�����ݣ������Ҫ�������ݵ�����д���µ�������Ӧ�ö������������в�������
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��Flash_Check(FLASH_SECTION_00,FLASH_PAGE_0);
*************************************************************************/
uint8_t Flash_Check (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no)
{
    uint16_t temp_loop;
    uint32_t flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no)); // ��ȡ��ǰ Flash ��ַ

    for(temp_loop = 0; temp_loop < FLASH_PAGE_SIZE; temp_loop+=4)      // ѭ����ȡ Flash
    {
        if( (*(__IO uint32_t*) (flash_addr+temp_loop)) != 0xFFFFFFFF ) //  0xFFFFFFFFΪ��
            return 1;
    }
    return 0;
}
/*************************************************************************
*  �������ƣ�uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  ����˵��������һҳ����
*  ����˵����sector_no ��Ҫд����������   ������Χ FLASH_SECTION_00-FLASH_SECTION_15
*            page_no   ��ǰ����ҳ�ı��        ������Χ FLASH_PAGE_0-FLASH_PAGE_3
*  �������أ�����1�б�ʾʧ��  ����0��ʾ�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��Flash_Erase_Page(FLASH_SECTION_00,FLASH_PAGE_0);
*************************************************************************/
uint8_t Flash_Erase_Page (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32_t flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no));     // ��ǰ Flash ��ַ

    FLASH_Unlock();                                                                                     // ���� Flash
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);                           // ���������־
    gFlashStatus = FLASH_ErasePage(flash_addr);                                                         // ����
    FLASH_ClearFlag(FLASH_FLAG_EOP );                                                                   // ���������־
    FLASH_Lock();                                                                                       // ���� Flash
    if(gFlashStatus != FLASH_COMPLETE)                                                                  // �жϲ����Ƿ�ɹ�
        return 1;
    return 0;
}
/*************************************************************************
*  �������ƣ�uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  ����˵������ȡһҳ����
*  ����˵����sector_no ��Ҫд����������   ������Χ FLASH_SECTION_00-FLASH_SECTION_15
*            page_no   ��ǰ����ҳ�ı��        ������Χ FLASH_PAGE_0-FLASH_PAGE_3
*            buf             ��Ҫд������ݵ�ַ   ������������ͱ���Ϊu32
*            len             ��Ҫд������ݳ���   ������Χ 1-256
*  �������أ�����1�б�ʾʧ��  ����0��ʾ�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
void Flash_Page_Read (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no, uint32_t *buf, uint16_t len)
{
    uint16_t temp_loop = 0;
    uint32_t flash_addr = 0;
    //flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no));            // ���㵱ǰ Flash��ַ
    flash_addr = ((uint32_t)0x0800F000);                                                              // ��ʼ��ַ

    for(temp_loop = 0; temp_loop < len; temp_loop++)                                                  // ����ָ�����ȶ�ȡ
    {
        *buf++ = *(__IO uint32_t*)(flash_addr+temp_loop*4);                                           // ѭ����ȡ Flash ��ֵ
    }
}
/*************************************************************************
*  �������ƣ�uint8_t Flash_Check (FLASH_SEC_eno sector_no, FLASH_PAGE_eno page_no)
*  ����˵����д��һҳ����
*  ����˵����sector_no ��Ҫд����������   ������Χ FLASH_SECTION_00-FLASH_SECTION_15
*           page_no  ��ǰ����ҳ�ı��        ������Χ FLASH_PAGE_0-FLASH_PAGE_3
*           buf      ��Ҫд������ݵ�ַ   ������������ͱ���Ϊu32
*           len      ��Ҫд������ݳ���   ������Χ 1-256
*  �������أ�����1�б�ʾʧ��  ����0��ʾ�ɹ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
uint8_t Flash_Page_Write (FLASH_SEC_enum sector_no, FLASH_PAGE_enum page_no, const uint32_t *buf, uint16_t len)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32_t flash_addr = 0;

    flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_no+FLASH_PAGE_SIZE*page_no));            // ���㵱ǰ Flash��ַ

    if(Flash_Check(sector_no, page_no))                                                               // �ж��Ƿ������� ����������ı��� ��ֹ����û������д��
        Flash_Erase_Page(sector_no, page_no);                                                         // ������һҳ

    FLASH_Unlock();                                                                                   // ���� Flash
    while(len--)                                                                                      // ���ݳ���
    {
        gFlashStatus = FLASH_ProgramWord(flash_addr, *buf++);                                         // ���� 32bit д������
        if(gFlashStatus != FLASH_COMPLETE)                                                            // ����ȷ�ϲ����Ƿ�ɹ�
            return 1;
        flash_addr += 4;                                                                              // ��ַ����
    }
    FLASH_Lock();                                                                                     // ���� Flash
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



