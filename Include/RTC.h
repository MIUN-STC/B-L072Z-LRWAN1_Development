#pragma once

#include "stm32l072xx.h"
#include "utility.h"


//Enable write access.
//After RTC domain reset, all the RTC registers are write-protected.
void RTC_Unlock ()
{
  //The following steps are required to unlock the write protection on all the RTC registers
  //except for RTC_TAMPCR, RTC_BKPxR, RTC_OR and RTC_ISR[13:8].
  //1. Write ‘0xCA’ into the RTC_WPR register.
  //2. Write ‘0x53’ into the RTC_WPR register.
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
}


//Disable write access.
//After RTC domain reset, all the RTC registers are write-protected
void RTC_Lock ()
{
  //Writing a wrong key reactivates the write protection.
  RTC->WPR = 0xFE;
  RTC->WPR = 0x64;
}


void RTC_Wakeup_Timer_Blocking (uint16_t Value)
{
  //Write access for RTC regsiters
  RTC_Unlock ();

  //Disable wake up timerto modify it
  RTC->CR &=~ RTC_CR_WUTE;

  //Wait until it is allowed to modify wake up reload value
  while((RTC->ISR & RTC_ISR_WUTWF) != RTC_ISR_WUTWF) {}

  //This register can be written only when RTC_ISR_WUTWF is set to 1 in RTC_ISR.
  //Modify wake upvalue reload counter to have a wake up each 1Hz
  RTC->WUTR = Value;

  //Enable wake up counter and wake up interrupt
  RTC->CR = RTC_CR_WUTE | RTC_CR_WUTIE;

  //Disable write access
  RTC_Lock ();
}



__INLINE static void Configure_RTC(void)
{
  //Enable PWR clock 
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;

  //Enable write in RTC domain control register 
  PWR->CR |= PWR_CR_DBP;

  //LSI for RTC clock 
  RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | RCC_CSR_RTCSEL_1;

  //Disable PWR clock
  RCC->APB1ENR &=~ RCC_APB1ENR_PWREN;

  //Modify wake upvalue reload counter to have a wake up each 1Hz
  RTC_Wakeup_Timer_Blocking (0x9C0);
  
  EXTI->IMR |= EXTI_IMR_IM20;
  EXTI->RTSR |= EXTI_RTSR_TR20;
  NVIC_SetPriority (RTC_IRQn, 0);
  NVIC_EnableIRQ (RTC_IRQn);
}

/**
  * Brief   This function configures RTC.
  * Param   uint32_t New time
  * Retval  None
  */
__INLINE void Init_RTC(uint32_t Time)
{
  /* RTC init mode */
  /* Configure RTC */
  /* (1) Write access for RTC registers */
  /* (2) Enable init phase */
  /* (3) Wait until it is allow to modify RTC register values */
  /* (4) set prescaler, 40kHz/64 => 625 Hz, 625Hz/625 => 1Hz */
  /* (5) New time in TR */
  /* (6) Disable init phase */
  /* (7) Disable write access for RTC registers */
  RTC_Unlock ();
  RTC->ISR = RTC_ISR_INIT; /* (2) */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) {}
  RTC->PRER = 0x003F0270; /* (4) */
  RTC->TR = RTC_TR_PM | Time; /* (5) */
  RTC->ISR =~ RTC_ISR_INIT; /* (6) */
  RTC_Lock ();
}




