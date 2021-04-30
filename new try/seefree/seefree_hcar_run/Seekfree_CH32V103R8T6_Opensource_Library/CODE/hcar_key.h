#ifndef _hcar_key_h
#define _hcar_key_h
#include"common.h"

//���尴������
#define KEY1    C8
#define KEY2    C9
#define KEY3    B2

//����״̬����
extern vuint8 key1_status;
extern vuint8 key2_status;
extern vuint8 key3_status;

//��һ�ο���״̬����
extern vuint8 key1_last_status;
extern vuint8 key2_last_status;
extern vuint8 key3_last_status;

//���ر�־λ
extern vuint8 key1_flag;
extern vuint8 key2_flag;
extern vuint8 key3_flag;

//����
void hcar_key_init(void);
void hcar_key_get(void);
#endif
