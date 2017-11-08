/*
https://feabhas.gitbooks.io/stm32f4-cmsis/content/gpio_as_an_external_interrupt.html

*/

#include <stdio.h>
#include "stm32l072xx.h"
#include "system_stm32l0xx.h"
#include "utility.h"
#include "sx1276.h"
#include "Board.h"
#include "Radio.h"
#include "RCC.h"
#include "LRWAN.h"




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
#define APP_MODE_LISTEN0 1
#define APP_MODE_LISTEN1 2
#define APP_MODE_LISTEN2 3
#define APP_MODE_INFO 10
#define APP_MODE_RECEIVE3 20
#define APP_MODE_TRANSMIT0 30
#define APP_MODE_TRANSMIT1 31
#define APP_MODE_JOIN0 40


int App_Mode = APP_MODE_IDLE;
int X = 0;

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
      
      case APP_MODE_LISTEN0:
        //Radio_Enable_Implicit_Header ();
        Radio_Enable_Explicit_Header ();
        Radio_Write (SX1276_RegOPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RXCONTINUOUS);
        App_Mode = APP_MODE_IDLE;
        break;
      
      case APP_MODE_LISTEN1:
      {
        uint8_t Flag;
        char Buffer [255];
        Flag = Radio_Read (SX1276_RegIRQFLAGS);
        if (Flag & RFLR_IRQFLAGS_RXDONE)
        {
          Radio_Receive ((uint8_t *)Buffer, 255);
          printf ("Buffer : %s\n", Buffer);
          printf ("RSSI   : %i\n", (int)Radio_RSSI (868100000));
          Radio_Write (SX1276_RegIRQFLAGS, 0xFF);
        }
        break;
      }
      
      case APP_MODE_LISTEN2:
      {
        uint8_t Flag;
        uint8_t Buffer [255];
        uint8_t Length;
        Flag = Radio_Read (SX1276_RegIRQFLAGS);
        if (Flag & RFLR_IRQFLAGS_RXDONE)
        {
          Length = Radio_Receive ((uint8_t *)Buffer, 255);
          printf ("Buffer:");
          for (uint8_t I = 0; I < Length; I = I + 1)
          {
            if ((I % 8) == 0) {printf ("\n");}
            printf ("%02x ", Buffer [I]);
          }
          printf ("\nRSSI: %i\n", (int)Radio_RSSI (868100000));
          Radio_Write (SX1276_RegIRQFLAGS, 0xFF);
        }
        App_Mode = APP_MODE_IDLE;
        break;
      }

      
      case APP_MODE_INFO:
      {
        uint8_t MODEMSTAT;
        uint8_t IRQFLAGS;
        uint8_t RXNBBYTES;
        uint8_t FIFORXBASEADDR;
        uint8_t FIFORXCURRENTADDR;
        char Buffer [256];
        MODEMSTAT = Radio_Read (SX1276_RegMODEMSTAT);
        IRQFLAGS = Radio_Read (SX1276_RegIRQFLAGS);
        RXNBBYTES = Radio_Read (SX1276_RegRXNBBYTES);
        FIFORXBASEADDR = Radio_Read (SX1276_RegFIFORXBASEADDR);
        FIFORXCURRENTADDR = Radio_Read (SX1276_RegFIFORXCURRENTADDR);
        Radio_Receive ((uint8_t *)Buffer, 255);
        printf ("SX1276_RegMODEMSTAT      %x\n", MODEMSTAT);
        printf ("SX1276_RegIRQFLAGS       %x\n", IRQFLAGS);
        printf ("SX1276_RegRXNBBYTES      %x\n", RXNBBYTES);
        printf ("SX1276_RegFIFORXBASEADDR %x\n", FIFORXBASEADDR);
        printf ("RegFIFORXCURRENTADDR     %x\n", FIFORXCURRENTADDR);
        printf ("Buffer                   %s\n", Buffer);
        Radio_Write (SX1276_RegIRQFLAGS, 0xFF);
        App_Mode = APP_MODE_IDLE;
        break;
      }
      
      case APP_MODE_TRANSMIT0:
      {
        App_Mode = APP_MODE_TRANSMIT1;
        break;
      }
      
      case APP_MODE_TRANSMIT1:
      {
        Radio_Write (SX1276_RegDIOMAPPING1, RFLR_DIOMAPPING1_DIO0_01);
        int R;
        char Send_Buffer [20];
        sprintf (Send_Buffer, "World %i", X++);
        R = Radio_Send ((uint8_t *)Send_Buffer, sizeof (Send_Buffer));
        printf ("Radio_Send: %i\n", R);
        App_Mode = APP_MODE_IDLE;
        break;
      }
      
      case APP_MODE_JOIN0:
      {
        int R;
        struct LRWAN_Frame_Join_Request Data = 
        {
          .MDHR     = LWAN_MDHR_MTYPE_JOIN_REQUEST | LWAN_MDHR_MAJOR_R1,
          .DevEUI   = LORAWAN_MAC, 
          .AppEUI   = LORAWAN_APPLICATION_EUI
        };
        //Data.DevNonce [0] = Radio_Read (SX1276_RegPKTRSSIVALUE) % 255;
        //Data.DevNonce [1] = Radio_Read (SX1276_RegPKTSNRVALUE) % 255;
        Data.DevNonce [0] = 0;
        Data.DevNonce [1] = 0;
        LRWAN_Join (&Data, (uint8_t [])LORAWAN_APPLICATION_KEY);
        R = Radio_Send ((uint8_t *) &Data, sizeof (struct LRWAN_Frame_Join_Request));
        printf ("Radio_Send: %i\n", R);
        App_Mode = APP_MODE_IDLE;
        break;
      }
      
      
    }
  }
}



