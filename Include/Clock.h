#pragma once

#include "stm32l072xx.h"
#include "utility.h"


#define MSI_FREQUENCY_65536Hz     0
#define MSI_FREQUENCY_131072Hz    1
#define MSI_FREQUENCY_262144Hz    2
#define MSI_FREQUENCY_524288Hz    3
#define MSI_FREQUENCY_1048000Hz   4
#define MSI_FREQUENCY_2097000Hz   5
#define MSI_FREQUENCY_4194000Hz   6
#define MSI_FREQUENCY_NOTALLOWED  7


void Clock_MSI_Frequency (uint32_t Frequency)
{
  Bitfield_Modify (RCC->ICSCR, RCC_ICSCR_MSIRANGE_Msk, Frequency << RCC_ICSCR_MSIRANGE_Pos);
}


void Clock_LSI_Enable ()
{
  //Enable the LSI clock
  RCC->CSR |= RCC_CSR_LSION;
  //Wait while it is not ready
  while((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY) {}
}



