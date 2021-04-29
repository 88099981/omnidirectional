#ifndef _hcar_init_h
#define _hcar_init_h
#include "common.h"
#define BEEP_PIN   D2       //¶¨Òå·äÃùÆ÷Òı½Å

//extern vuint8 ok_sign;
extern vuint8 turn_sign;
extern vuint8 camera_down;
extern int16_t host_encoder_left;//1ÂÖ
extern int16_t host_encoder_right;//2ÂÖ
extern int16 aim_x, aim_y, aim_z;
extern int16 targetSpeed1, targetSpeed2, targetSpeed3, targetSpeed4;
extern uint16 encoder_str[];
extern int32 turn_sum;
extern vuint8 turn_over;
extern vuint8 sancha_first;
extern vuint8 zuo_yuanhuan_flag,you_yuanhuan_flag;
extern uint16 bb_time;
extern int16 fuhe_speed;
extern vuint8 fuhe_sancha;
extern int16 ad_all;
extern int16 fuhe_ad_all;
extern int16 judge_lr;
extern uint16 no_time;
extern uint8 zebra_first;
extern uint16 no_time1;

//º¯Êı
void hcar_init(void);
#endif
