/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�SPIN27PS/CH32V103R8T6���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2020��10��28��
 �������Ϣ�ο����е�ַ��
 ����    վ��http://www.lqist.cn
 ���Ա����̡�http://longqiu.taobao.com
 ------------------------------------------------
 ��IDE��CH32V103R8T6:MounRiver Studio�����ϰ汾
 ��IDE��MM32SPIN27PS:IAR7.8/MDK5.2�����ϰ汾  
 ��Target �� SPIN27PS/CH32V103R8T6
 ��SYS PLL�� 80/96MHz
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_
#define SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_

extern volatile u8 Game_Over; // С�����ȫ������ͣ��
extern int16_t BackOffset ;
extern int16_t FrontOffset ;
extern u16 MagneticField;     // �ų�ǿ��
extern int16_t LleftFP, LleftFV, LrightFV, LrightFP;  // ���ƫ����
extern int16_t LleftBP, LleftBV, LrightBV, LrightBP;  // ���ƫ����
extern int16_t ad_max[8]; // �°��ӷŵ������궨���ֵ,�ᱻ����ˢ��
extern int16_t ad_min[8] ; // �°��Ӿ���Ҫ�궨��Сֵ,�ᱻ����ˢ��
/*************************************************************************
 *  �������ƣ�void InductorInit (void)
 *  ����˵�����ĸ����ADC��ʼ��������
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void InductorInit(void);

/*************************************************************************
 *  �������ƣ�void InductorNormal(void)
 *  ����˵�����ɼ���е�ѹ������һ����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void InductorNormal(void);

/*************************************************************************
 *  �������ƣ�uint8 ReadOutInGarageMode(void)
 *  ����˵������ȡ���뿪�����ó����ģʽ
 *  ����˵������
 *  �������أ������ģʽ,0�����⣻Ĭ��1�ҳ����
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
uint8_t ReadOutInGarageMode(void);

/*************************************************************************
 *  �������ƣ�void ElectroMagneticCar(void)
 *  ����˵������ų�˫������ٿ���
 -->1.�����㷨���򵥵ķֶα��������㷨����ѧ��ʾ�����㷨��
 2.�����㷨��PID����Ӧ�ÿ����㷨����ѧ��ʾ�����㷨��
 3.�߶��㷨���Ľ�����ȺЭͬ�����㷨��
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��10��28��
 *  ��    ע������2�����
 *************************************************************************/
void  ElectroMagneticMWCar (void);

/*************************************************************************
 *  �������ƣ�void TFT_Show_EleMag_Info(void)
 *  ����˵������ʾ����������Ϣ
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void TFT_Show_EleMag_Info(void);
#endif /* SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_ */
