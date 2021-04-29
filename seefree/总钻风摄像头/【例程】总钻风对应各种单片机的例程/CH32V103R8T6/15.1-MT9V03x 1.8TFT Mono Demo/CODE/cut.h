/*
 * cut.h
 *
 *  Created on: 2021年2月21日
 *      Author: SLM
 */

#ifndef CODE_CUT_H_
#define CODE_CUT_H_

#include "common.h"
#include "hcar_camera.h"

extern void get_cut_array();        //初始化函数
extern void cut_image_to_img();

extern uint8 img_cut_H[IMG_H];
extern uint8 img_cut_W[IMG_W];


#endif /* CODE_CUT_H_ */
