#ifndef __TIM_CFG_H__
#define __TIM_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../user_timer_types.h"

#define  MAX_EVENT_ALL_TIM    5

#define  MAX_EVENT_TIM2   4
#define  MAX_EVENT_TIM3   4
#define  MAX_EVENT_TIM4   4

extern Tim_OCCMode_CfgTypes OCCMode_Cfg;
extern Tim_CfgTypes         Timer2_Cfg;
extern const uint8_t        Max_Event_Time[TIM_MAX_HW];

#ifdef __cplusplus
}
#endif

#endif
