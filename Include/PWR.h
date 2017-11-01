#pragma once

#include "stm32l072xx.h"
#include "utility.h"


#define PWR_REGULATOR_FORBIDDEN   0
#define PWR_REGULATOR_120mV       1
#define PWR_REGULATOR_150mV       2
#define PWR_REGULATOR_180mV       3


void PWR_Regulator_Voltage (uint32_t Voltage)
{
  Bitfield_Modify (PWR->CR, PWR_CR_VOS_Msk, Voltage << PWR_CR_VOS_Pos);
}