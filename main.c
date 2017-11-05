/*
https://feabhas.gitbooks.io/stm32f4-cmsis/content/gpio_as_an_external_interrupt.html

*/

#include "stm32l072xx.h"
#include "system_stm32l0xx.h"
#include "utility.h"
#include "sx1276.h"
#include "Board.h"
#include "Radio.h"
#include "RCC.h"
#include <stdio.h>

uint32_t Timeon_LD1_GREEN = 0;
uint32_t Timeon_LD2_GREEN = 0;
uint32_t Timeon_LD3_BLUE = 0;
uint32_t Timeon_LD4_RED = 0;


void Print_Radio ()
{
  uint8_t R;
  char Buffer [100];
  for (int I = 0; I < 128; I = I + 1)
  {
    R = Radio_Read (I);
    sprintf (Buffer, "%x : %x\n", I, R);
    USART_Transmit_CString_Blocking (STLINK_USART, Buffer);
  }
}




int main(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_IOPCEN;

  //At this stage the microcontroller clock setting is already configured,
  //this is done through SystemInit() function which is called from
  //startup file (startup_stm32l0xx.s) before to branch to application main.
  //To reconfigure the default setting of SystemInit() function, refer to system_stm32l0xx.c file 1ms config
  SysTick_Config(2000);
  SystemClock_Config();
  //if (error != 0) {while(1) {}}
  //1ms config
  //SysTick_Config(16000);
  //init_clock_r ();
  //RCC->CFGR &= (uint32_t) RCC_CFGR_MCOSEL;
  //RCC->CFGR |= RCC_CFGR_MCO_SYSCLK | RCC_CFGR_MCO_PRE_4;


  RCC_Enable_LSI_Blocking ();
  Configure_RTC ();
  RTC_Calender_Update (100000000);


  Board_Init ();
  Radio_Init (868100000); //Hz
  

  USART_Transmit_CString_Blocking (STLINK_USART, "Resetting RADIO\r\n");
  GPIO_Pin_Clear (RADIO_RESET_PORT, RADIO_RESET_PIN);
  Delay1 (1000);
  GPIO_Pin_Set (RADIO_RESET_PORT, RADIO_RESET_PIN);
  Delay1 (1000);

  USART_Transmit_CString_Blocking (STLINK_USART, "Loop main\r\n");
  uint32_t Timeout = 0;
  while (1) 
  {

    if(((RTC->ISR & RTC_ISR_RSF) == RTC_ISR_RSF))
    {
      RTC->DR; /* need to read date also */
    }

    if (Timeout == 0)
    {
      GPIO_Pin_Clear (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
      GPIO_Pin_Clear (LED_LD1_GREEN_PORT, LED_LD1_GREEN_PIN);
      GPIO_Pin_Clear (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
      //GPIO_Pin_Clear (LED_LD4_RED_PORT, LED_LD4_RED_PIN);
      Timeout = 2000000;
      
      Radio_Enable_Explicit_Header ();
      Radio_Write (SX1276_RegOPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER_SINGLE);
      
      Radio_Write (SX1276_RegFIFOADDRPTR, 0);
      uint8_t Length = Radio_Read (SX1276_RegPAYLOADLENGTH);
      printf ("Length %i\n", (int)Length);
      int RSSI; 
      RSSI = Radio_RSSI (868E6);
      printf ("RSSI %i\n", (int)RSSI);
      
      
      
    }
    else if (Timeout == 1000000)
    {
      GPIO_Pin_Set (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
      if (Timeon_LD1_GREEN > 0) {GPIO_Pin_Set (LED_LD1_GREEN_PORT, LED_LD1_GREEN_PIN);Timeon_LD1_GREEN--;}
      if (Timeon_LD3_BLUE > 0) {GPIO_Pin_Set (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);Timeon_LD3_BLUE--;}
      if (Timeon_LD4_RED > 0) {GPIO_Pin_Set (LED_LD4_RED_PORT, LED_LD4_RED_PIN);Timeon_LD4_RED--;}
    }
    Timeout--;
    



    
  }
}


void USART2_IRQHandler(void)
{
  int R;
  char Send_Buffer [] = "Hello\n";
  /*
  if((USART2->ISR & USART_ISR_TC) == USART_ISR_TC)
  {
    USART2->ICR = USART_ICR_TCCF; // Clear transfer complete flag
    GPIO_Pin_Toggle (LED_LD4_RED_PORT, LED_LD4_RED_PIN);
  }
  */
  if((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
    //Receive data, clear flag
    uint8_t Data = (uint8_t)(USART2->RDR);  
    switch(Data)
    {
      case 'b':
      case 'B': 
        GPIO_Pin_Toggle (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        break;
      
      case 'r':
      case 'R': 
        Board_Enter_Standby ();
        break;
        
        
      case 't':
      R = Radio_Send ((uint8_t *)Send_Buffer, sizeof (Send_Buffer));
      printf ("Radio_Send: %i\n", R);
      break;
      
      default: 
        break;
    }
  }
}


void Board_Button ()
{
  Timeon_LD1_GREEN++;
  
  if ((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)
  {
    Print_Radio ();
  }
}

