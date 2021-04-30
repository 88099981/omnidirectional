/*
 * middline.c
 *
 *  Created on: 2021��2��21��
 *      Author: SLM
 */

#include "middline.h"
#include "ostu.h"
#include "hcar_camera.h"
#include "hcar_init.h"

#define DEVIATION_MAX   7    //���ƫ����
#define DEVIATION_MIN   -7      //��Сƫ����
#define EDGE_START 57       //����������ʼ
#define CAR_HEAD    40      //��ͷ����38
#define ALOW_SCOPE  5       //׼�����ƫ�Χ
#define EDGE_END   8        //������ֹ
#define PI 3.14159265
#define edge_clean();           memset(mid,-1,sizeof(mid));

#define KP_X_SET  0
#define KD_X_SET  5
#define KP_Z_SET  14
#define KD_Z_SET  20



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

//Ԫ��ʶ���־λ
vuint8 break_flag;                  //����ֹͣ��־λ
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
vuint8 l_search_end_flag;
vuint8 r_search_end_flag;
vuint8 l_trend_in_falg;

int8 mid[IMG_H];
uint8 img[IMG_H][IMG_W];

//vuint8 e_i=0, e_j=0;
//int8 x_error[5]={0}, z_error[5]={0};
//int16 x_sum=0, z_sum=0;

float sum_x1=0;
//vint16 sum_x2=0;
//vint16 z_error=0;

//float x_filter1=0.0, z_filter2=0.0;
//
//Site_xy X_deviation;
//float angle=0.0;

float kp_x;
//float ki_x=0;
float kd_x;

float kp_z;
//float ki_z=0;
float kd_z;

