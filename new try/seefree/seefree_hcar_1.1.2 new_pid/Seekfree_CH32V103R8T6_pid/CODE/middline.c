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

#define DEVIATION_MAX   7    //最大偏差量
#define DEVIATION_MIN   -7      //最小偏差量
#define EDGE_START 55       //搜索边沿起始
#define CAR_HEAD    40      //车头坐标
#define ALOW_SCOPE  5       //准许边沿偏差范围
#define EDGE_END   8        //搜索终止
#define PI 3.14159265
#define edge_clean();           memset(mid,-1,sizeof(mid));

float kp_x=5;
//float ki_x=0;
float kd_x=0;

float kp_z=3;
//float ki_z=0;
float kd_z=0;

float lasttime_x=0.0;
float lasttime_z=0.0;

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

//int16 shu;
vuint8 break_flag;                  //出界停止标志位
vuint8 zebra_flag;
vuint8 l_turn_flag;
vuint8 r_turn_flag;
int8 l_link_num;
int8 r_link_num;
int8 l_lost_num;
int8 r_lost_num;
int8 temp_num;
int8 temp_row;
vuint8 l_search_end_flag;
vuint8 r_search_end_flag;
vuint8 l_trend_in_falg;

int8 mid[IMG_H];
uint8 img[IMG_H][IMG_W];

Site_xy X_deviation;//可能需要去除
float angle=0.0;//可能需要去除
void edge_clean_2()
{
    int8 i;
    for (i = 0; i < IMG_H; i++)
    {
        new_l_edge[i] = 0;
        new_r_edge[i] = IMG_W;
    }
}

