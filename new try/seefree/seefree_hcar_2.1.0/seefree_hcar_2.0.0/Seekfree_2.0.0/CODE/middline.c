/*
 * middline.c
 *
 *  Created on: 2021年2月21日
 *      Author: SLM
 */

#include "middline.h"
#include "ostu.h"
#include "hcar_camera.h"
#include "hcar_init.h"
#include "hcar_adc.h"

#define DEVIATION_MAX   7    //最大偏差量
#define DEVIATION_MIN   -7      //最小偏差量
#define EDGE_START 57       //搜索边沿起始
#define CAR_HEAD    40      //车头坐标38
#define ALOW_SCOPE  5       //准许边沿偏差范围
#define EDGE_END   8        //搜索终止
#define PI 3.14159265
#define edge_clean();           memset(mid,-1,sizeof(mid));





int8 new_l_edge[IMG_H] = {0};
int8 new_r_edge[IMG_H] = {IMG_W};
//int16 correct[IMG_H];

int8 edge_i;
int8 edge_j;
int8 l_end_row;
int8 l_edge_col_old;
int8 r_edge_col_old;
vuint8 l_get_flag;
vuint8 r_get_flag;
//int16 l_lost_flag;
//int16 r_lost_flag;

//元素识别标志位
vuint8 break_flag;                  //出界停止标志位
vuint8 zebra_flag;
vuint8 three_fork_flag;
vuint8 l_turn_flag;
vuint8 r_turn_flag;
vuint8 cross_flag;
int8 l_link_num;
int8 r_link_num;
int8 l_lost_num;
int8 r_lost_num;
int8 temp_num;
int8 temp_row;
uint8 r_loop_able;
uint8 l_loop_able;
uint8 three_folk;
uint8 three_fork_num=0;
vuint8 l_search_end_flag;
vuint8 r_search_end_flag;
vuint8 l_trend_in_falg;

int8 mid[IMG_H];
uint8 img[IMG_H][IMG_W];

//vuint8 e_i=0, e_j=0;
//int8 x_error[5]={0}, z_error[5]={0};
//int16 x_sum=0, z_sum=0;

vint16 sum_x1=0;
vint16 sum_x2=0;
vint16 z_error=0;
vint16 camera_speed=0;

//float x_filter1=0.0, z_filter2=0.0;
//
//Site_xy X_deviation;
//float angle=0.0;

//float kp_x;
//float ki_x=0;
//float kd_x;

float kp_z;
//float ki_z=0;
float kd_z;

//float lasttime_x=0.0;
float lasttime_z=0.0;
void edge_clean_2()
{
    int8 i;
    for (i = 0; i < IMG_H; i++)
    {
        new_l_edge[i] = -1;
        new_r_edge[i] = -1;
    }
}

