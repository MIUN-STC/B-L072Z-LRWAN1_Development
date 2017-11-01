#pragma once

#include "stm32l072xx.h"
#include "utility.h"


//SYSCFG external interrupt configuration register
//Managing external interrupts line multiplexing to the internal edge detector
void SYSCFG_Source_Input (SYSCFG_TypeDef * SYSCFGx, GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  uint32_t Port_Index;
  switch ((intptr_t)GPIOx)
  {
    case (intptr_t)GPIOA:
      Port_Index = 0;
      break;
    case (intptr_t)GPIOB:
      Port_Index = 1;
      break;
    case (intptr_t)GPIOC:
      Port_Index = 2;
      break;
    case (intptr_t)GPIOD:
      Port_Index = 3;
      break;
    case (intptr_t)GPIOE:
      Port_Index = 4;
      break;
    default:
      while (1) {};
      break;
  }
  //Page 257.
  //10.2.8 SYSCFG register map
  //AFR [0] :  0,  1,  2,  3
  //AFR [1] :  4,  5,  6,  7
  //AFR [2] :  8,  9, 10, 11
  //AFR [3] : 12, 13, 14, 15
  //Every 4 bit represent 1 pin.
  //(Pin & 0x3)  : Wrap around 0 .. 15 to 0 .. 3.
  //(Pin >> 2)   : Map 0 .. 15 to 0 .. 4
  //(Pos * 4)    : For every 4 bit.
  //(0xF << Pos) : Clear mask
  //(Port_Index << Pos) : Value mask
  Bitfield_Modify (SYSCFG->EXTICR [Pin >> 2], 0xF << ((Pin & 0x3) * 4), Port_Index << ((Pin & 0x3) * 4));
}