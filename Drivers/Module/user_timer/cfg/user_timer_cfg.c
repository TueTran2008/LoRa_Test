#include "../user_timer.h"
#include "main.h"

const uint8_t Max_Event_Time[TIM_MAX_HW]={4,4,4,4}; /*max callback func per timer*/

Tim_TRIGMode_CfgTypes TRIGMode_Cfg =
{
    0xFFFF,             /*compare value*/
    CHANNEL1_HW         /*source trgger*/
};

Tim_Chanel_CfgTypes Tim2_chanel2_Cfg[2] =
{
    /*chanel config 1*/
    {DEFAULT_MODE,      /*mode*/
    2,                  /*chanel*/
    NULL,               /*pointer of OCC_MODE config*/
    NULL,               /*pointer of ICC_MODE config*/
    &TRIGMode_Cfg},     /*pointer of TRIGMode config*/
};

Tim_BaseCfgTypes Tim2_BaseCfg =
{
    99,
	7199,/*prescaler*/
    TIME_INT_ENABLE      /*enable or disable interrupt*/
};

Tim_HW_CfgTypes Tim2_HW_Cfg[3]=
{
    /*hw config 1*/
    {TIMER2_HW,         /*instance of hw*/
    2,                  /*number chanel config*/
    &Tim2_BaseCfg,      /*pointer of base config*/
    Tim2_chanel2_Cfg},  /*pointer of chanel config*/
};

Tim_CfgTypes  Timer2_Cfg = {
    1,                  /*number of hw config*/
    Tim2_HW_Cfg         /*pointer of hw config*/
};
