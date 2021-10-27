#include "user_timer.h"




static EventInt EventInterrupt[TIM_MAX_HW][MAX_EVENT_ALL_TIM];

void Init_Timer(Tim_CfgTypes *Timer_Cfg)
{
     TIM_TypeDef *hw;
     uint32_t tmpcr1 = 0U;
     uint32_t tmpccmrx;
     uint32_t tmpdier;
     uint32_t tmpccer;
     uint8_t  chanel;
    
     Tim_OCCMode_CfgTypes  OCCMode_Cfg_local;
     Tim_ICCMode_CfgTypes  ICCMode_Cfg_local;
     Tim_TRIGMode_CfgTypes TRIGMode_Cfg_local;
    
     for(uint8_t i = 0; i < Timer_Cfg->Number_HW; i++) 
     {
         /*get base address of hw*/
         switch(Timer_Cfg->Hw_Config[i].HW)
         {
             case TIMER1_HW : 
                 hw = TIM1;
                 __HAL_RCC_TIM1_CLK_ENABLE();
                 break;
             case TIMER2_HW : 
                 hw = (TIM_TypeDef *)TIM2;
                 __HAL_RCC_TIM2_CLK_ENABLE();
                 break;      
             case TIMER3_HW : 
                 hw = (TIM_TypeDef *)TIM3;
                 __HAL_RCC_TIM3_CLK_ENABLE();
                 break;
             case TIMER4_HW : 
                 //hw = (TIM_TypeDef *)TIM4;
                 break;
             default :
                 break;
         }
         /*cau hinh prescaler/period cho timer*/
         tmpcr1 = hw->CR1;
         tmpcr1 &= ~TIM_CR1_ARPE_Msk;
         tmpcr1 |= TIM_CR1_ARPE_Msk;
         hw->CR1 = tmpcr1;
         hw->PSC = (uint32_t)Timer_Cfg->Hw_Config[i].Base_Cfg->Prescaler;
         hw->ARR = (uint32_t)Timer_Cfg->Hw_Config[i].Base_Cfg->Period ;
         hw->EGR = TIM_EGR_UG_Msk;
         /* check bien enable ngat de cau hinh ngat*/
         if(Timer_Cfg->Hw_Config[i].Base_Cfg->Int_Enable == TIME_INT_ENABLE)
         {
             if((hw==TIM2)||(hw==TIM3))
             {
                 tmpdier = hw->DIER;
                 tmpdier |= TIM_DIER_UIE;
                 hw->DIER = tmpdier;
             	 HAL_NVIC_EnableIRQ(TIM2_IRQn);
             	 HAL_NVIC_EnableIRQ(TIM3_IRQn);

             	 HAL_NVIC_SetPriority(TIM3_IRQn,5u,0u);
             	 HAL_NVIC_SetPriority(TIM2_IRQn,5u,0u);
             }
             else
             {
                 
             }
         }
         /* config chanel of hw*/
         for(uint8_t j = 0; j < Timer_Cfg->Hw_Config[i].Number_Chanel; j++)
         {
             chanel=Timer_Cfg->Hw_Config[i].Chanel_Config[j].Chanel_ID;
             switch(Timer_Cfg->Hw_Config[i].Chanel_Config[j].Mode)
             {
                 case OCC_MODE :
                     /*cau hinh thanh ghi cho output compare mode*/
                                     OCCMode_Cfg_local= *(Timer_Cfg->Hw_Config[i].Chanel_Config[j].OCCMode_Cfg);               
                     hw->CCER &= ~(TIM_CCER_CC1E<<((chanel-1)*4));
                     tmpccer = hw->CCER;
                     /*cau hinh mode/compare_value/OCPolarity */
                     if(chanel<3)
                     {
                         tmpccmrx = hw->CCMR1;
                         tmpccmrx &= ~(TIM_CCMR1_OC1M<<((chanel-1)*8));
                         tmpccmrx &= ~(TIM_CCMR1_CC1S<<((chanel-1)*8));
                         tmpccmrx |= (OCCMode_Cfg_local.OCMode<<((chanel-1)*8))|(TIM_CCMR1_OC1PE<<((chanel-1)*8));
                         hw->CCMR1 = tmpccmrx;
                     }   
                     else
                     {   
                         tmpccmrx = hw->CCMR2;
                         tmpccmrx &= ~(TIM_CCMR1_OC1M<<((chanel-3)*8));
                         tmpccmrx &= ~(TIM_CCMR1_CC1S<<((chanel-3)*8));
                         tmpccmrx |= (OCCMode_Cfg_local.OCMode<<((chanel-3)*8))|(TIM_CCMR1_OC1PE<<((chanel-3)*8));
                         hw->CCMR2 = tmpccmrx;
                     }
                     tmpccer &= ~(TIM_CCER_CC1P<<((chanel-1)*4));
                     tmpccer |= (OCCMode_Cfg_local.OCPolarity<<((chanel-1)*4));
                     hw->CCER = tmpccer;
                     switch(chanel)
                     {
                         case CHANNEL1_HW : hw->CCR1 = OCCMode_Cfg_local.Compare_value;
                         break;
                         case CHANNEL2_HW : hw->CCR2 = OCCMode_Cfg_local.Compare_value;
                         break;      
                         case CHANNEL3_HW : hw->CCR3 = OCCMode_Cfg_local.Compare_value;
                         break;
                         case CHANNEL4_HW : hw->CCR4 = OCCMode_Cfg_local.Compare_value;
                         break;
                         default :
                         break;
                     }
                     break;
                 case ICC_MODE :
                     /*cau hinh thanh ghi cho input capture mode*/
                     ICCMode_Cfg_local= *(Timer_Cfg->Hw_Config[i].Chanel_Config[j].ICCMode_Cfg);
                     hw->CCER &= ~(TIM_CCER_CC1E<<((chanel-1)*4));
                     /*Reset counter value*/
                     switch(chanel)
                     {
                         case CHANNEL1_HW : hw->CCR1 = 0;
                         break;
                         case CHANNEL2_HW : hw->CCR2 = 0;
                         break;      
                         case CHANNEL3_HW : hw->CCR3 = 0;
                         break;
                         case CHANNEL4_HW : hw->CCR4 = 0;
                         break;
                         default :
                         break;
                     }
                     /*cau hinh filter/ICPrescaler/ICPolarity source input*/
                     tmpccmrx = hw->CCMR1;
                     tmpccer = hw->CCER;
                     if(chanel<3)
                     {
                         tmpccmrx = hw->CCMR1;
                         tmpccmrx &= ~(TIM_CCMR1_IC1F<<((chanel-1)*8));
                         tmpccmrx &= ~(TIM_CCMR1_IC1PSC<<((chanel-1)*8));
                         tmpccmrx |= (ICCMode_Cfg_local.ICFilter<<((chanel-1)*8))|(ICCMode_Cfg_local.ICPrescaler<<((chanel-1)*8));
                         tmpccmrx &= ~(TIM_CCMR1_CC1S<<((chanel-1)*8));
                         tmpccmrx |= ICCMode_Cfg_local.ICSelection<<((chanel-3)*8);
                         hw->CCMR1 = tmpccmrx;
                     }   
                     else
                     {   
                         tmpccmrx = hw->CCMR2;
                         tmpccmrx &= ~(TIM_CCMR1_IC1F<<((chanel-3)*8));
                         tmpccmrx &= ~(TIM_CCMR1_IC1PSC<<((chanel-3)*8));
                         tmpccmrx |= (ICCMode_Cfg_local.ICFilter<<((chanel-3)*8))|(ICCMode_Cfg_local.ICPrescaler<<((chanel-3)*8));
                         tmpccmrx &= ~(TIM_CCMR1_CC1S<<((chanel-3)*8));
                         tmpccmrx |= ICCMode_Cfg_local.ICSelection<<((chanel-3)*8);
                         hw->CCMR2 = tmpccmrx;
                     }
                     tmpccer  &= ~(TIM_CCER_CC1P<<((chanel-1)*4));
                     tmpccer  |= (ICCMode_Cfg_local.ICPolarity<<((chanel-1)*4));
                     hw->CCER  = tmpccer;
                     break;
                 case TRIGER_MODE :
                     /*cau hinh thanh ghi cho trigger  mode*/
                     /*get config*/
                     TRIGMode_Cfg_local= *(Timer_Cfg->Hw_Config[i].Chanel_Config[j].TRIGMode_Cfg);
                     /*cau hinh compare_value */
                     switch(chanel)
                     {
                         case CHANNEL1_HW : hw->CCR1 = TRIGMode_Cfg_local.Compare_value;
                         break;
                         case CHANNEL2_HW : hw->CCR2 = TRIGMode_Cfg_local.Compare_value;
                         break;      
                         case CHANNEL3_HW : hw->CCR3 = TRIGMode_Cfg_local.Compare_value;
                         break;
                         case CHANNEL4_HW : hw->CCR4 = TRIGMode_Cfg_local.Compare_value;
                         break;
                         default :
                         break;
                     }
                     /*Enable source trigger dma*/
                     tmpdier = hw->DIER;
                     tmpdier &= ~(TIM_DIER_CC1DE<<(chanel+8));
                     tmpdier |= TIM_DIER_CC1DE<<(chanel+8);
                     hw->DIER =tmpdier;
                     break;
                 case DEFAULT_MODE : /*Do nothing*/                                     
                     break;
                 default :
                     break;
             }                
         }
         /*Enable timer*/
         hw->CR1|=TIM_CR1_CEN;
     }
}