void get_new_edge()
{
    edge_clean();
    edge_clean_2();
    wipe_out();
    initialize_variables();
    l_link_num=0;
    r_link_num=0;
    vuint8 l_edge_limt_flag = 0;//边沿限制搜索flag 用于屏蔽条件
    vuint8 r_edge_limt_flag = 0;//边沿限制搜索flag 用于屏蔽条件
    for(edge_i= EDGE_START;edge_i>1;edge_i--)
    {
        l_get_flag=0;
        r_get_flag=0;
        for(edge_j=1;edge_j<col_end;edge_j++)//找左边沿
        {
            if(img[edge_i][edge_j]==White && img[edge_i+2][edge_j]==Black && img[edge_i-2][edge_j]==Black && img[edge_i][edge_j+2]==Black)
            {
                continue;
            }
            if(img[edge_i][edge_j]==Black && img[edge_i][edge_j+1]==White && img[edge_i][edge_j+2]==White  && img[edge_i][edge_j+4]==White   && img[edge_i][edge_j+5]==White)//找到黑白跳变点
            {
                if (edge_i >= EDGE_START - 2 && edge_j<=CAR_HEAD)//对最初的几行边沿作为置信度较高的边沿
                {
                    //线性探测将最里面的跳变点作为真正的边沿
                    int test_j;
                    for (test_j=edge_j+1;test_j<=CAR_HEAD;test_j++)
                        if (img[edge_i][test_j] == Black && img[edge_i][test_j + 1] == White && img[edge_i][test_j + 2] == White)//找到黑白跳变点
                        {
                            edge_j = test_j;//更新边沿坐标
                        }
                    //探测结束正常录入边沿
                    new_l_edge[edge_i]=edge_j;//将edge_j存入左边沿数组中
                    l_get_flag=1;//提取到本行左边沿标志位成立
                    l_link_num++;//连续左边沿数加1
                    if(l_link_num>4)
                    {
                        l_lost_num=0;
                    }
                    l_edge_col_old=edge_j;//本行搜索边沿点赋为edge_j;
                    break;//搜到边沿，停止本行搜索
                }
                else  if((edge_j>= l_edge_col_old-ALOW_SCOPE && edge_j <= l_edge_col_old + ALOW_SCOPE) || l_edge_limt_flag)//位于合适的范围内
                {
                    new_l_edge[edge_i] = edge_j;//将edge_j存入左边沿数组中
                    l_get_flag = 1;//提取到本行左边沿标志位成立
                    l_link_num++;//连续左边沿数加1
                    if (l_link_num>4)
                    {
                        l_lost_num = 0;
                    }
                    l_edge_col_old = edge_j;//本行搜索边沿点赋为edge_j;

                    //关闭屏蔽字
                    l_edge_limt_flag = 0;

                    break;//搜到边沿，停止本行搜索
                }
                else if (new_l_edge[edge_i + 2] <= 2 && new_r_edge[edge_i + 2] >= IMG_W - 2 )//对于十字特殊情况
                {
                    l_edge_limt_flag = 1;//开启屏蔽等待下次校正
                }


            }
            else
            {
                l_lost_num++;
                if(l_lost_num>4)
                {
                    l_link_num=0;
                    //l_lost_flag=1;
                }
            }

        }

        for(edge_j=col_end;edge_j>col_start;edge_j--)//找右边沿
        {
            if(img[edge_i][edge_j]==White && img[edge_i+2][edge_j]==Black && img[edge_i-2][edge_j]==Black && img[edge_i][edge_j-2]==Black)
            {
                continue;
            }
            if(img[edge_i][edge_j]==Black && img[edge_i][edge_j-1]==White && img[edge_i][edge_j-2]==White  && img[edge_i][edge_j-4]==White   && img[edge_i][edge_j-5]==White)//找到黑白跳变点
            {

                if (edge_i >= EDGE_START - 2 && edge_j >= CAR_HEAD)//对最初的几行边沿作为置信度较高的边沿
                {
                    //线性探测将最里面的跳变点作为真正的边沿
                    int test_j;
                    for (test_j = edge_j ; test_j >= CAR_HEAD; test_j--)
                        if (img[edge_i][test_j] == Black && img[edge_i][test_j - 1] == White && img[edge_i][test_j - 2] == White)//找到黑白跳变点
                        {
                            edge_j = test_j;//更新边沿坐标
                        }
                    //探测结束正常录入边沿
                    new_r_edge[edge_i] = edge_j;//将edge_j存入左边沿数组中
                    r_get_flag = 1;//提取到本行左边沿标志位成立
                    r_link_num++;//连续左边沿数加1
                    if (r_link_num>4)
                    {
                        r_lost_num = 0;
                    }
                    r_edge_col_old = edge_j;//本行搜索边沿点赋为edge_j;
                    break;//搜到边沿，停止本行搜索
                }
                else  if ((edge_j >= r_edge_col_old - ALOW_SCOPE && edge_j <= r_edge_col_old + ALOW_SCOPE) || r_edge_limt_flag)//位于合适的范围内
                {
                    new_r_edge[edge_i] = edge_j;//将edge_j存入左边沿数组中
                    r_get_flag = 1;//提取到本行左边沿标志位成立
                    r_link_num++;//连续左边沿数加1
                    if (r_link_num>4)
                    {
                        r_lost_num = 0;
                    }
                    r_edge_col_old = edge_j;//本行搜索边沿点赋为edge_j;

                                            //关闭屏蔽字
                    r_edge_limt_flag = 0;

                    break;//搜到边沿，停止本行搜索
                }
                else if (new_l_edge[edge_i + 2] <= 2 && new_r_edge[edge_i + 2] >= IMG_W - 2 )//对于十字特殊情况
                {
                    r_edge_limt_flag = 1;//开启屏蔽等待下次校正
                }
            }
            else
            {
                r_lost_num++;
                if(r_lost_num>4)
                {
                    r_link_num=0;
                //  r_lost_flag=1;
                }

            }

        }
        //if(new_l_edge[edge_i] && new_r_edge[edge_i])
        //{
        //  correct[edge_i]=(new_r_edge[edge_i]-new_l_edge[edge_i])/2;
        //}
    }
//    if(ok_sign==2)
//    {
//        ok_sign=3;
//    }

}

