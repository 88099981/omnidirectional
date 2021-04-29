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
#ifndef _LQ_FLASH_h
#define _LQ_FLASH_h

#define FLASH_BASE_ADDR             (0x08000000)   //FALSH起始地址
#define FLASH_PAGE_SIZE             1024           //1K BYTE
#define FLASH_SECTION_SIZE          (FLASH_PAGE_SIZE*4) //4K BYTE
//共16扇区，每扇区4K。
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define PAGE_WRITE_START_ADDR  ((uint32_t)0x0800F000) /* Start from 60K */
#define PAGE_WRITE_END_ADDR    ((uint32_t)0x08010000) /* End at 63K */

#define FLASH_PAGES_TO_BE_PROTECTED FLASH_WRProt_Pages60to63

typedef enum
{
    FLASH_SECTION_00,
    FLASH_SECTION_01,
    FLASH_SECTION_02,
    FLASH_SECTION_03,
    FLASH_SECTION_04,
    FLASH_SECTION_05,
    FLASH_SECTION_06,
    FLASH_SECTION_07,
    FLASH_SECTION_08,
    FLASH_SECTION_09,
    FLASH_SECTION_10,
    FLASH_SECTION_11,
    FLASH_SECTION_12,
    FLASH_SECTION_13,
    FLASH_SECTION_14,
    FLASH_SECTION_15,
}FLASH_SEC_enum;

typedef enum
{
    FLASH_PAGE_0,
    FLASH_PAGE_1,
    FLASH_PAGE_2,
    FLASH_PAGE_3,
}FLASH_PAGE_enum;

uint8_t Flash_Check(FLASH_SEC_enum sector_num, FLASH_PAGE_enum page_num);
uint8_t Flash_Erase_Page(FLASH_SEC_enum sector_num, FLASH_PAGE_enum page_num);
void Flash_Page_Read (FLASH_SEC_enum sector_num, FLASH_PAGE_enum page_num, uint32_t *buf, uint16_t len);
uint8_t Flash_Page_Write(FLASH_SEC_enum sector_num, FLASH_PAGE_enum page_num, const uint32_t *buf, uint16_t len);
void Test_EEPROM(void);
#endif