void Tim_SetOuput_Compare(uint32_t Compare_value,uint8_t tim_instance,uint8_t chanel)
{
    TIM_TypeDef *hw;
    /*get base address of hw*/
    switch(tim_instance)
    {
        case TIMER1_HW : 
            hw = TIM1;
            break;
        case TIMER2_HW : 
            hw = TIM2;
            break;      
        case TIMER3_HW : 
            hw = TIM2;
            break;
        case TIMER4_HW : 
            hw = TIM2;
            break;
        default :
            break;
    }
    /*set compare value*/
    hw->CCER &= ~(TIM_CCER_CC1E<<((chanel-1)*4));
    switch(chanel)
    {
        case CHANNEL1_HW : hw->CCR1 = Compare_value;
        break;
        case CHANNEL2_HW : hw->CCR2 = Compare_value;
        break;      
        case CHANNEL3_HW : hw->CCR3 = Compare_value;
        break;
        case CHANNEL4_HW : hw->CCR4 = Compare_value;
        break;
        default :
        break;
    }
    /*enable output compare*/
    hw->CCER |= (TIM_CCER_CC1E<<((chanel-1)*4));
}

void Tim_GetIuput_Capture(uint32_t *capture_value,uint8_t tim_instance,uint8_t chanel)
{
    TIM_TypeDef *hw;
    /*get base address of hw*/
    switch(tim_instance)
    {
        case TIMER1_HW : 
            hw = TIM1;
            break;
        case TIMER2_HW : 
            hw = TIM2;
            break;      
        case TIMER3_HW : 
            hw = TIM3;
            break;
        case TIMER4_HW : 
            //hw = TIM4;
            break;
        default :
            break;
    }
    /*enable input capture*/
    hw->CCER |= (TIM_CCER_CC1E<<((chanel-1)*4));
    /*wait */
    while((hw->SR&&((TIM_SR_CC1IF<<(chanel-1))))==0);
    /*get capture value*/
    switch(chanel)
    {
        case CHANNEL1_HW : *capture_value=hw->CCR1;
        break;
        case CHANNEL2_HW : *capture_value=hw->CCR2;
        break;      
        case CHANNEL3_HW : *capture_value=hw->CCR3;
        break;
        case CHANNEL4_HW : *capture_value=hw->CCR4;
        break;
        default :
        break;
    }
}


