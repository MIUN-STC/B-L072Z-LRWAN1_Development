#pragma once

#include <stdint.h>
#include "stm32l072xx.h"


#define Bitfield_Modify(Result, Mask, Value) ((Result) = ((Result) & ~(Mask)) | ((Value) & (Mask)))


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


void EXTI_Interrupt_Unmask (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->IMR |= 1 << Pin;
}


void EXTI_Rising_Edge (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->RTSR |= 1 << Pin;
}


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


void USART_Transmit8_Blocking (USART_TypeDef * USARTx, uint8_t Data)
{
  while ((USARTx->ISR & USART_ISR_TXE) == 0){}
  USARTx->TDR = Data;
}


void USART_Transmit8v_Blocking (USART_TypeDef * USARTx, uint8_t * Data, uint32_t Count)
{
  for (uint32_t I = 0; I < Count; I = I + 1)
  {
    USART_Transmit8_Blocking (USARTx, Data [I]);
  }
}


void USART_Transmit_CString_Blocking (USART_TypeDef * USARTx, char * CString)
{
  while (*CString != '\0')
  {
    USART_Transmit8_Blocking (USARTx, (uint8_t)*CString);
    CString = CString + 1;
  }
}


uint8_t SPI_Transfer8_Blocking (SPI_TypeDef * SPIx, uint8_t Outdata)
{
  uint8_t Indata;
  volatile uint8_t *DR = (volatile uint8_t*)&(SPIx->DR);
  //Wait until transmit buffer is empty.
  //The SPI_SR_TXE flag (Tx buffer empty) is set when the data
  //are transferred from the Tx buffer to the shift register.
  //It indicates that the internal Tx buffer is ready to be loaded with the next data.
  while ((SPIx->SR & SPI_SR_TXE) == 0){}
  //Writing to DR writes to TX buffer.
  *DR = Outdata;
  //Wait until receive buffer is empty.
  //The SPI_SR_RXNE flag (Rx buffer not empty) is set on the last sampling clock edge,
  //when the data are transferred from the shift register to the Rx buffer.
  while ((SPIx->SR & SPI_SR_RXNE) == 0){}
  //Reading from DR reads from RX buffer.
  Indata = *DR;
  return Indata;
}


void SPI_Init (SPI_TypeDef * SPIx)
{
  // Enable the peripheral clock SPI1
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  
  //Controlling NSS:
  //SPI_CR1_SSM = 0 and SPI_CR2_SSOE = 1 and SPI_CR1_SPE = 0 => NSS = 0
  //SPI_CR1_SSM = 0 and SPI_CR2_SSOE = 1 and SPI_CR1_SPE = 1 => NSS = 1

  //Configure SPI1 in master
  //Master selection, BR: Fpclk/256 (due to C13 on the board, SPI_CLK is set to the minimum)
  //CPOL and CPHA at zero (rising first edge), 8-bit data frame
  SPIx->CR1 =
  SPI_CR1_MSTR |
  SPI_CR1_BR |
  //SPI_CR1_SSM |
  0;
  
  SPIx->CR1 &= ~SPI_CR1_SSM;

  //Slave select output enabled, RXNE IT
  SPIx->CR2 =
  SPI_CR2_SSOE |
  //SPI_CR2_RXNEIE|
  0;

  //Enable/disable SPI
  //SPIx->CR1 |= SPI_CR1_SPE;
  SPIx->CR1 &= ~SPI_CR1_SPE;
}
