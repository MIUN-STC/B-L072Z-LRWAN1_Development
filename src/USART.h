#pragma once

#include <stdint.h>
#include "stm32l072xx.h"
#include "utility.h"


//See alternate function at DocID027100 Rev 4, Page 37, Table 16, Pin definition.
#define STLINK_USART USART2
#define STLINK_USART_RX_PORT GPIOA
#define STLINK_USART_RX_PIN 2
#define STLINK_USART_RX_EXTI_LINE 2
#define STLINK_USART_RX_AF 4
#define STLINK_USART_TX_PORT GPIOA
#define STLINK_USART_TX_PIN 3
#define STLINK_USART_TX_EXTI_LINE 3
#define STLINK_USART_TX_AF 4

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
  while (*CString != 0)
  {
    USART_Transmit8_Blocking (USARTx, (uint8_t)*CString);
    CString = CString + 1;
  }
}


