/*
 * camera.h
 *
 *  Created on: 2021年2月21日
 *      Author: SLM
 */

#ifndef CODE_HCAR_CAMERA_H_
#define CODE_HCAR_CAMERA_H_

#define IMAGE_H 50             //提取图像的高度
#define IMAGE_W 188            //提取图像的宽度

#define IMG_H 60                //预处理后图像的高度
#define IMG_W 80                //预处理后图像的宽度

extern uint8 image[IMAGE_H][IMAGE_W]; //接收到的图像
extern uint8 img[IMG_H][IMG_W];     //预处理后的图像

extern int8 new_l_edge[IMG_H];
extern int8 new_r_edge[IMG_H];


#endif /* CODE_HCAR_CAMERA_H_ */
