#ifndef _hcar_init_h
#define _hcar_init_h
#include "common.h"

//extern vuint8 ok_sign;
extern vuint8 turn_sign;
extern int16_t host_encoder_left;//1��
extern int16_t host_encoder_right;//2��
extern int16 aim_x, aim_y, aim_z;
extern int16 targetSpeed1, targetSpeed2, targetSpeed3, targetSpeed4;
extern uint16 encoder_str[];
//����
void hcar_init(void);
#endif
