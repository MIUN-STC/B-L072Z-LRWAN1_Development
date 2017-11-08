#pragma once

#include "stm32l072xx.h"
#include "utility.h"

void RCC_Enable_LSI_Blocking ()
{
  //Enable the LSI
  RCC->CSR |= RCC_CSR_LSION;

  //Wait while it is not ready
  while ((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY) {}
}
