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


#define APP_MODE_IDLE 0
#define APP_MODE_RECEIVE0 1
#define APP_MODE_RECEIVE1 2
#define APP_MODE_RECEIVE2 3
#define APP_MODE_TRANSMIT 4
int App_Mode = APP_MODE_IDLE;

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

  uint8_t Result = 0;
  int RSSI; 
  uint8_t Status = 0;

  while (1) 
  {

    switch (App_Mode)
    {
      case APP_MODE_IDLE:
        GPIO_Pin_Clear (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
        Delay1 (1000);
        GPIO_Pin_Set (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
        Delay1 (1000);
        break;

      case APP_MODE_RECEIVE0:
        //Radio_Enable_Implicit_Header ();
        Radio_Enable_Explicit_Header ();
        Radio_Write (SX1276_RegOPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER_SINGLE);
        App_Mode = APP_MODE_RECEIVE1;
        break;

      case APP_MODE_RECEIVE1:
        Status = Radio_Read (SX1276_RegMODEMSTAT);
        if (Status != 0x10)
        {
          App_Mode = APP_MODE_RECEIVE2;
        }
        /*
        Result = Radio_Read (SX1276_RegIRQFLAGS);
        if (Result & (RFLR_IRQFLAGS_RXDONE | RFLR_IRQFLAGS_RXTIMEOUT))
        {
          Radio_Write (SX1276_RegIRQFLAGS, 0);
          App_Mode = APP_MODE_RECEIVE2;
        }*/
        break;

      case APP_MODE_RECEIVE2:
        printf ("Status     %x\n", Status);
        printf ("SX1276_RegIRQFLAGS      %x\n", Result);
        printf ("RFLR_IRQFLAGS_RXDONE    %x\n", Result & RFLR_IRQFLAGS_RXDONE);
        printf ("RFLR_IRQFLAGS_RXTIMEOUT %x\n", Result & RFLR_IRQFLAGS_RXTIMEOUT);
        //Radio_Write (SX1276_RegFIFOADDRPTR, 0);
        uint8_t Length = Radio_Read (SX1276_RegPAYLOADLENGTH);
        printf ("Length %i\n", (int)Length);
        RSSI = Radio_RSSI (868E6);
        printf ("RSSI %i\n", (int)RSSI);
        App_Mode = APP_MODE_IDLE;
        break;
    }
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
        GPIO_Pin_Toggle (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        break;
      
      case 's': 
        Board_Enter_Standby ();
        break;

      case 'r': 
        App_Mode = APP_MODE_RECEIVE0;
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
  if ((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)
  {
    Print_Radio ();
  }
}