//深度优先搜索，八邻通域
void DFS_GET_LEDGE()
{
    int8 hnum[8] = {-1,-1,-1,0,0,1,1,1};//对应行h上、上右、上左、中、下
    //int8 wnum[8] = {0,1,-1,1,-1,0,1,-1};//对应列w
    int8 wnum[8] = { 0,-1,1,-1,1,0,-1,1 };//对应列w
    uint8 visit_color = 200;

    int8 top = -1, k, MAX_NODE = 8, ii, jj;

    uint8 stack_h[IMG_W], stack_w[IMG_W];
    //预存入第一个,提取根节点
    int8 j,b_num=0;
    for (j = 0; j < IMG_W; j++)
    {
        /*if (j == 0 && img[IMG_H - 1][j] == White)//十字中
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }
        else if (img[IMG_H - 1][j] == Black && img[IMG_H - 1][j + 1] == White &&
            img[IMG_H - 1][j + 2] == White)//正常情况下
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }
        else
        {
            //return 0;
        }*/
        if (j == 0 && img[IMG_H - 1][j] == White)//十字中
                {
                    for(edge_j=j;edge_j<IMG_W/2;edge_j++)
                    {
                        if(img[IMG_H - 2][edge_j]==Black)
                        {
                            b_num++;
                        }
                    }
                    if(b_num<10)
                    {
                        top++;
                        stack_h[top] = IMG_H - 1;
                        stack_w[top] = j;
                        break;
                    }
                    /*top++;
                    stack_h[top] = IMG_H - 1;
                    stack_w[top] = j;
                    break;*/
                }
                else if (img[IMG_H - 1][j] == Black && img[IMG_H - 1][j + 1] == White &&
                    img[IMG_H - 1][j + 2] == White)//正常情况下
                {
                    top++;
                    stack_h[top] = IMG_H - 1;
                    stack_w[top] = j;
                    break;
                }
        /*if (img[IMG_H - 1][j] == Black && img[IMG_H - 1][j + 1] == White &&
            img[IMG_H - 1][j + 2] == White)//正常情况下
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }
        else if (j == 0 && img[IMG_H - 1][j] == White)//十字中
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }*/
    }
    //开始
    edge_i = IMG_H - 1;//从下往上
    while (top != -1 && edge_i > 1)//能找到根节点
    {
        //出栈
        ii = stack_h[top];//记载根节点的行和列
        jj = stack_w[top];

        for (k = 0; k < MAX_NODE; k++)//MAX_NODE=8
        {
            edge_i = ii + hnum[k];
            edge_j = jj + wnum[k];

            if (edge_i >= 0 && edge_i < IMG_H &&edge_j>=0 && edge_j < IMG_W &&
                img[edge_i][edge_j] != visit_color&&
                ((img[edge_i][edge_j - 1] == Black && img[edge_i][edge_j + 1] == White)||
                (img[edge_i - 1][edge_j] == Black && img[edge_i + 1][edge_j] == White)||
                (img[edge_i + 1][edge_j] == Black && img[edge_i - 1][edge_j] == White)||
                (edge_j == 0 && img[edge_i][edge_j+1] == White )||
                ((img[edge_i][edge_j + 1] == Black || img[edge_i][edge_j + 1] == visit_color ) && img[edge_i][edge_j - 1] == White && img[edge_i+1][edge_j] == Black)
                ))
            {
                if (k == 0 || k == 1 || k == 2)
                {
                    if (edge_j>0 && edge_j < IMG_W -1 && (img[edge_i][edge_j - 1] == Black) && img[edge_i][edge_j + 1] == White)
                    {//黑黑白
                        img[edge_i][edge_j] = visit_color;//标记访问
                        if (new_l_edge[edge_i] == -1)//未被赋值,还没找到左边沿
                        {
                            new_l_edge[edge_i] = edge_j;
                            stack_h[++top]=edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }

                            break;

                        }
                        else
                        {
                            ///
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }

                            break;
                        }
                    }
                    else if (edge_i>0 && edge_i <IMG_H-1 && ((img[edge_i - 1][edge_j] == Black && img[edge_i + 1][edge_j] == White)||
                    (img[edge_i + 1][edge_j] == Black   && img[edge_i - 1][edge_j] == White))
                    )
                    {//纵向黑黑白或者纵向白黑黑
                        img[edge_i][edge_j] = visit_color;//标记访问
                        //不更新作为过渡
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                    else if (edge_j == 0 && img[edge_i][edge_j+1] == White)
                    {//最边上的点，往右一格也是白
                        img[edge_i][edge_j] = visit_color;//标记访问
                        new_l_edge[edge_i] = edge_j;
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                }
                else if (k ==3 || k == 4)
                {
                    if(edge_i != IMG_H - 1)
                    if (edge_i>0 && edge_i<IMG_H-1 && ((img[edge_i - 1][edge_j] == Black && img[edge_i + 1][edge_j] == White)||
                    (img[edge_i+1][edge_j ] == Black && img[edge_i - 1][edge_j] == White))
                    )
                    {//纵向黑黑白或者纵向白黑黑
                        img[edge_i][edge_j] = visit_color;//标记访问
                                                          //不更新作为过渡
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                }
                else if (k == 5 || k == 6 || k == 7)
                {
                    if (edge_j>0 && edge_j < IMG_W -1 && (img[edge_i][edge_j - 1] == Black)&& img[edge_i][edge_j + 1] == White)
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                        if (new_l_edge[edge_i] == -1)//未被赋值
                        {
                            new_l_edge[edge_i] = edge_j;//当出现黑黑白时
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }
                            break;

                        }
                        else
                        {
                            ///
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }
                            break;
                        }
                    }
                    ////
                    else if (edge_i>0 && edge_i<IMG_H-1 && edge_j>0 && edge_j < IMG_W -1 && ((img[edge_i - 1][edge_j] == Black && img[edge_i + 1][edge_j] == White)||
                    (img[edge_i + 1][edge_j] == Black && img[edge_i - 1][edge_j] == White)||
                    ((img[edge_i][edge_j + 1] == Black || img[edge_i][edge_j + 1] == visit_color )&& img[edge_i][edge_j - 1] == White && img[edge_i+1][edge_j] == Black))
                    )
                    {   //纵向黑黑白或者纵向白黑黑或横向白黑黑
                        img[edge_i][edge_j] = visit_color;//标记访问
                                                          //当纵向出现黑黑白或白黑黑时不更新作为过渡
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;
                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                    else if (edge_j == 0 && img[edge_i][edge_j+1] == White)
                    {   //最边上的点，往右一格也是白
                        img[edge_i][edge_j] = visit_color;//标记访问
                        new_l_edge[edge_i] = edge_j;//出现黑白时更新
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }
                        break;
                    }
                }



            }
        }
        if (k == MAX_NODE)
        {
            top--;
        }
    }
}
//r
void DFS_GET_REDGE()
{
    int8 hnum[8] = { -1,-1,-1,0,0,1,1,1 };//对应行h上、上右、上左、中、下
    //int8 wnum[8] = { 0,1,-1,1,-1,0,1,-1 };//对应列w
    int8 wnum[8] = { 0,-1,1,-1,1,0,-1,1 };//对应列wmy
    uint8 visit_color = 200;

    int8 top = -1, k, MAX_NODE = 8, ii, jj;

    uint8 stack_h[IMG_W], stack_w[IMG_W];
    //预存入第一个
    int8 j,b_num=0;
    for (j = IMG_W-1; j > 0; j--)
    {
        /*if (j == IMG_W - 1 && img[IMG_H - 1][j] == White)
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }
        else if (img[IMG_H - 1][j] == Black && img[IMG_H - 1][j - 1] == White &&
            img[IMG_H - 1][j - 2] == White)
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }
        else
        {
            //return 0;
        }*/
        if (j == IMG_W - 1 && img[IMG_H - 1][j] == White)
        {
             for(edge_j=j;edge_j>IMG_W/2;edge_j--)
                    {
                        if(img[IMG_H - 2][edge_j]==Black)
                        {
                            b_num++;
                        }
                    }
                    if(b_num<10)
                    {
                        top++;
                        stack_h[top] = IMG_H - 1;
                        stack_w[top] = j;
                        break;
                    }
                    /*top++;
                    stack_h[top] = IMG_H - 1;
                    stack_w[top] = j;
                    break;*/
                }
                else if (img[IMG_H - 1][j] == Black && img[IMG_H - 1][j - 1] == White &&
                    img[IMG_H - 1][j - 2] == White)
                {
                    top++;
                    stack_h[top] = IMG_H - 1;
                    stack_w[top] = j;
                    break;
                }
        /*if (img[IMG_H - 1][j] == Black && img[IMG_H - 1][j - 1] == White &&
            img[IMG_H - 1][j - 2] == White)
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }
        else if (j == IMG_W - 1 && img[IMG_H - 1][j] == White)
        {
            top++;
            stack_h[top] = IMG_H - 1;
            stack_w[top] = j;
            break;
        }*/
    }
    //开始
    edge_i = IMG_H - 1;
    while (top != -1 && edge_i > 1)
    {
        //出栈
        ii = stack_h[top];
        jj = stack_w[top];

        for (k = 0; k < MAX_NODE; k++)
        {
            edge_i = ii + hnum[k];
            edge_j = jj + wnum[k];

            if (edge_i >= 0 && edge_i < IMG_H &&edge_j >= 0 && edge_j < IMG_W &&
                img[edge_i][edge_j] != visit_color &&
                ((img[edge_i][edge_j + 1] == Black && img[edge_i][edge_j - 1] == White )||
                    (img[edge_i - 1][edge_j] == White && img[edge_i + 1][edge_j] == Black )||
                    (img[edge_i + 1][edge_j] == White && img[edge_i - 1][edge_j] == Black )||
                    (edge_j == IMG_W -1 && img[edge_i][edge_j - 1] == White )||
                    (img[edge_i+1][edge_j] == Black && img[edge_i][edge_j+1]==White && img[edge_i][edge_j-1]==Black && (img[edge_i][edge_j-1]==Black || img[edge_i][edge_j-1]==visit_color))))
            {
                if (k == 0 || k == 1 || k == 2)
                {
                    if (edge_j>0 && edge_j < IMG_W -1 && img[edge_i][edge_j + 1] == Black  && img[edge_i][edge_j - 1] == White)
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                        if (new_r_edge[edge_i] == -1)//未被赋值
                        {
                            new_r_edge[edge_i] = edge_j;
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }

                            break;

                        }
                        else
                        {
                            ///
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }

                            break;
                        }
                    }
                    else if (edge_i>0 && edge_i<IMG_H-1 && ((img[edge_i - 1][edge_j] == White && img[edge_i + 1][edge_j] == Black)||
                    (img[edge_i + 1][edge_j] == White && img[edge_i - 1][edge_j] == Black))
                    )
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                                                          //不更新作为过渡
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }


                        break;
                    }
                    else if (edge_j == IMG_W -1 && img[edge_i][edge_j - 1] == White)
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                        new_r_edge[edge_i] = edge_j;
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                }
                else if (k == 3 || k == 4)
                {
                    if(edge_i != IMG_H-1)
                    if (edge_i>0 && edge_i<IMG_H-1 && ((img[edge_i - 1][edge_j] == White && img[edge_i + 1][edge_j] == Black)||
                    (img[edge_i + 1][edge_j] == White && img[edge_i - 1][edge_j] == Black))

                    )
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                                                          //不更新作为过渡
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                }
                else if (k == 5 || k == 6 || k == 7)
                {
                    if (edge_j>0 && edge_j<IMG_W-1 && img[edge_i][edge_j + 1] == Black  && img[edge_i][edge_j - 1] == White)
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                        if (new_r_edge[edge_i] == -1)//未被赋值
                        {
                            new_r_edge[edge_i] = edge_j;
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }
                            break;

                        }
                        else
                        {
                            ///
                            stack_h[++top] = edge_i;
                            stack_w[top] = edge_j;

                            if (top == IMG_W - 1)
                            {
                                top--;

                            }
                            break;
                        }
                    }
                    else if (edge_i>0 && edge_i<IMG_H-1 && edge_j>0 && edge_j<IMG_W-1 && ((img[edge_i - 1][edge_j] == White && img[edge_i + 1][edge_j] == Black)||
                    (img[edge_i + 1][edge_j] == White && img[edge_i - 1][edge_j] == Black) ||
                    (img[edge_i][edge_j+1]==White&& img[edge_i+1][edge_j] == Black && (img[edge_i][edge_j-1]==Black || img[edge_i][edge_j-1]==visit_color))
                    //(img[edge_i][edge_j-1]=Black && img[edge_i][edge_j+1]==White)
                    ))
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                                                          //不更新作为过渡
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;
                        if (top == IMG_W - 1)
                        {
                            top--;

                        }

                        break;
                    }
                    else if (edge_j == IMG_W-1 && img[edge_i][edge_j - 1] == White)
                    {
                        img[edge_i][edge_j] = visit_color;//标记访问
                        new_r_edge[edge_i] = edge_j;
                        stack_h[++top] = edge_i;
                        stack_w[top] = edge_j;

                        if (top == IMG_W - 1)
                        {
                            top--;

                        }
                        break;
                    }
                }



            }
        }
        if (k == MAX_NODE)
        {
            top--;
        }
    }
}

