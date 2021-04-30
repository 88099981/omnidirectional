


#include "common.h"
#include "zf_uart.h"
#include "ch32v10x.h"

#ifndef _SEEKFREE_7725_h
#define _SEEKFREE_7725_h

#define OV7725_UART_W                   160
#define OV7725_UART_H                   120

#define OV7725_UART_SIZE                (OV7725_UART_W * OV7725_UART_H /8 )
#define OV7725_UART_DMA_NUM             (OV7725_UART_SIZE )


#define OV7725_UART_DMA_CH              DMA1_Channel4          //TIM����DMAͨ����ֹ�����޸�
#define OV7725_UART_DMA_IRQN            DMA1_Channel4_IRQn     //DMA�ж�ͨ��


#define OV7725_UART_COF_UART            UART_2                 //С������ô���
#define OV7725_UART_COF_UART_TX         UART2_TX_A2            //С���TX��������
#define OV7725_UART_COF_UART_RX         UART2_RX_A3            //С���RX��������

#define OV7725_UART_PCLK                A12                    //GPIO����TIM���Ž�ֹ�����޸�

#define OV7725_UART_VSYNC_PIN           A1                     //���ж�����
#define OV7725_UART_VSYNC_IRQN          EXTI1_IRQn             //�жϺ�

//--------------------------------------------------------------------------------------------------
//        ����ͷ���ݽӿ�
//        GPIO_IDR_ADDR(2,0)
//        ��һ������2������ʹ��C�ڣ�����ѡ��0��1��2��3�ֱ��ӦGPIO_PIN(A,0)��GPIO_PIN(B,0)��GPIO_PIN(C,0)��GPIO_PIN(D,0)
//        �ڶ�������0������ʹ�õ���C0 - C7��,
//        ����ѡ��0��1��2��3���ֱ��Ӧ����    �ö˿ڵ�X0-X7��X8-X15��X16-X23��X24-X31(����x������ǵ�һ����ĸ���������ڵľ���C0-C7)
//--------------------------------------------------------------------------------------------------
#define OV7725_UART_DATA_PIN            GPIO_IDR_ADDR(2, 0)

typedef enum
{
    OV7725_INIT     = 0x00,        
    OV7725_RESERVE,                
    OV7725_CONTRAST,                
    OV7725_FPS,                     
    OV7725_COL,                    
    OV7725_ROW,                     
    OV7725_CONFIG_FINISH,         

    OV7725_GET_WHO_AM_I= 0xEF,
    OV7725_GET_STATUS  = 0xF1,
    OV7725_GET_VERSION = 0xF2,

    OV7725_SET_ADDR    = 0xFE,
    OV7725_SET_DATA    = 0xFF,
}OV7725_CMD;



extern uint8 ov7725_uart_image_bin[OV7725_UART_W/8][OV7725_UART_H];
extern uint8 ov7725_uart_image_dec[OV7725_UART_H][OV7725_UART_W];
extern uint8 ov7725_uart_finish_flag;


uint8_t ov7725_uart_init(void);
void seekfree_sendimg_7725(UARTN_enum uartn, void *imgaddr, uint32 imgsize);
void ov7725_cof_uart_interrupt(void);
void ov7725_uart_dma(void);
void ov7725_uart_vsync(void);


#endif