void set_middline_black()
{
    int8 i;
    vuint8 cross_l = -1;//定义十字拐点，一副图用完自动重新置位-1
    vuint8 cross_r = -1;
    int8 bend_l_allow = 3, bend_r_allow = 3;//定义弯道处理允许位
    int8 test_i;//线性探测计数
    float ratio_k = 2;//弯道比例系数
    for(i= EDGE_START;i>EDGE_END;i--)
    {
        if(new_l_edge[i]!=0&&new_r_edge[i]== IMG_W && bend_r_allow)///右弯道->
        {
            //mid[i]=(new_l_edge[i]+col_end)/2;
            //mid[i]=(new_l_edge[i]+new_r_edge[i+1])/2;
            //mid[i]=(new_l_edge[edge_i+1]+1)/2+new_l_edge[i]-new_l_edge[i+1];
            //mid[i]=mid[i+2]+new_l_edge[i]-new_l_edge[i+2];
            //mid[i]=correct[i]+new_l_edge[i];
            //线性探测防止误判。注意：由于代价太大，仅在本轮第一次使用
            if (bend_r_allow == 3)//本轮第一次
            {
                for (test_i = i; test_i > 1 && bend_r_allow; test_i -= 2)//为了快速收敛
                    if (new_r_edge[test_i] != IMG_W)//若出现存在值且连续三次，则在处理完本次后放弃本轮特殊处理
                    {
                        test_i += 2;//还原
                        for (; test_i > 1 && bend_r_allow; test_i -= 2)
                            if (img[test_i][70] == White)//定点检测
                                bend_r_allow--;
                            else
                                break;
                        break;
                    }
                bend_r_allow = bend_r_allow == 0 ? 0 : 1;//保证仅仅使用一次

            }

            //弯道处理
            if (i >= EDGE_START - 2)//对于开头的几行特殊处理因为没有参考项
            {
                mid[i] = ( new_l_edge[i] + IMG_W ) / 2;//只有l存在取中间值
            }
            else    //可以参考进行修正
            {
                mid[i] = mid[i + 1] + ratio_k*( new_l_edge[i] - new_l_edge[i + 1] );//上一个中线加上边沿曲率偏差值，注意：默认边沿正确且递增
            }

        }
        else if(new_l_edge[i]==0&&new_r_edge[i]!=IMG_W && bend_l_allow)///左弯道<-
        {
            //mid[i]=(new_l_edge[i]+col_end)/2;
            //mid[i]=(new_r_edge[i]+new_l_edge[i+1])/2;
            //mid[i]=(new_r_edge[edge_i+1]+1)/2+new_r_edge[i]-new_r_edge[i+1];
            //mid[i]=mid[i+2]+new_r_edge[i]-new_r_edge[i+2];
            //mid[i]=new_r_edge[i]-correct[i];
            //线性探测防止误判。注意：由于代价太大，仅在本轮第一次使用
            if (bend_l_allow == 3)//本轮第一次
            {
                for (test_i = i; test_i > 1 && bend_l_allow; test_i -= 2)//为了快速收敛
                    if (new_l_edge[test_i]!=0)//若出现存在值且连续三次，则在处理完本次后放弃本轮特殊处理
                    {
                        test_i += 2;//还原
                        for (; test_i > 1 && bend_l_allow; test_i-=2)
                            if (img[test_i][10] == White)//定点检测
                                bend_l_allow--;
                            else
                                break;
                        break;
                    }
                bend_l_allow = bend_l_allow == 0 ? 0 : 1;//保证仅仅使用一次
            }

            //弯道处理
            if (i >= EDGE_START- 2)//对于开头的几行特殊处理因为没有参考项
            {
                mid[i] = (new_r_edge[i]) / 2;//只有r存在取中间值
            }
            else    //可以参考进行修正
            {
                mid[i] = mid[i + 1] + ratio_k*(new_r_edge[i] - new_r_edge[i + 1]);//上一个中线加上边沿曲率偏差值，注意：默认边沿正确且递减
            }

        }
        else if(new_l_edge[i]<=2&&new_r_edge[i]>=IMG_W-2)//十字处理
        {
            mid[i]=mid[i+1];
            //分情况处理
            if (cross_l == -1 && cross_r == -1)//初始出现尚未找到拐点
            {
                int j;
                for ( j =i;j>=0;j--)    //寻找l拐点
                    if (new_l_edge[i] != -1)//如果出现边沿了，作为拐点
                    {
                        cross_l = (new_l_edge[j - 4] + new_l_edge[j - 5]) / 2;//取后续边沿作为参考以防噪点
                        break;
                    }
                for ( j = i; j >= 0; j--)//寻找r拐点
                    if (new_r_edge[i] != -1)//如果出现边沿了，作为拐点
                    {
                        cross_r = (new_r_edge[j - 4] + new_r_edge[j - 5]) / 2;//取后续边沿作为参考以防噪点
                        break;
                    }
            }
            //找到拐点后正常补线
            //mid[i] = (cross_l + cross_r) / 2;
            mid[i] = IMG_W / 2;
            //img[i][mid[i]] = Black;
        }
        else            ///所有特殊处理排除后才是普通处理
        {
            mid[i] = (new_l_edge[i] + new_r_edge[i]) / 2;
        }
        img[i][mid[i]]=Black;
        //显示边沿
        //img[i][new_l_edge[i]+2] = Black;
        //img[i][new_r_edge[i]-1] = Black;
    }
//    if(ok_sign==3)
//    {
//        ok_sign=4;
//    }

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

    l_get_flag=0;
    r_get_flag=0;

    break_flag=0;
    zebra_flag=0;

    l_edge_col_old=0;
    r_edge_col_old=77;

    l_end_row=0;

    l_turn_flag=0;
    r_turn_flag=0;


}

void wipe_out()
{   //由于摄像头安装角度原因，可能会拍到杜邦线等造成干扰，粗暴简单的把那些位置像素置白
    img[58][39] = White;
    img[58][40] = White;
    img[59][40] = White;
    img[57][39] = White;
    img[58][36] = White;
    img[56][38] = White;
    img[55][37] = White;
    img[56][36] = White;
    img[57][35] = White;
    img[58][35] = White;
    img[59][35] = White;
}

