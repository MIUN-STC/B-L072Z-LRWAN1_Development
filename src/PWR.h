#pragma once

#include "stm32l072xx.h"
#include "utility.h"


#define PWR_REGULATOR_FORBIDDEN   0
#define PWR_REGULATOR_120mV       1
#define PWR_REGULATOR_150mV       2
#define PWR_REGULATOR_180mV       3


//Voltage scaling range selection
//An embedded linear voltage regulator supplies all the digital circuitries except for the
//Standby circuitry. The regulator output voltage (VCORE) can be programmed by software to
//three different ranges within 1.2 - 1.8 V.
void PWR_Regulator_Voltage (uint32_t Voltage)
{
  //The PWR_CR_VOS bits are used to select the internal regulator voltage range.
  Bitfield_Modify (PWR->CR, PWR_CR_VOS, Voltage << PWR_CR_VOS_Pos);

  //The PWR_CSR_VOSF bit indicates that the regulator has reached the voltage level defined with bits VOS of PWR_CR register.
  while ((PWR->CR & PWR_CSR_VOSF) != 0) {};
}
