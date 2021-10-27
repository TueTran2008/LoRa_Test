#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./cfg/user_timer_cfg.h"
#include "std_types.h"
#include "main.h"

#define  TIMER1_HW        1
#define  TIMER2_HW        2
#define  TIMER3_HW        3
#define  TIMER4_HW        4
#define  CHANNEL1_HW      1
#define  CHANNEL2_HW      2
#define  CHANNEL3_HW      3
#define  CHANNEL4_HW      4
#define  CHANNELLT_MODE   0
#define  OCC_MODE         1
#define  ICC_MODE         2
#define  TRIGER_MODE      3
#define  DEFAULT_MODE     4
#define  TIME_INT_DISABLE 0
#define  TIME_INT_ENABLE  1

extern void Timer_ResetEvent(uint8_t TIMER_HW);
extern void Init_Timer(Tim_CfgTypes *Timer_Cfg);
extern void Tim_SetOuput_Compare(uint32_t compare_value,uint8_t tim_instance,uint8_t chanel);
extern void Tim_GetIuput_Capture(uint32_t *capture_value,uint8_t tim_instance,uint8_t chanel);
extern void Tim_SetTriger(uint8_t tim_instance,uint8_t chanel);
extern void TIM_CMD(TIM_TypeDef *hw, uint8_t state);
Std_ReturnType Tim_AddEvent(uint8_t tim_instance,void (*fun)());

#ifdef __cplusplus
}
#endif
#endif

