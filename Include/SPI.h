#pragma once

#include "stm32l072xx.h"
#include "utility.h"


#define SPI_BUADRATE_PCLK_DIV2   0 
#define SPI_BUADRATE_PCLK_DIV4   1
#define SPI_BUADRATE_PCLK_DIV8   2
#define SPI_BUADRATE_PCLK_DIV16  3
#define SPI_BUADRATE_PCLK_DIV32  4
#define SPI_BUADRATE_PCLK_DIV64  5
#define SPI_BUADRATE_PCLK_DIV128 6
#define SPI_BUADRATE_PCLK_DIV256 7


void SPI_Buadrate (SPI_TypeDef * SPIx, uint32_t Buadrate)
{
  Bitfield_Modify (SPIx->CR1, SPI_CR1_BR_Msk, Buadrate << SPI_CR1_BR_Pos);
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
  //SPI_CR1_BR |
  //SPI_CR1_SSM |
  0;
  
  SPI_Buadrate (SPIx, SPI_BUADRATE_PCLK_DIV2);
  
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