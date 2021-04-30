/*
 * camera.h
 *
 *  Created on: 2021��2��21��
 *      Author: SLM
 */

#ifndef CODE_HCAR_CAMERA_H_
#define CODE_HCAR_CAMERA_H_

#define IMAGE_H 50             //��ȡͼ��ĸ߶�
#define IMAGE_W 188            //��ȡͼ��Ŀ��

#define IMG_H 60                //Ԥ�����ͼ��ĸ߶�
#define IMG_W 80                //Ԥ�����ͼ��Ŀ��

extern uint8 image[IMAGE_H][IMAGE_W]; //���յ���ͼ��
extern uint8 img[IMG_H][IMG_W];     //Ԥ������ͼ��

extern int8 new_l_edge[IMG_H];
extern int8 new_r_edge[IMG_H];


#endif /* CODE_HCAR_CAMERA_H_ */
