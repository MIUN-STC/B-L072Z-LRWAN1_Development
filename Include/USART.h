#pragma once

#include "stm32l072xx.h"
#include "utility.h"


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


#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

int _write(int file, char *data, int len)
{
  USART_Transmit8v_Blocking (USART2, (uint8_t *) data, (uint32_t) len);
  // return # of bytes written - as best we can tell
  return len;
}