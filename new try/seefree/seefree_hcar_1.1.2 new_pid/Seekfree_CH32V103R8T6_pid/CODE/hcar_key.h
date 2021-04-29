#ifndef _hcar_key_h
#define _hcar_key_h
#include"common.h"

//定义按键引脚
#define KEY1    C8
#define KEY2    C9
#define KEY3    B2

//开关状态变量
extern vuint8 key1_status;
extern vuint8 key2_status;
extern vuint8 key3_status;

//上一次开关状态变量
extern vuint8 key1_last_status;
extern vuint8 key2_last_status;
extern vuint8 key3_last_status;

//开关标志位
extern vuint8 key1_flag;
extern vuint8 key2_flag;
extern vuint8 key3_flag;

//函数
void hcar_key_init(void);
void hcar_key_get(void);
#endif
