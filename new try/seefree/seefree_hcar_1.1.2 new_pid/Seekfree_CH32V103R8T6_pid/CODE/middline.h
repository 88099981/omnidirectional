/*
 * middline.h
 *
 *  Created on: 2021年2月21日
 *      Author: SLM
 */

#ifndef CODE_MIDDLINE_H_
#define CODE_MIDDLINE_H_

#include "common.h"
#include "hcar_camera.h"

#define edge_wide  35
#define col_end   77
#define col_start   3
#define row_end   57

#define INV_A  3.0972
#define INV_B  -0.060336
#define INV_C  -115.5153
#define INV_D  4.4242e-14
#define INV_E   -1.3374
#define INV_F  89.3224
#define INV_G  -1.9155e-15
#define INV_H  0.045922
//k1=-1.3091e-05;
//k2=-1.2545e-05;
//rot = [ 2.4079, 0.2282, -211.9439; 0.15049, -3.266, 154.3089; 0.0050164, 0.10982, 1 ]
//#define edge_clean();           memset(mid,-1,sizeof(mid));


typedef struct
{
      float x;
      float y;
} Site_xy;         //定义浮点型结构体

typedef struct
{
      int8 x;
      int8 y;
} Site_xy1;        //定义整型结构体

typedef struct
{
      float x;
      float y;
} result;
//#define edge_clean();           memset(mid,-1,sizeof(mid));

extern void road_seek ();
extern void wipe_out();
extern void get_new_edge();
extern void three_fork();
extern void zebra_crossing();//识别斑马线函数
extern void set_middline_black();
extern void initialize_variables();
extern Site_xy get_inv_img(int8 xxx ,int8 yyy);

extern int8 edge_i;
extern int8 edge_j;
extern int8 l_end_row;
extern int8 l_edge_col_old;
extern int8 r_edge_col_old;

extern vuint8 l_get_flag;
extern vuint8 r_get_flag;

extern vuint8 l_turn_flag;
extern vuint8 r_turn_flag;
extern vuint8 break_flag;
extern vuint8 zebra_flag;

extern int8 l_link_num;
extern int8 r_link_num;
extern int8 l_lost_num;
extern int8 r_lost_num;
extern int8 temp_num;
extern int8 temp_row;
//extern int16 l_lost_flag;
//extern int16 r_lost_flag;
extern vuint8 l_search_end_flag;
extern vuint8 r_search_end_flag;
extern vuint8 l_trend_in_falg;


extern int8 mid[IMG_H];
//extern int16 correct[IMG_H];
extern uint8 img[IMG_H][IMG_W];

extern Site_xy X_deviation;
//extern Site_xy p_deviation;
//extern Site_xy q_deviation;

extern float angle;

#endif /* CODE_MIDDLINE_H_ */
