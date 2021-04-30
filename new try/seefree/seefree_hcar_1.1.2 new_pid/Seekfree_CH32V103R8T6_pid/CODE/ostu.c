/*
 * ostu.c
 *
 *  Created on: 2021��2��21��
 *      Author: SLM
 */

#include "ostu.h"
#include "hcar_camera.h"
#include "hcar_init.h"

#define Step_yu    252              //4��   �Ȳ�  �Ҷȼ�
#define Step   4

uint8 get_ostu_thres()
{
    uint8 gray_start=0;         //ͼ�����ʼ�Ҷ�
    uint8 gray_end=255;         //ͼ�����ֹ�Ҷ�

    //����ͼ�� ֻ��Ҫ����20-60��
    uint8 height_start=10;
    uint8 width_start=0;

    uint8  width = IMG_W;
    uint8  height = IMG_H;

    uint16 i, j, pixelSum = (width-width_start) * (height-height_start);

    uint16  pixelCount[256]={0};
    float pixelPro[256]={0};

    uint8 i_left,i_right;

    //�����Ҷ�ֱ��ͼ
    for(i=height_start;i<height;i++)
    {
        for(j = width_start;j<width;j++)
        {
            pixelCount[img[i][j]&Step_yu]++;
        }
    }

    //�ҵ��Ҷ�ֵ����ʼ�����ֹ��
    uint8 start_ok=0;
    for(i=0;i<256;i=i+Step)
    {
        if(pixelCount[i])           //���Լ������һ������ֵ
        {
            gray_end=i;
            start_ok=1;             //�Ժ��ټ�¼startֵ
        }
        else if(!start_ok)
        {
            gray_start=i;
        }
    }

    float gray_mean=0;              //ͼ��ĻҶȵľ�ֵ

    //����ÿ������������ͼ���еı�����������ƽ���Ҷ�ֵ
    for(i = 0; i < 256; i=i+Step)
    {
        pixelPro[i] = (float)(pixelCount[i]) / (float)(pixelSum);
        gray_mean=i*pixelPro[i]+gray_mean;
    }

    uint8 s_gray_mean=(uint8)gray_mean;     //ͼ��ĻҶȵľ�ֵ

    //��һ��ͼ��ĻҶȵ�ƽ��ֵ������䷽��
    uint32 q_gray_mean=0,h_gray_mean=0;
    uint32 q_sum=0,h_sum=0;
    for(i = gray_start; i < gray_end; i=i+Step)
    {
        if(i<s_gray_mean)
        {
            q_sum=q_sum+pixelCount[i];
            q_gray_mean=i*pixelCount[i]+q_gray_mean;
        }
        else
        {
            h_sum=h_sum+pixelCount[i];
            h_gray_mean=i*pixelCount[i]+h_gray_mean;
        }
    }
    q_gray_mean=q_gray_mean/q_sum;
    h_gray_mean=h_gray_mean/h_sum;

    //����ostu�㷨,�õ�ǰ���ͱ����ķָ�
    //�����Ҷȼ�[0,255],������������ĻҶ�ֵ,Ϊ�����ֵ
    float q_w0, q_w1, q_u0tmp, q_u1tmp, q_u0, q_u1, q_u;
    float h_w0, h_w1, h_u0tmp, h_u1tmp, h_u0, h_u1, h_u;
    float deltaTmp_left,deltaTmp_right;
    q_w0 = q_w1 = q_u0tmp = q_u1tmp = q_u0 = q_u1 = q_u  = 0;
    h_w0 = h_w1 = h_u0tmp = h_u1tmp = h_u0 = h_u1 = h_u  = 0;

    for(j = gray_start; j < gray_end; j=j+Step)
    {
        if(j <= h_gray_mean) //��������
        {
            //��iΪ��ֵ���࣬��һ���ܵĸ���
            h_w0 += pixelPro[j];
            h_u0tmp += j * pixelPro[j];
        }
        else       //ǰ������
        {
            //��iΪ��ֵ���࣬�ڶ����ܵĸ���
            h_w1 += pixelPro[j];
            h_u1tmp += j * pixelPro[j];
        }
        if(j <= q_gray_mean) //��������
        {
            //��iΪ��ֵ���࣬��һ���ܵĸ���
            q_w0 += pixelPro[j];
            q_u0tmp += j * pixelPro[j];
        }
        else       //ǰ������
        {
            //��iΪ��ֵ���࣬�ڶ����ܵĸ���
            q_w1 += pixelPro[j];
            q_u1tmp += j * pixelPro[j];
        }
    }

    q_u0 = q_u0tmp / q_w0;        //��һ���ƽ���Ҷ�
    q_u1 = q_u1tmp / q_w1;        //�ڶ����ƽ���Ҷ�
    q_u =  q_u0tmp + q_u1tmp;      //����ͼ���ƽ���Ҷ�

    h_u0 = h_u0tmp / h_w0;        //��һ���ƽ���Ҷ�
    h_u1 = h_u1tmp / h_w1;        //�ڶ����ƽ���Ҷ�
    h_u = h_u0tmp + h_u1tmp;      //����ͼ���ƽ���Ҷ�

    //������䷽��
    deltaTmp_left = q_w0 * (q_u0 - q_u)*(q_u0 - q_u) + q_w1 * (q_u1 - q_u)*(q_u1 - q_u);
    deltaTmp_right= h_w0 * (h_u0 - h_u)*(h_u0 - h_u) + h_w1 * (h_u1 - h_u)*(h_u1 - h_u);


    ///�����Ż�    ʹ�ö��ַ�
    float w0, w1, u0tmp, u1tmp, u0,  u1, u ,deltaTmp;
    i_left=q_gray_mean;
    i_right=h_gray_mean;
    uint8 i_chaju=8;                                    //����2

    for(i = (i_left+i_right)>>1;(i_right-i_left)>i_chaju ; i=(i_left+i_right)>>1)
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for(j = gray_start; j < gray_end; j=j+Step)
        {
            if(j <= i) //��������
            {
                //��iΪ��ֵ���࣬��һ���ܵĸ���
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else       //ǰ������
            {
                //��iΪ��ֵ���࣬�ڶ����ܵĸ���
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }

        u0 = u0tmp / w0;        //��һ���ƽ���Ҷ�
        u1 = u1tmp / w1;        //�ڶ����ƽ���Ҷ�
        u = u0tmp + u1tmp;      //����ͼ���ƽ���Ҷ�
        //������䷽��
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
        //�ҳ������䷽���Լ���Ӧ����ֵ
        /* if(deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }*/
        if(deltaTmp>=deltaTmp_left&&deltaTmp_right>=deltaTmp_left)
        {
            //������ ��
            i_left=i;
            deltaTmp_left=deltaTmp;
        }
        else if(deltaTmp>=deltaTmp_right&&deltaTmp_left>=deltaTmp_right)
        {
            //���� �� ��
            i_right=i;
            deltaTmp_right=deltaTmp;
        }
        ////�ų����ŵ�����³��ֵ�   �м��ֵ��С��ʱ��  ������ѭ����
        else if(deltaTmp<=deltaTmp_left&&deltaTmp<=deltaTmp_right)
        {
          // ��������   ȡ����
            if(deltaTmp_left>=deltaTmp_right)
            {
                i_right=i;               //�滻�ұ�
                deltaTmp_right=deltaTmp;
            }
            else
            {
                i_left=i;                //�滻���
                deltaTmp_left=deltaTmp;
            }
        }
    }
    //���������ֵ;     //i�������Ҷ�ֵ
    return i;
}

void binary_img()
{
    uint8 ostu_thres=get_ostu_thres();
    uint8 i,j;
    for(i=0;i<IMG_H;i++)
    {
        for(j=0;j<IMG_W;j++)
        {
            if(img[i][j]>ostu_thres)
                img[i][j]=White;
            else
                img[i][j]=Black;
        }
    }
//    if(ok_sign==1)
//    {
//        ok_sign=2;
//    }
}
