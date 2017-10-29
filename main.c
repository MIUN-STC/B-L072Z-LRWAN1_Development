#include "stm32l072xx.h"
#include "system_stm32l0xx.h"
#include "utility.h"
#include "sx1276.h"
#include "B-L072Z-LRWAN1.h"


uint32_t Timeon_LD1_GREEN = 0;
uint32_t Timeon_LD2_GREEN = 0;
uint32_t Timeon_LD3_BLUE = 0;
uint32_t Timeon_LD4_RED = 0;


int main(void)
{
  
  Board_Init ();
  Board_Radio_Init ();
  

  USART_Transmit_CString_Blocking (STLINK_USART, "Reset RADIO\r\n");
  GPIO_Pin_Set (RADIO_RESET_PORT, RADIO_RESET_PIN);
  Delay1 (1000);
  GPIO_Pin_Clear (RADIO_RESET_PORT, RADIO_RESET_PIN);
  Delay1 (1000);
  
  
  
  uint32_t Timeout = 0;
  while (1) 
  {
    if (Timeout == 0)
    {
      GPIO_Pin_Clear (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
      if (Timeon_LD1_GREEN > 0) {GPIO_Pin_Clear (GPIOB, LED_LD1_GREEN_PIN);Timeon_LD1_GREEN--;}
      if (Timeon_LD3_BLUE > 0) {GPIO_Pin_Clear (GPIOB, LED_LD3_BLUE_PIN);Timeon_LD3_BLUE--;}
      if (Timeon_LD4_RED > 0) {GPIO_Pin_Clear (GPIOB, LED_LD4_RED_PIN);Timeon_LD4_RED--;}
      Timeout = 2000000;
    }
    else if (Timeout == 1000000)
    {
      GPIO_Pin_Set (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
      if (Timeon_LD1_GREEN > 0) {GPIO_Pin_Set (GPIOB, LED_LD1_GREEN_PIN);}
      if (Timeon_LD3_BLUE > 0) {GPIO_Pin_Set (GPIOB, LED_LD3_BLUE_PIN);}
      if (Timeon_LD4_RED > 0) {GPIO_Pin_Set (GPIOB, LED_LD4_RED_PIN);}
    }
    Timeout--;
  }
}


void USART2_IRQHandler(void)
{
  //USART_ISR_TC bit is set by hardware if the transmission of a frame containing data is complete and if TXE is set. 
  //An interrupt is generated if TCIE=1 in the USART_CR1 register. 
  //It is cleared by software, writing 1 to the TCCF in the USART_ICR register or by a write to the USART_TDR register.
  //An interrupt is generated if TCIE=1 in the USART_CR1 register.
  if((USART2->ISR & USART_ISR_TC) == USART_ISR_TC)
  {
    USART2->ICR = USART_ICR_TCCF;
    /*
    if(send == sizeof(stringtosend))
    {
      send=0;
      //Clear transfer complete flag when there is no more to send.
      USART2->ICR = USART_ICR_TCCF;
    }
    else
    {
      //Clear transfer complete flag by sending a new char.
      USART2->TDR = stringtosend[send++];
    }
    */
  }
  
  if((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
    uint8_t Data;
    Data = (uint8_t)(USART2->RDR); /* Receive data, clear flag */   
    switch(Data)
    {
      case 'b':
      case 'B': 
	GPIO_Pin_Toggle (GPIOB, LED_LD3_BLUE_PIN);
	break;
      default: break;
    }
    //USART_Send_Value_Blocking (USART2, chartoreceive);
    //SPI_Send_Value_Blocking (SPI1, chartoreceive);
  }
}





void Board_Button ()
{
  uint8_t R;
  if ((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)
  {
    GPIO_Pin_Clear (RADIO_NSS_PORT, RADIO_NSS_PIN);
    R = SPI_Transfer8 (SPI1, SX1276_LORA_VERSION);
    USART_Transmit8_Blocking (USART2, R);
    R = SPI_Transfer8 (SPI1, 0x00);
    USART_Transmit8_Blocking (USART2, R);
    GPIO_Pin_Set (RADIO_NSS_PORT, RADIO_NSS_PIN);
    Timeon_LD1_GREEN++;
  }
}