float lasttime_x=0.0;
float lasttime_z=0.0;
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
    vuint8 l_edge_limt_flag = 0;//������������flag ������������
    vuint8 r_edge_limt_flag = 0;//������������flag ������������
    for(edge_i= EDGE_START;edge_i>1;edge_i--)
    {
        l_get_flag=0;
        r_get_flag=0;
        for(edge_j=1;edge_j<col_end;edge_j++)//�������
        {
            if(img[edge_i][edge_j]==White && img[edge_i+2][edge_j]==Black && img[edge_i-2][edge_j]==Black && img[edge_i][edge_j+2]==Black)
            {
                continue;
            }
            if(img[edge_i][edge_j]==Black && img[edge_i][edge_j+1]==White && img[edge_i][edge_j+2]==White  && img[edge_i][edge_j+4]==White   && img[edge_i][edge_j+5]==White)//�ҵ��ڰ������
            {
                if (edge_i >= EDGE_START - 2 && edge_j<=CAR_HEAD)//������ļ��б�����Ϊ���ŶȽϸߵı���
                {
                    //����̽�⽫��������������Ϊ�����ı���
                    int test_j;
                    for (test_j=edge_j+1;test_j<=CAR_HEAD;test_j++)
                        if (img[edge_i][test_j] == Black && img[edge_i][test_j + 1] == White && img[edge_i][test_j + 2] == White)//�ҵ��ڰ������
                        {
                            edge_j = test_j;//���±�������
                        }
                    //̽���������¼�����
                    new_l_edge[edge_i]=edge_j;//��edge_j���������������
                    l_get_flag=1;//��ȡ����������ر�־λ����
                    l_link_num++;//�������������1
                    if(l_link_num>4)
                    {
                        l_lost_num=0;
                    }
                    l_edge_col_old=edge_j;//�����������ص㸳Ϊedge_j;
                    break;//�ѵ����أ�ֹͣ��������
                }
                else  if((edge_j>= l_edge_col_old-ALOW_SCOPE && edge_j <= l_edge_col_old + ALOW_SCOPE) || l_edge_limt_flag)//λ�ں��ʵķ�Χ��
                {
                    new_l_edge[edge_i] = edge_j;//��edge_j���������������
                    l_get_flag = 1;//��ȡ����������ر�־λ����
                    l_link_num++;//�������������1
                    if (l_link_num>4)
                    {
                        l_lost_num = 0;
                    }
                    l_edge_col_old = edge_j;//�����������ص㸳Ϊedge_j;

                    //�ر�������
                    l_edge_limt_flag = 0;

                    break;//�ѵ����أ�ֹͣ��������
                }
                else if (new_l_edge[edge_i + 2] <= 2 && new_r_edge[edge_i + 2] >= IMG_W - 2 )//����ʮ���������
                {
                    l_edge_limt_flag = 1;//�������εȴ��´�У��
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

        for(edge_j=col_end;edge_j>col_start;edge_j--)//���ұ���
        {
            if(img[edge_i][edge_j]==White && img[edge_i+2][edge_j]==Black && img[edge_i-2][edge_j]==Black && img[edge_i][edge_j-2]==Black)
            {
                continue;
            }
            if(img[edge_i][edge_j]==Black && img[edge_i][edge_j-1]==White && img[edge_i][edge_j-2]==White  && img[edge_i][edge_j-4]==White   && img[edge_i][edge_j-5]==White)//�ҵ��ڰ������
            {

                if (edge_i >= EDGE_START - 2 && edge_j >= CAR_HEAD)//������ļ��б�����Ϊ���ŶȽϸߵı���
                {
                    //����̽�⽫��������������Ϊ�����ı���
                    int test_j;
                    for (test_j = edge_j ; test_j >= CAR_HEAD; test_j--)
                        if (img[edge_i][test_j] == Black && img[edge_i][test_j - 1] == White && img[edge_i][test_j - 2] == White)//�ҵ��ڰ������
                        {
                            edge_j = test_j;//���±�������
                        }
                    //̽���������¼�����
                    new_r_edge[edge_i] = edge_j;//��edge_j���������������
                    r_get_flag = 1;//��ȡ����������ر�־λ����
                    r_link_num++;//�������������1
                    if (r_link_num>4)
                    {
                        r_lost_num = 0;
                    }
                    r_edge_col_old = edge_j;//�����������ص㸳Ϊedge_j;
                    break;//�ѵ����أ�ֹͣ��������
                }
                else  if ((edge_j >= r_edge_col_old - ALOW_SCOPE && edge_j <= r_edge_col_old + ALOW_SCOPE) || r_edge_limt_flag)//λ�ں��ʵķ�Χ��
                {
                    new_r_edge[edge_i] = edge_j;//��edge_j���������������
                    r_get_flag = 1;//��ȡ����������ر�־λ����
                    r_link_num++;//�������������1
                    if (r_link_num>4)
                    {
                        r_lost_num = 0;
                    }
                    r_edge_col_old = edge_j;//�����������ص㸳Ϊedge_j;

                                            //�ر�������
                    r_edge_limt_flag = 0;

                    break;//�ѵ����أ�ֹͣ��������
                }
                else if (new_l_edge[edge_i + 2] <= 2 && new_r_edge[edge_i + 2] >= IMG_W - 2 )//����ʮ���������
                {
                    r_edge_limt_flag = 1;//�������εȴ��´�У��
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
    //int8 i,ii,edge_jj;
    int8 i=0;
    //uint8 num=0;
    //int8 cross_l = -1, cross_r = -1;//����ʮ�ֹյ㣬һ��ͼ�����Զ�������λ-1
    //int8 bend_l_allow = 3, bend_r_allow = 3;//���������������λ
    //int8 test_i;
    //int8 test_j;//����̽�����
//    uint8 num=0;
    //int8 mid_num=0;
    //int8 up[2]={0};
    //int8 down[2]={0};
    //int8 cross_edge=0;
    float ratio_k = 2.0;//�������ϵ��2.0 1.7
    //float d_in=0.0;
    //float d_inn=0.0;
    for(i= EDGE_START;i>EDGE_END;i--)
    {
        //if(new_l_edge[i]!=0&&new_r_edge[i]== IMG_W && bend_r_allow)///�����->
        if(new_l_edge[i]!=0&&new_r_edge[i]== IMG_W)
        {
            r_turn_flag=1;
            //mid[i]=(new_l_edge[i]+col_end)/2;
            //mid[i]=(new_l_edge[i]+new_r_edge[i+1])/2;
            //mid[i]=(new_l_edge[edge_i+1]+1)/2+new_l_edge[i]-new_l_edge[i+1];
            //mid[i]=mid[i+2]+new_l_edge[i]-new_l_edge[i+2];
            //mid[i]=correct[i]+new_l_edge[i];
            //����̽���ֹ���С�ע�⣺���ڴ���̫�󣬽��ڱ��ֵ�һ��ʹ��
            /*if (bend_r_allow == 3)//���ֵ�һ��
            {
                for (test_i = i; test_i > 1 && bend_r_allow; test_i -= 2)//Ϊ�˿�������
                    if (new_r_edge[test_i] != IMG_W)//�����ִ���ֵ���������Σ����ڴ����걾�κ�����������⴦��
                    {
                        test_i += 2;//��ԭ
                        for (; test_i > 1 && bend_r_allow; test_i -= 2)
                            if (img[test_i][70] == White)//������
                                bend_r_allow--;
                            else
                                break;
                        break;
                    }
                bend_r_allow = bend_r_allow == 0 ? 0 : 1;//��֤����ʹ��һ��

            }*/

            //�������
            if (i >= EDGE_START - 2)//���ڿ�ͷ�ļ������⴦����Ϊû�вο���
            {
                mid[i] = ( new_l_edge[i] + IMG_W ) / 2;//ֻ��l����ȡ�м�ֵ
            }
            else    //���Բο���������
            {
                mid[i] = mid[i + 1] + ratio_k*( new_l_edge[i] - new_l_edge[i + 1] );//��һ�����߼��ϱ�������ƫ��ֵ��ע�⣺Ĭ�ϱ�����ȷ�ҵ���
            }

        }
        //else if(new_l_edge[i]==0&&new_r_edge[i]!=IMG_W && bend_l_allow)///�����<-
        else if(new_l_edge[i]==0&&new_r_edge[i]!=IMG_W)
        {
            //mid[i]=(new_l_edge[i]+col_end)/2;
            //mid[i]=(new_r_edge[i]+new_l_edge[i+1])/2;
            //mid[i]=(new_r_edge[edge_i+1]+1)/2+new_r_edge[i]-new_r_edge[i+1];
            //mid[i]=mid[i+2]+new_r_edge[i]-new_r_edge[i+2];
            //mid[i]=new_r_edge[i]-correct[i];
            //����̽���ֹ���С�ע�⣺���ڴ���̫�󣬽��ڱ��ֵ�һ��ʹ��
            l_turn_flag=1;
            /*if (bend_l_allow == 3)//���ֵ�һ��
            {
                for (test_i = i; test_i > 1 && bend_l_allow; test_i -= 2)//Ϊ�˿�������
                    if (new_l_edge[test_i]!=0)//�����ִ���ֵ���������Σ����ڴ����걾�κ�����������⴦��
                    {
                        test_i += 2;//��ԭ
                        for (; test_i > 1 && bend_l_allow; test_i-=2)
                            if (img[test_i][10] == White)//������
                                bend_l_allow--;
                            else
                                break;
                        break;
                    }
                bend_l_allow = bend_l_allow == 0 ? 0 : 1;//��֤����ʹ��һ��
            }*/

            //�������
            if (i >= EDGE_START- 2)//���ڿ�ͷ�ļ������⴦����Ϊû�вο���
            {
                mid[i] = (new_r_edge[i]) / 2;//ֻ��r����ȡ�м�ֵ
            }
            else    //���Բο���������
            {
                mid[i] = mid[i + 1] + ratio_k*(new_r_edge[i] - new_r_edge[i + 1]);//��һ�����߼��ϱ�������ƫ��ֵ��ע�⣺Ĭ�ϱ�����ȷ�ҵݼ�
            }

        }
        else if(new_l_edge[i]<=5&&new_r_edge[i]>=IMG_W-5)//ʮ�ִ���
        {
           // mid[i]=mid[i+1];
            //���������
            /*if (cross_l == -1 && cross_r == -1)//��ʼ������δ�ҵ��յ�
            {
                int j;
                for ( j =i;j>=0;j--)    //Ѱ��l�յ�
                    if (new_l_edge[i] != -1)//������ֱ����ˣ���Ϊ�յ�
                    {
                        cross_l = (new_l_edge[j - 4] + new_l_edge[j - 5]) / 2;//ȡ����������Ϊ�ο��Է����
                        break;
                    }
                for ( j = i; j >= 0; j--)//Ѱ��r�յ�
                    if (new_r_edge[i] != -1)//������ֱ����ˣ���Ϊ�յ�
                    {
                        cross_r = (new_r_edge[j - 4] + new_r_edge[j - 5]) / 2;//ȡ����������Ϊ�ο��Է����
                        break;
                    }
            }*/
            //�ҵ��յ����������
            //mid[i] = (cross_l + cross_r) / 2;

                //mid[i] = (new_l_edge[i] + new_r_edge[i]) / 2;
//            for(test_i=EDGE_END;test_i>=EDGE_END-3;test_i--)
//            {
//                for(test_j=0;test_j<IMG_W;test_j++)
//                {
//                    if(img[test_i][test_j]==White)
//                    {
//                        num++;
//                    }
//                }
//
//            }
//            if(num>=30 )
//            {
//                cross_flag=1;
//                mid[i]=IMG_W/2;
//            }

            mid[i] = IMG_W / 2;
            //img[i][mid[i]] = Black;
        }
        else            ///�������⴦���ų��������ͨ����
        {
//            if((new_r_edge[i]-new_l_edge[i])>55 && (new_r_edge[i-1]-new_l_edge[i-1])>55)
//            {
//                mid[i]=IMG_W/2;
//                cross_flag=1;
//            }
//            else
//            {
//                mid[i] = (new_l_edge[i] + new_r_edge[i]) / 2;
//            }
             mid[i] = (new_l_edge[i] + new_r_edge[i]) / 2;
        }
        img[i][mid[i]]=Black;
        //��ʾ����
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

    cross_flag=0;//�¼ӵ�
    three_fork_flag=0;//�¼ӵ�

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
{   //��������ͷ��װ�Ƕ�ԭ�򣬿��ܻ��ĵ��Ű��ߵ���ɸ��ţ��ֱ��򵥵İ���Щλ�������ð�
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

    vint8 temp=0;
    //    int8 num=0;
        //int8 ave=0;

        sum_x1=0;
//        sum_x2=0;
//        z_error=0;

        for(temp=EDGE_START;temp>EDGE_START-5;temp--)
        {
            if(mid[temp])
            {
                sum_x1+=mid[temp]-40;
            }
        }
        sum_x1/=5.0;
//        for(temp=EDGE_START-5;temp>EDGE_START-8;temp--)
//        {
//            if(mid[temp])
//            {
//                sum_x2+=mid[temp]-40;
//            }
//        }
//        z_error=sum_x1;

//        z_error=-z_error;
    //guiyi
//    X_deviation.x=X_deviation.x*20.0/3.0;
//    angle=angle*10.0/9.0;
//    if(X_deviation.x-100.0>=0.0001)
//    {
//        X_deviation.x=100.0;
//    }
//    else if(X_deviation.x+100.0<=-0.0001)
//    {
//        X_deviation.x=-100.0;
//    }
//    if(angle-100.0>=0.0001)
//    {
//        angle=100.0;
//    }
//    else if(angle+100.0<=-0.0001)
//    {
//        angle=-100.0;
//    }

//    //�˲�
//    x_error[e_i] = X_deviation.x;
//    z_error[e_i] = angle;
//
//    //����
//    if(e_j<5)
//    {
//        e_j++;
//    }
//    e_i++;
//    if(e_i > 4)
//    {
//        e_i = 0;
//    }
//    //�ۺ͹���
//    x_sum = 0;
//    z_sum = 0;
//
//    //�ۺ�
//    for(uint8_t j=0;j<5;j++)
//    {
//        x_sum += x_error[j];
//    }
//    for(uint8_t j=0;j<5;j++)
//    {
//        z_sum += z_error[j];
//    }
//
//    //ƽ��
//    x_filter1 = x_sum/e_j;
//    z_filter2 = z_sum/e_j;


//    aim_x = kp_x*x_filter1 + kd_x*(x_filter1-lasttime_x);
//    ////lasttime_x=X_deviation.x;
//    lasttime_x=x_filter1;
//
//    aim_z = kp_z*z_filter2 + kd_z*(z_filter2-lasttime_z);
//    lasttime_z=z_filter2;

        if(l_turn_flag==1||r_turn_flag==1)
        {
//            kp_x=0.6;
            aim_y=100;//80
        }
        else
        {
//            kp_x=KP_X_SET;
//            kd_x=KD_X_SET;
            kp_z=KP_Z_SET;
            kd_z=KD_Z_SET;
            aim_y=125;//160
        }
//        aim_x = kp_x*sum_x1 + kd_x*(sum_x1-lasttime_x);
//        ////lasttime_x=X_deviation.x;
//        lasttime_x=sum_x1;

        aim_z = kp_z*sum_x1 + kd_z*(sum_x1-lasttime_z);
        lasttime_z=sum_x1;


}

//****************************************��͸�Ӻ���****************************************//
Site_xy get_inv_img(int8 xxx ,int8 yyy)//��͸�ӣ�xxxΪͼ���У�yyyΪ�У�
{

  Site_xy temp;
  xxx++;
  yyy++;
  temp.x = (INV_A*yyy+INV_B*xxx+INV_C)/(INV_G*yyy+INV_H*xxx+1);
  temp.y = (INV_D*yyy+INV_E*xxx+INV_F)/(INV_G*yyy+INV_H*xxx+1);
  return temp;

}

Site_xy1 get_invinv_img(float xxx,float yyy)//����͸�ӣ�xxxΪʵ���У�yyyΪ�У�
{

  Site_xy1 temp;
  float uSrcimg,vSrcimg;//����͸������
  uSrcimg =(INV_B*INV_F - INV_C*INV_E + INV_E*yyy - INV_B*xxx - INV_F*INV_H*yyy + INV_C*INV_H*xxx)/(INV_A*INV_E - INV_B*INV_D + INV_D*INV_H*yyy - INV_E*INV_G*yyy - INV_A*INV_H*xxx + INV_B*INV_G*xxx);
  vSrcimg =-(INV_A*INV_F - INV_C*INV_D + INV_D*yyy - INV_A*xxx - INV_F*INV_G*yyy + INV_C*INV_G*xxx)/(INV_A*INV_E - INV_B*INV_D + INV_D*INV_H*yyy - INV_E*INV_G*yyy - INV_A*INV_H*xxx + INV_B*INV_G*xxx);
  temp.x=round(vSrcimg-1);
  temp.y=round(uSrcimg-1);
  return temp;

}

void three_fork()//����ͣ������
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
  {//����·�ڴ�ͣ��
      if(img[20][11]==White && img[20][12]==White && img[20][13]==White && img[21][11]==White && img[21][12]==White && img[21][13]==White)
      {
          if(img[20][70]==White && img[20][71]==White && img[20][72]==White && img[21][70]==White && img[21][71]==White && img[21][72]==White)
          {
              three_fork_flag=1;
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
                jump_num++;//�ۼƺڰ��������Ŀ
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