void USART2_IRQHandler(void)
{
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
        
      case 'i':
        App_Mode = APP_MODE_INFO;
        break;
        
      case 't':
        if (App_Mode == APP_MODE_IDLE)
        {
          App_Mode = APP_MODE_TRANSMIT0;
        }
        break;
      
      case 'l':
        App_Mode = APP_MODE_LISTEN0;
        break;
      
      case 'j':
        if (App_Mode == APP_MODE_IDLE)
        {
          App_Mode = APP_MODE_JOIN0;
        }
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


void EXTI0_1_IRQHandler ()
{
  if((EXTI->PR & (1 << RADIO_DIO1_PIN)) == (1 << RADIO_DIO1_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << RADIO_DIO1_PIN);
    GPIO_Pin_Toggle (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
    printf ("RADIO_DIO1_PIN\n");
  }
  if((EXTI->PR & (1 << RADIO_DIO2_PIN)) == (1 << RADIO_DIO2_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << RADIO_DIO2_PIN);
    GPIO_Pin_Toggle (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
    printf ("RADIO_DIO2_PIN\n");
  }
}


void EXTI2_3_IRQHandler(void)
{
  //Page 289.
  //13.5.6 EXTI pending register (EXTI_PR)
  //This bit is set when the selected edge event arrives on the interrupt line.
  if((EXTI->PR & (1 << BUTTON_USER_PIN)) == (1 << BUTTON_USER_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << BUTTON_USER_PIN);
    Board_Button ();
  }
}


void EXTI4_15_IRQHandler ()
{
  if((EXTI->PR & (1 << RADIO_DIO0_PIN)) == (1 << RADIO_DIO0_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << RADIO_DIO0_PIN);
    App_Mode = APP_MODE_LISTEN2;
    printf ("RADIO_DIO0_PIN\n");
  }
  if((EXTI->PR & (1 << RADIO_DIO3_PIN)) == (1 << RADIO_DIO3_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << RADIO_DIO3_PIN);
    printf ("RADIO_DIO3_PIN\n");
  }
  //printf ("EXTI4_15!\n");
}