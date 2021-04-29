/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                                                               00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                                   00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  go                             00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_element.h"
#include "hcar_init.h"
#include "hcar_adc.h"
#include "SEEKFREE_ICM20602.h"

void hcar_start(void)
{
    aim_x=-150;
    hcar_adc_get();
    while((ad_value1 + ad_value2 >= ad_value3 + ad_value4 )||(ad_value1 + ad_value2 + ad_value3 + ad_value4 <= 90))
    {
        hcar_adc_get();
        /*aim_x-=10;*/

    }
    aim_x=0;
    aim_y=210;
    turn_sign=1;
}

void hcar_threefork30()
{
    aim_y=0;
    if(turn_over==0)
    {
        aim_z=-200;
    }

    get_icm20602_gyro_spi();
}