void road_seek()
{

//    int8 temp=0;
//    int8 num=0;
    //int8 ave=0;
    float k=0.0;
    float difference=0.0;
    Site_xy X1_deviation={0.0};

//    for(temp= EDGE_START;temp>EDGE_END;temp-- )
//    {
//        if(mid[temp])
//        {
//            num++;
//        }
//        else
//        {
//            break;
//        }
//    }
//    if(num>4)
//    {
//        ave=num/4;
//    }
    X_deviation=get_inv_img(EDGE_START,mid[EDGE_START]);
    ////X1_deviation = get_inv_img(EDGE_START-ave ,mid[EDGE_START-ave]);//近距
    X1_deviation = get_inv_img(42 ,mid[42]);//近距

//        assert(X_deviation.y);
    difference=X1_deviation.y-X_deviation.y;
    if(difference<0.0001)
    {
        difference=0.0001;
    }
    k=(X1_deviation.x-X_deviation.x)/difference;
    ////k=X_deviation.x/X_deviation.y;//为负应偏左
        ////angle = atan2(X_deviation.y, X_deviation.x)*180/PI;
    angle = atan(k)*180/PI;


    aim_x = kp_x*X_deviation.x + kd_x*(X_deviation.x-lasttime_x);
    ////lasttime_x=X_deviation.x;
    lasttime_x=X_deviation.y;

    aim_z = kp_z*angle + kd_z*(angle-lasttime_z);
    lasttime_z=angle;
//    if(ok_sign==4)
//    {
//        ok_sign=0;
//    }
}

//****************************************逆透视函数****************************************//
Site_xy get_inv_img(int8 xxx ,int8 yyy)//逆透视（xxx为图像行，yyy为列）
{

  Site_xy temp;
  xxx++;
  yyy++;
  temp.x = (INV_A*yyy+INV_B*xxx+INV_C)/(INV_G*yyy+INV_H*xxx+1);
  temp.y = (INV_D*yyy+INV_E*xxx+INV_F)/(INV_G*yyy+INV_H*xxx+1);
  return temp;

}

Site_xy1 get_invinv_img(float xxx,float yyy)//反逆透视（xxx为实际行，yyy为列）
{

  Site_xy1 temp;
  float uSrcimg,vSrcimg;//反逆透视坐标
  uSrcimg =(INV_B*INV_F - INV_C*INV_E + INV_E*yyy - INV_B*xxx - INV_F*INV_H*yyy + INV_C*INV_H*xxx)/(INV_A*INV_E - INV_B*INV_D + INV_D*INV_H*yyy - INV_E*INV_G*yyy - INV_A*INV_H*xxx + INV_B*INV_G*xxx);
  vSrcimg =-(INV_A*INV_F - INV_C*INV_D + INV_D*yyy - INV_A*xxx - INV_F*INV_G*yyy + INV_C*INV_G*xxx)/(INV_A*INV_E - INV_B*INV_D + INV_D*INV_H*yyy - INV_E*INV_G*yyy - INV_A*INV_H*xxx + INV_B*INV_G*xxx);
  temp.x=round(vSrcimg-1);
  temp.y=round(uSrcimg-1);
  return temp;

}

void three_fork()//出界停车函数
{
  /*int8 k,j;
  int16 shu=0;
//  int16 w_num=0;

  shu=0;

  for(k=IMG_H-8;k>IMG_H-10;k--)
  {
    for(j=0;j<IMG_W;j++)
    {
      if(img[k][j]==Black)
      {

        shu++;

      }
    }
  }
  if(shu>140&&(break_flag=0))//if(shu>=CAMERA_W*3-5)
  {

    break_flag=1;

  }*/
  if((break_flag==0)&&(img[49][39]==White && img[49][40]==White && img[49][41]==White && img[50][39]==White && img[50][40]==White && img[50][41]==White))
  {//三叉路口处停车
      if(img[20][11]==White && img[20][12]==White && img[20][13]==White && img[21][11]==White && img[21][12]==White && img[21][13]==White)
      {
          if(img[20][70]==White && img[20][71]==White && img[20][72]==White && img[21][70]==White && img[21][71]==White && img[21][72]==White)
          {
              break_flag=1;
          }
      }
  }
  /*for(k=2;k<8;k++)
  {
    for(j=40;j>=2;j--)
    {
        if(img[k][j]==Black && img[k][j-1]==White && img[k][j-2]==White && count<12)
        {
            count++;
        }
    }
    for(j=40;j<75;j++)
    {
        if(img[k][j]==Black && img[k][j-1]==White && img[k][j-2]==White && count<12)
        {
            count++;
        }
    }
  }*/

}

void zebra_crossing()
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
}