void judge_loop()
{
    int8  r_out_num=0,test_i=0,l_in_num=0,l_out_num=0,r_in_num=0;
    //右环
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i]!=-1 && new_l_edge[edge_i+1]!=-1)
        {
            if(new_l_edge[edge_i]>=new_l_edge[edge_i+1])
            {
                l_in_num++;//计一幅图中左边沿内倾数
            }
        }
    }
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_r_edge[edge_i]!=-1 &&new_r_edge[edge_i+1]!=-1)
        {
            if(new_r_edge[edge_i]-new_r_edge[edge_i+1]>ALOW_SCOPE)
            {
                for(test_i=edge_i ;test_i>edge_i-7;test_i--)
                {
                    if(new_r_edge[test_i]!=-1)
                    {
                        if(new_r_edge[test_i]-new_r_edge[edge_i+1]>ALOW_SCOPE)
                        {
                            r_out_num++;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(r_out_num>=4)
        {
            break;
        }
    }
    if(l_in_num>=40 && r_out_num>=4)
    {
        r_loop_able=1;
    }
    //左环 待改进
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_r_edge[edge_i]!=-1 && new_r_edge[edge_i+1]!=-1)
        {
            if(new_r_edge[edge_i]<=new_r_edge[edge_i+1])
            {
                r_in_num++;//计一幅图中右边沿内倾数
            }
        }
    }
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i]!=-1 &&new_l_edge[edge_i+1]!=-1)
        {
            if(new_l_edge[edge_i+1]-new_l_edge[edge_i]>ALOW_SCOPE)
            {
                for(test_i=edge_i ;test_i>edge_i-7;test_i--)
                {
                    if(new_l_edge[test_i]!=-1)
                    {
                        if(new_l_edge[edge_i+1]-new_l_edge[test_i]>ALOW_SCOPE)
                        {
                            l_out_num++;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(l_out_num>=4)
        {
            break;
        }
    }
    if(r_in_num>=40 && l_out_num>=4)
    {
        l_loop_able=1;
    }

}

void jude_cross()
{
    int8 r_out_num=0,r_in_num=0,l_out_num=0,l_in_num=0;
    int8 test_i=0;
    //右边沿向外突变
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_r_edge[edge_i]!=-1 &&new_r_edge[edge_i+1]!=-1 &&new_r_edge[edge_i+2]!=-1)
        {
            if(new_r_edge[edge_i]-new_r_edge[edge_i+1]>ALOW_SCOPE && new_r_edge[edge_i]-new_r_edge[edge_i+2]>ALOW_SCOPE)
            {
                for(test_i=edge_i ;test_i>edge_i-7;test_i--)
                {
                    if(new_r_edge[test_i]!=-1)
                    {
                        if(new_r_edge[test_i]-new_r_edge[edge_i+1]>ALOW_SCOPE)
                        {
                            r_out_num++;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(r_out_num>=4)
        {
            break;
        }
    }
    //左边沿向外突变
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i]!=-1 &&new_l_edge[edge_i+1]!=-1 && new_l_edge[edge_i+2]!=-1)
        {
            if(new_l_edge[edge_i+1]-new_l_edge[edge_i]>ALOW_SCOPE && new_l_edge[edge_i+2]-new_l_edge[edge_i]>ALOW_SCOPE)
            {
                for(test_i=edge_i ;test_i>edge_i-7;test_i--)
                {
                    if(new_l_edge[test_i]!=-1)
                    {
                        if(new_l_edge[edge_i+1]-new_l_edge[test_i]>ALOW_SCOPE)
                        {
                            l_out_num++;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(l_out_num>=4)
        {
            break;
        }
    }
    //右边沿向内突变
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_r_edge[edge_i-1]!=-1 && new_r_edge[edge_i]!=-1 &&new_r_edge[edge_i+1]!=-1)
        {
            if(new_r_edge[edge_i+1]-new_r_edge[edge_i]>ALOW_SCOPE && new_r_edge[edge_i+1]-new_r_edge[edge_i-1]>ALOW_SCOPE)
            {
                for(test_i=edge_i+1 ;test_i<edge_i+7;test_i++)
                {
                    if(new_r_edge[test_i]!=-1)
                    {
                        if(new_r_edge[test_i]-new_r_edge[edge_i]>ALOW_SCOPE)
                        {
                            r_in_num++;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(r_in_num>=4)
        {
            break;
        }
    }
    //左边沿向内突变
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i-1]!=-1 && new_l_edge[edge_i]!=-1 &&new_l_edge[edge_i+1]!=-1)
        {
            if(new_l_edge[edge_i]-new_l_edge[edge_i+1]>ALOW_SCOPE && new_l_edge[edge_i-1]-new_l_edge[edge_i+1]>ALOW_SCOPE)
            {
                for(test_i=edge_i+1 ;test_i<edge_i+7;test_i++)
                {
                    if(new_l_edge[test_i]!=-1)
                    {
                        if(new_l_edge[edge_i]-new_l_edge[test_i]>ALOW_SCOPE)
                        {
                            l_in_num++;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(l_in_num>=4)
        {
            break;
        }
    }
    //考虑只有两个下拐点，两个上拐点或者四个拐点或左下丢边或右下丢边
    if((l_out_num>=4 && r_out_num>=4 ) ||
     (l_in_num>=4 && r_in_num>=4) ||
     (l_out_num>=4 && r_out_num>=4 && l_in_num>=4 && r_in_num>=4) ||
     (l_out_num<4 && r_out_num>=4 && l_in_num>=4 && r_in_num>=4) ||
     (l_out_num>=4 && r_out_num<4 && l_in_num>=4 && r_in_num>=4))
     {
        cross_flag=1;
     }


}

void get_new_edge2()
{
    edge_clean_2();
    initialize_variables();
    DFS_GET_LEDGE();
    DFS_GET_REDGE();
    judge_loop();
    zebra_crossing();
    jude_cross();
    three_fork();
    //search_cross();
    //cross_connect();
}

void show_edge()
{
    uint8 i;
    for (i = EDGE_START; i >= EDGE_END; i--)
    {
        if(new_l_edge[i] != -1)
        img[i][new_l_edge[i] + 2] = 100;
        if(new_r_edge[i] !=-1 )
        img[i][new_r_edge[i] - 2] = 100;
        /*if(new_r_edge[i] !=-1 && new_l_edge[i] != -1)
        {
            uint8 mid = (new_r_edge[i]+new_l_edge[i])/2;
            //img[i][mid] = 100;
        }*/
    }
}

void get_middline()
{
    float k=2.0;
    for(edge_i=EDGE_START;edge_i>EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i] == -1 || new_r_edge[edge_i] ==-1)
        {
            continue;
        }
        if(new_l_edge[edge_i]<=new_r_edge[edge_i] && new_r_edge[edge_i]<IMG_W)
        {
            if(new_l_edge[edge_i]==0 && new_r_edge[edge_i]<IMG_W-1)//左弯道<--
            {
                l_turn_flag=1;
                if(edge_i>=EDGE_START-3)
                {
                    mid[edge_i] = (new_r_edge[edge_i]) / 2;
                }
                else if(new_r_edge[edge_i+1]!=-1)
                {
                    mid[edge_i]=mid[edge_i+1]+k*(new_r_edge[edge_i]-new_r_edge[edge_i+1]);
                    if(mid[edge_i]<0)
                    {
                        mid[edge_i]=0;
                    }
                }
            }
            else if(new_r_edge[edge_i]==IMG_W-1 && new_l_edge[edge_i]>0)//右弯道--->
            {
                r_turn_flag=1;
                if(edge_i>=EDGE_START-3)
                {
                    mid[edge_i] = (new_l_edge[edge_i]+new_r_edge[edge_i]) / 2;
                }
                else if(new_l_edge[edge_i+1]!=-1)
                {
                    mid[edge_i]=mid[edge_i+1]+k*(new_l_edge[edge_i]-new_l_edge[edge_i+1]);
                    if(mid[edge_i]>79)
                    {
                        mid[edge_i]=79;
                    }
                }
            }
            else
            {
                mid[edge_i]=(new_l_edge[edge_i]+new_r_edge[edge_i]) / 2;
            }
            img[edge_i][mid[edge_i]]=Black;
        }
    }
}


void initialize_variables()
{
    l_link_num=0;
    r_link_num=0;
    l_lost_num=0;
    r_lost_num=0;
    l_get_flag=0;
    r_get_flag=0;
    l_search_end_flag=0;
    r_search_end_flag=0;
    l_trend_in_falg=0;

    r_loop_able=0;
    l_loop_able=0;

    l_get_flag=0;
    r_get_flag=0;

    three_fork_flag=0;

    break_flag=0;
    cross_flag=0;
    zebra_flag=0;
    three_folk=0;

    l_edge_col_old=0;
    r_edge_col_old=IMG_W;

    l_end_row=0;

    l_turn_flag=0;
    r_turn_flag=0;


}

void zebra_crossing()
{
     int8 jump_num=0,link_num=0,i=0,j=0;
    /*int8 jump_num=0,link_num=0,l_in_num=0,r_in_num=0,test_i=0;
    int8 i,j,r_out_num=0,l_out_num=0;
    //对于车库在右边
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i]!=-1 && new_l_edge[edge_i+1]!=-1)
        {
            if(new_l_edge[edge_i]>=new_l_edge[edge_i+1])
            {
                l_in_num++;//计一幅图中左边沿内倾数
            }
        }
    }
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_r_edge[edge_i]!=-1 &&new_r_edge[edge_i+1]!=-1)
        {
            if(new_r_edge[edge_i]-new_r_edge[edge_i+1]>ALOW_SCOPE)
            {
                for(test_i=edge_i ;test_i>edge_i-7;test_i--)
                {
                    if(new_r_edge[test_i]!=-1)
                    {
                        if(new_r_edge[test_i]-new_r_edge[edge_i+1]>ALOW_SCOPE)
                        {
                            r_out_num++;//右边沿突变
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(r_out_num>=4)
        {
            break;
        }
    }
    //对于车库在左边
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_r_edge[edge_i]!=-1 && new_r_edge[edge_i+1]!=-1)
        {
            if(new_r_edge[edge_i]<=new_r_edge[edge_i+1])
            {
                r_in_num++;//计一幅图中左边沿内倾数
            }
        }
    }
    for(edge_i=EDGE_START;edge_i>=EDGE_END;edge_i--)
    {
        if(new_l_edge[edge_i]!=-1 &&new_l_edge[edge_i+1]!=-1)
        {
            if(new_l_edge[edge_i+1]-new_l_edge[edge_i]>ALOW_SCOPE)
            {
                for(test_i=edge_i ;test_i>edge_i-7;test_i--)
                {
                    if(new_l_edge[test_i]!=-1)
                    {
                        if(new_l_edge[edge_i+1]-new_l_edge[test_i]>ALOW_SCOPE)
                        {
                            l_out_num++;//右边沿突变
                        }
                    }
                    else
                    {
                        continue;
                    }
                }

            }
        }
        else
        {
            continue;
        }
        if(l_out_num>=4)
        {
            break;
        }
    }*/
    for(i = 57;i >= 30 ;i--)
    {
        jump_num=0;
        for(j = 15;j < 65; j++)
        {
            if(img[i][j] == Black && img[i][j+1]== White)
            {
                jump_num++;//累计黑白跳变点数目
            }
            if(jump_num >=4)
            {
                break;
            }
        }
        if(jump_num >=4 )
        {
            link_num++;
        }
        if(link_num>=3)//连续三行
        {
            //zebra_flag=1;
            break;
        }
    }
//    if(((l_in_num>40 && r_out_num>=4) || (r_in_num>40 && l_out_num>=4))&& link_num>=3)
//    {
//        zebra_flag=1;
//        //bb_time=100;
//    }
    if(link_num>=3)
        {
            zebra_flag=1;
            //bb_time=100;
        }

}




void road_seek()
{

    vint8 temp=0;
    //    int8 num=0;
        //int8 ave=0;

        sum_x1=0;
        //sum_x2=0;
//        z_error=0;

        for(temp=EDGE_START;temp>EDGE_START-9;temp--)
        {
            if(mid[temp])
            {
                sum_x1+=mid[temp]-40;
            }
        }
//        for(;temp>EDGE_START-20;temp--)
//        {
//            if(mid[temp])
//            {
//                sum_x2+=mid[temp]-40;
//            }
//        }

        z_error=sum_x1;
//        if(cross_flag==1)
//        {
//            will_cross++;
//            if(will_cross>=3)
//            {
//                camera_down=10;
//            }
//        }


        if(camera_down>0)
        {
            camera_down--;
            kp_z=0;
            kd_z=0;
            aim_y=200;
        }
        else
        {
            if(l_turn_flag==1||r_turn_flag==1)
            {
                kp_z=0.2;
                kp_ad=kp_ad_turn;
                kd_ad=kd_ad_turn;

                aim_y=200;//180
            }
            else
            {

                kp_z=KP_Z_SET;
                kd_z=KD_Z_SET;
                kp_ad=kp_ad_str;
                kd_ad=kd_ad_str;
                aim_y=220;//160
            }
        }
//        aim_x = kp_x*sum_x1 + kd_x*(sum_x1-lasttime_x);
//        ////lasttime_x=X_deviation.x;
//        lasttime_x=sum_x1;

        camera_speed = kp_z*z_error + kd_z*(z_error-lasttime_z);
        lasttime_z=z_error;


}



void three_fork()//出界停车函数
{
    uint8 b_num=0,b1_num=0,b2_num=0,help_flag=0;
    //int8 test_j=0;
        for(edge_i=59;edge_i>=0;edge_i--)
        {
            if(img[edge_i][40]==Black)
            {
                b_num++;
            }
            else if(img[edge_i][37]==Black)
            {
                b1_num++;
            }
            else if(img[edge_i][43]==Black)
            {
                b2_num++;
            }
            /*if(img[edge_i][20]==White)
            {
                w_num1++;
            }
            if(img[edge_i][60]==White)
            {
                 w_num2++;
            }*/
        }
        if(new_l_edge[57]!=-1 && new_r_edge[57]!=-1 && new_l_edge[56]!=-1 && new_r_edge[56]!=-1 &&
           new_l_edge[50]!=-1 && new_r_edge[50]!=-1  &&new_l_edge[49]!=-1 && new_r_edge[49]!=-1  )
        {
            if(new_r_edge[57]-new_l_edge[57]>55 && new_r_edge[56]-new_l_edge[56]>55 &&
                    new_r_edge[50]-new_l_edge[50]>55 && new_r_edge[49]-new_l_edge[49]>55)
            {
                help_flag=1;
            }
        }
        if((b_num>=10 ||b1_num>=10 || b2_num>=10) && help_flag==1 )
        {
            three_folk=1;

        }
        if(three_folk==1)
        {
            three_fork_num++;
        }
        else
        {
            three_fork_num=0;
        }
        if(three_fork_num>=3)
        {
            three_folk=2;
            three_fork_flag=1;
            //bb_time=50;
        }


}

/*void zebra_crossing()
{
    int8 jump_num=0,link_num=0;
    int8 i,j;
    for(i = 45;i >= 18 ;i--)
    {
        for(j = 15;j < 65; j++)
        {
            if(img[i][j] == Black && img[i][j+1]== White)
            {
                jump_num++;//累计黑白跳变点数目
            }
            if(jump_num >=7)
            {
                break;
            }
        }
        if(jump_num >=7 )
        {
            link_num++;
        }
        if(link_num>=3)
        {
            zebra_flag=1;
            break;
        }
    }
}*/

