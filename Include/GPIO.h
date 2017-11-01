#pragma once

#include "stm32l072xx.h"
#include "utility.h"


//Input Floating Mode
#define  GPIO_MODE_INPUT                        ((uint32_t)0x00000000U)

//Output Push Pull Mode
#define  GPIO_MODE_OUTPUT_PP                    ((uint32_t)0x00000001U)

//Alternate Function Push Pull Mode
#define  GPIO_MODE_AF_PP                        ((uint32_t)0x00000002U)

//Analog Mode
#define  GPIO_MODE_ANALOG                       ((uint32_t)0x00000003U)


//range up to 0.4 MHz, please refer to the product datasheet
#define  GPIO_SPEED_FREQ_LOW                    ((uint32_t)0x00000000U)

//range 0.4 MHz to 2 MHz, please refer to the product datasheet
#define  GPIO_SPEED_FREQ_MEDIUM                 ((uint32_t)0x00000001U)

//range   2 MHz to 10 MHz, please refer to the product datasheet
#define  GPIO_SPEED_FREQ_HIGH                   ((uint32_t)0x00000002U)

//range  10 MHz to 35 MHz, please refer to the product datasheet
#define  GPIO_SPEED_FREQ_VERY_HIGH              ((uint32_t)0x00000003U)


//No Pull-up or Pull-down activation
#define  GPIO_NOPULL                            ((uint32_t)0x00000000U)

//Pull-up activation
#define  GPIO_PULLUP                            ((uint32_t)0x00000001U)

//Pull-down activation
#define  GPIO_PULLDOWN                          ((uint32_t)0x00000002U)





void GPIO_Pin_Mode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Mode)
{
  //Every 2 bit represent 1 pin
  Bitfield_Modify (GPIOx->MODER, GPIO_MODER_MODE0_Msk << (Pin * 2), Mode << (Pin * 2));
}


void GPIO_Pin_Pull (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Pull)
{
  //Every 2 bit represent 1 pin
  Bitfield_Modify (GPIOx->PUPDR, GPIO_PUPDR_PUPD0_Msk << (Pin * 2), Pull << (Pin * 2));
}


void GPIO_Pin_Speed (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Speed)
{
  //Every 2 bit represent 1 pin
  Bitfield_Modify (GPIOx->OSPEEDR, GPIO_OSPEEDER_OSPEED0_Msk << (Pin * 2), Speed << (Pin * 2));
}


void GPIO_Pin_Set (GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  GPIOx->BSRR = 1 << Pin;
}


void GPIO_Pin_Clear (GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  GPIOx->BRR = 1 << Pin;
}


void GPIO_Pin_Toggle (GPIO_TypeDef * GPIOx, uint32_t Pin)
{
  GPIOx->ODR ^= 1 << Pin;
}


void GPIO_Pin_Output (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Output)
{
  //Every 1 bit represent 1 pin
  Bitfield_Modify (GPIOx->OTYPER, 0x1 << Pin, Output << Pin);
}


//See alternate functiona at DocID027100 Rev 4, Page 37, Table 16, Pin definition.
void GPIO_Alternate_Function (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t AF)
{
  //AFR [0] : 0, 1,  2,  3,  4,  5,  6,  7
  //AFR [1] : 8, 9, 10, 11, 12, 13, 14, 15
  //Every 4 bit represent 1 pin.
  //(Pin & 0x7)  : Wrap around 0 .. 15 to 0 .. 7.
  //(Pin >> 3)   : Map 0 .. 15 to 0 .. 1
  //(Pos * 4)    : For every 4 bit.
  //(0xF << Pos) : Clear mask
  //(AF << Pos) : Value mask
  Bitfield_Modify (GPIOA->AFR[Pin >> 3], 0xF << ((Pin & 0x7) * 4), AF << ((Pin & 0x7) * 4));
}