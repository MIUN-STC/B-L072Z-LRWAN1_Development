#pragma once

#include "stm32l072xx.h"
#include "utility.h"


void EXTI_Interrupt_Unmask (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->IMR |= 1 << Pin;
}


void EXTI_Rising_Edge (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->RTSR |= 1 << Pin;
}