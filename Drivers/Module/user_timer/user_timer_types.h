#ifndef TIM_TYPES_H
#define TIM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define TIM_MAX_HW        5
#define TIM_MAX_CHANEL    8

typedef void (*EventInt)(void);

typedef struct
{
    volatile uint32_t Prescaler;     
    volatile uint32_t Period;
             uint8_t  Int_Enable;
}Tim_BaseCfgTypes;

typedef struct
{
    uint32_t Compare_value;
    uint32_t OCMode;
    uint32_t OCPolarity;
    uint32_t OCFastMode;
}Tim_OCCMode_CfgTypes;

typedef struct
{
    uint32_t ICPolarity;
    uint32_t ICSelection;
    uint32_t ICPrescaler;
    uint32_t ICFilter;
    
    
}Tim_ICCMode_CfgTypes;

typedef struct
{
    uint32_t Compare_value;
    uint8_t  Source_Triger;
    
}Tim_TRIGMode_CfgTypes;

typedef struct
{
    uint8_t                Mode;
    uint8_t                Chanel_ID;
    Tim_OCCMode_CfgTypes*  OCCMode_Cfg;
    Tim_ICCMode_CfgTypes*  ICCMode_Cfg;
    Tim_TRIGMode_CfgTypes* TRIGMode_Cfg;
    
}Tim_Chanel_CfgTypes;

typedef struct
{
    uint8_t                HW;
    uint8_t                Number_Chanel;
    Tim_BaseCfgTypes*      Base_Cfg;
    Tim_Chanel_CfgTypes*   Chanel_Config;

}Tim_HW_CfgTypes;

typedef struct
{
    uint8_t                Number_HW;
    Tim_HW_CfgTypes*       Hw_Config;
    
}Tim_CfgTypes;

#ifdef __cplusplus
}
#endif

#endif /* TIM_TYPES_H */