void Tim_SetTriger(uint8_t tim_instance,uint8_t chanel)
{
    TIM_TypeDef *hw;
    /*get base address of hw*/
    switch(tim_instance)
    {
        case TIMER1_HW : 
            hw = TIM1;
            break;
        case TIMER2_HW : 
            hw = TIM2;
            break;      
        case TIMER3_HW : 
            hw = TIM3;
            break;
        case TIMER4_HW : 
            //hw = TIM4;
            break;
        default :
            break;
    }
    /*enable trigger*/
    hw->CCER |= (TIM_CCER_CC1E<<((chanel-1)*4));
}


void TIM_CMD(TIM_TypeDef *hw, uint8_t state)
{
	if(state == ENABLE)
	{
		hw->CR1 |= TIM_CR1_CEN;
	}
	else
	{
		hw->CR1 &= ~TIM_CR1_CEN;
	}
}



Std_ReturnType Tim_AddEvent(uint8_t tim_instance,void (*fun)())
{
    static uint8_t count_event[TIM_MAX_HW] ;
    /*Add callback func*/
    EventInterrupt[tim_instance][count_event[tim_instance]] = fun;
      if(++ count_event[tim_instance] >= Max_Event_Time[tim_instance])
        {
            count_event[tim_instance] = Max_Event_Time[tim_instance];
            return E_NOT_OK;
        }
        return E_OK;
}


void Timer_ResetEvent(uint8_t TIMER_HW)
{
    /*Remove callback func*/
    for(uint8_t i=0;i<Max_Event_Time[TIMER_HW];i++)
    {
        EventInterrupt[TIMER_HW][i] = NULL;
    }
}

void TIM2_IRQHandler()
{
    /* Run call back func */
    for(uint8_t i = 0; i< Max_Event_Time[TIMER2_HW]; i++)
    {
        if(EventInterrupt[TIMER2_HW][i]!=NULL)
        EventInterrupt[TIMER2_HW][i]();
    }
    TIM2->SR&=(~TIM_SR_UIF);

}

void TIM3_IRQHandler()
{
    /* Run call back func */
    for(uint8_t i = 0; i< Max_Event_Time[TIMER3_HW]; i++)
    {
        if(EventInterrupt[TIMER3_HW][i]!=NULL)
        EventInterrupt[TIMER3_HW][i]();
    }
    TIM3->SR&=(~TIM_SR_UIF);
}

void TIM4_IRQHandler()
{
    /* Run call back func */
    for(uint8_t i = 0; i < Max_Event_Time[TIMER4_HW]; i++)
    {
        if(EventInterrupt[TIMER4_HW][i]!=NULL)
        EventInterrupt[TIMER4_HW][i]();
    }
   // TIM4->SR&=(~TIM_SR_UIF);
}
