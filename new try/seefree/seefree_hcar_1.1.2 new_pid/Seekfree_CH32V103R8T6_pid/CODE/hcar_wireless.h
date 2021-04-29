#ifndef _hcar_wireless_h
#define _hcar_wireless_h
#include "common.h"
#include "SEEKFREE_WIRELESS.h"
#include "board.h"

//º¯Êý
void hcar_pid_wireless(void);
void hcar_camera_wireless(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);

#endif
