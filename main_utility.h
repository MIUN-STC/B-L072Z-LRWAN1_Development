#pragma once

#include <stdint.h>
#include "stm32l072xx.h"

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define  GPIO_MODE_INPUT                        ((uint32_t)0x00000000U)   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP                    ((uint32_t)0x00000001U)   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_AF_PP                        ((uint32_t)0x00000002U)   /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_ANALOG                       ((uint32_t)0x00000003U)   /*!< Analog Mode  */

#define  GPIO_SPEED_FREQ_LOW              ((uint32_t)0x00000000U)  /*!< range up to 0.4 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_MEDIUM           ((uint32_t)0x00000001U)  /*!< range 0.4 MHz to 2 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_HIGH             ((uint32_t)0x00000002U)  /*!< range   2 MHz to 10 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_VERY_HIGH        ((uint32_t)0x00000003U)  /*!< range  10 MHz to 35 MHz, please refer to the product datasheet */

#define  GPIO_NOPULL        ((uint32_t)0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        ((uint32_t)0x00000001U)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      ((uint32_t)0x00000002U)   /*!< Pull-down activation                */



#define Bitfield_Modify(Result, Mask, Value) ((Result) = ((Result) & ~(Mask)) | ((Value) & (Mask)))


void GPIO_Pin_Mode (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Mode)
{
  //Every 2 bit represent 1 pin
  Bitfield_Modify (GPIOx->MODER, 0x3 << (Pin * 2), Mode << (Pin * 2));
}


void GPIO_Pin_Pull (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Pull)
{
  //Every 2 bit represent 1 pin
  Bitfield_Modify (GPIOx->PUPDR, 0x3 << (Pin * 2), Pull << (Pin * 2));
}


void GPIO_Pin_Speed (GPIO_TypeDef * GPIOx, uint32_t Pin, uint32_t Speed)
{
  //Every 2 bit represent 1 pin
  Bitfield_Modify (GPIOx->OSPEEDR, 0x3 << (Pin * 2), Speed << (Pin * 2));
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
  //AFR [Pin / 8]
  //AFR [Pin >> 3]
  //Every 4 bit represent 1 pin.
  Bitfield_Modify (GPIOA->AFR[Pin >> 3], 0xF << (Pin * 4), AF << (Pin * 4));
}


void EXTI_Interrupt_Unmask (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->IMR |= 1 << Pin;
}
