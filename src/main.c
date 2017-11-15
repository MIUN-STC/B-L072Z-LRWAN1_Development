/*
https://feabhas.gitbooks.io/stm32f4-cmsis/content/gpio_as_an_external_interrupt.html

*/

#include <string.h>
#include "stm32l072xx.h"
#include "system_stm32l0xx.h"
#include "utility.h"
#include "sx1276.h"
#include "Board.h"
#include "Radio.h"
#include "RCC.h"
#include "LRWAN.h"
#include "USART.h"
#include "App_printf.h"



void Print_Radio ()
{
  uint8_t R;
  for (int I = 0; I < 128; I = I + 1)
  {
    R = Radio_Read (I);
    App_printf ("%02x : %02x\n", I, R);
  }
}

void Print_Radio_FError ()
{
  uint8_t R;
  for (int I = SX1276_RegFEIMSB; I <= SX1276_RegFEILSB; I = I + 1)
  {
    R = Radio_Read (I);
    App_printf ("%02x : %02x\n", I, R);
  }
}



void App_Print_LRWAN_Message (union LRWAN_Message * Message)
{
  switch (Message->MDHR & LWAN_MHDR_MTYPE)
  {
    case LWAN_MDHR_MTYPE_JOIN_REQUEST:
    {
    App_printf ("AppEUI:   ");
    for (uint8_t I = 0; I < 8; I = I + 1) {App_printf ("%02x ", Message->Request.AppEUI [I]);}
    App_printf ("\n");
    App_printf ("DevEUI:   ");
    for (uint8_t I = 0; I < 8; I = I + 1) {App_printf ("%02x ", Message->Request.DevEUI [I]);}
    App_printf ("\n");
    App_printf ("DevNonce: ");
    for (uint8_t I = 0; I < 2; I = I + 1) {App_printf ("%02x ", Message->Request.DevNonce [I]);}
    App_printf ("\n");
    break;
    }
    case LWAN_MDHR_MTYPE_JOIN_ACCEPT:
    {
    App_printf ("AppNonce: ");
    for (uint8_t I = 0; I < 3; I = I + 1) {App_printf ("%02x ", Message->Accept.AppNonce [I]);}
    App_printf ("\n");
    App_printf ("NetID: ");
    for (uint8_t I = 0; I < 3; I = I + 1) {App_printf ("%02x ", Message->Accept.NetID [I]);}
    App_printf ("\n");
    App_printf ("DevAddr: ");
    for (uint8_t I = 0; I < 4; I = I + 1) {App_printf ("%02x ", Message->Accept.DevAddr [I]);}
    App_printf ("\n");
    App_printf ("DLSettings: %x\n", Message->Accept.DLSettings);
    App_printf ("RXDelay:    %x\n", Message->Accept.RXDelay);
    break;
    }
    case LWAN_MDHR_MTYPE_UNCONFIRMED_DATA_UP:
    case LWAN_MDHR_MTYPE_UNCONFIRMED_DATA_DOWN:
    case LWAN_MDHR_MTYPE_CONFIRMED_DATA_UP:
    case LWAN_MDHR_MTYPE_CONFIRMED_DATA_DOWN:
    App_printf ("DevAddr: ");
    for (uint8_t I = 0; I < 4; I = I + 1) {App_printf ("%02x ", Message->Data.DevAddr [I]);}
    App_printf ("\n");
    App_printf ("FHDR_ADR:       %d\n", (Message->Data.FCtrl & LRWAN_FHDR_ADR) == LRWAN_FHDR_ADR);
    App_printf ("FHDR_ADRACKREQ: %d\n", (Message->Data.FCtrl & LRWAN_FHDR_ADRACKREQ) == LRWAN_FHDR_ADRACKREQ);
    App_printf ("FHDR_ACK:       %d\n", (Message->Data.FCtrl & LRWAN_FHDR_ACK) == LRWAN_FHDR_ACK);
    App_printf ("FHDR_RFU:       %d\n", (Message->Data.FCtrl & LRWAN_FHDR_RFU) == LRWAN_FHDR_RFU);
    App_printf ("FHDR_FPENDING:  %d\n", (Message->Data.FCtrl & LRWAN_FHDR_FPENDING) == LRWAN_FHDR_FPENDING);
    App_printf ("FHDR_FOPTSLEN:  %d\n", (Message->Data.FCtrl & LRWAN_FHDR_FOPTSLEN));
    App_printf ("FCnt: ");
    for (uint8_t I = 0; I < 2; I = I + 1) {App_printf ("%02x ", Message->Data.FCnt [I]);}
    App_printf ("\n");
    break;
    case LWAN_MDHR_MTYPE_RFU:
    case LWAN_MDHR_MTYPE_PROPRIETARY:
    break;
    break;
  }
}



#define APP_MODE_IDLE                 0
#define APP_MODE_INFO                 10
#define APP_MODE_LISTEN0              1000
#define APP_MODE_READ                 1001
#define APP_MODE_LISTEN               1002
#define APP_MODE_JOIN_TRANSMIT        2000
#define APP_MODE_JOIN_TRANSMITTING    2001
#define APP_MODE_TRANSMIT             3000
#define APP_MODE_TRANSMITTING         3001

int App_Mode = APP_MODE_IDLE;
int X = 0;
int App_TX_Counter = 0;

int main(void)
{
  NVIC_SetPriority (EXTI0_1_IRQn, 0);
  NVIC_SetPriority (EXTI2_3_IRQn, 0);
  NVIC_SetPriority (EXTI4_15_IRQn, 0);
  NVIC_SetPriority (SPI1_IRQn, 0);
  NVIC_SetPriority (USART2_IRQn, 0);

  NVIC_EnableIRQ (EXTI0_1_IRQn);
  NVIC_EnableIRQ (EXTI2_3_IRQn);
  NVIC_EnableIRQ (EXTI4_15_IRQn);
  //TODO: Why is this required when IRQ is not used?
  NVIC_EnableIRQ (SPI1_IRQn);
  NVIC_EnableIRQ (USART2_IRQn);

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
  Radio_Init ();

  App_printf ("Resetting RADIO\r\n");
  GPIO_Pin_Clear (RADIO_RESET_PORT, RADIO_RESET_PIN);
  Delay1 (10000);
  GPIO_Pin_Set (RADIO_RESET_PORT, RADIO_RESET_PIN);
  //Save power. Reset pin is pulled up in SX1276.
  GPIO_Pin_Mode (RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_MODE_INPUT);
  Delay1 (10000);

  SPI_Init (RADIO_SPI);
  Radio_Sleep ();
  //Radio_Set_Carrier_Frequency (800300000);
  //Radio_Set_Carrier_Frequency (868300000);
  //Radio_Set_Carrier_Frequency (868300000);
  Radio_Set_Carrier_Frequency (869900000);
  Radio_Write (SX1276_RegFIFOADDRPTR, 0x00);
  Radio_Write (SX1276_RegFIFOTXBASEADDR, 0);
  Radio_Write (SX1276_RegFIFORXBASEADDR, 0);
  Radio_Write (SX1276_RegSYNCWORD, 0x34);
  Radio_Write (SX1276_RegLNA, RFLR_LNA_GAIN_G4 | RFLR_LNA_BOOST_HF_ON);
  //Radio_Write (SX1276_RegMODEMCONFIG3, RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON);
  Radio_Write (SX1276_RegMODEMCONFIG3, RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON | RFLR_MODEMCONFIG3_AGCAUTO_ON);
  Radio_Write (SX1276_RegINVERTIQ, 0x67); //IQ inverted. Node to GW
  //Radio_Write (SX1276_RegINVERTIQ, 0x27); //IQ non inverted. Node to Node
  //Radio_Write (SX1276_RegMODEMCONFIG3, RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON | RFLR_MODEMCONFIG3_AGCAUTO_OFF);
  
  
  
  App_printf ("Loop %s.\r\n", "main");
  
  uint8_t Buf [20];
  memset (Buf, 0x22, 20);
        
  while (1)
  {

    switch (App_Mode)
    {
      case APP_MODE_JOIN_TRANSMITTING:
      case APP_MODE_TRANSMITTING:
      case APP_MODE_LISTEN:
      case APP_MODE_IDLE:
        GPIO_Pin_Clear (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
        Delay1 (1000);
        GPIO_Pin_Set (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN);
        Delay1 (1000);
        break;

      case APP_MODE_LISTEN0:
      {
        App_Mode = APP_MODE_LISTEN;
        GPIO_Pin_Clear (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        Radio_Write (SX1276_RegMODEMCONFIG2, RFLR_MODEMCONFIG2_SF_12 | RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF | RFLR_MODEMCONFIG2_RXPAYLOADCRC_OFF);
        Radio_Write (SX1276_RegMODEMCONFIG1, RFLR_MODEMCONFIG1_BW_125_KHZ | RFLR_MODEMCONFIG1_CODINGRATE_4_5 | RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF);
        Radio_Write (SX1276_RegOPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RXCONTINUOUS);
        Radio_Write (SX1276_RegSYMBTIMEOUTLSB, 0x18);
        //Radio_Set_Carrier_Frequency (868300000 + 51000);
        //Radio_Set_Carrier_Frequency (869100000);
        Radio_Write (SX1276_RegPpmCorrection, 0x00);
        //Radio_Write (SX1276_RegOPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER_SINGLE);
        //Print_Radio ();
        Print_Radio_FError ();
        break;
      }

      case APP_MODE_READ:
      {
        App_Mode = APP_MODE_LISTEN0;
        GPIO_Pin_Set (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        union LRWAN_Message Message [1];
        uint8_t Payload [256];
        uint8_t Length;
        Length = Radio_Receive ((uint8_t *)Payload, 255);
        App_printf ("\nRX EVENT\n");
        App_printf ("Length:    %d\n", (int)Length);
        App_printf ("CRC On:    %d\n", (Radio_Read (SX1276_RegHOPCHANNEL) & RFLR_HOPCHANNEL_CRCONPAYLOAD_ON) == RFLR_HOPCHANNEL_CRCONPAYLOAD_ON);
        App_printf ("CRC Error: %d\n", (Radio_Read (SX1276_RegIRQFLAGS) & RFLR_IRQFLAGS_PAYLOADCRCERROR) == RFLR_IRQFLAGS_PAYLOADCRCERROR);
        App_printf ("RSSI:      %d\n", (int)Radio_RSSI (868300000));
        if (Length < 8) {Radio_Write (SX1276_RegIRQFLAGS, 0xFF);break;};
        LRWAN_Decypt (Payload, Length, (uint8_t [])LORAWAN_APPLICATION_KEY, Message);
        App_printf ("MTYPE: %s\n", LRWAN_MTYPE_Get_String (Message->MDHR & LWAN_MHDR_MTYPE));
        App_printf ("MIC:   ");
        for (uint8_t I = Length - 4 - 1; I < Length; I = I + 1) {App_printf ("%02x ", Message->Buffer [I]);}
        App_printf ("\n");
        App_printf ("MIC:   ");
        for (uint8_t I = 0; I < 4; I = I + 1) {App_printf ("%02x ", Message->MIC [I]);}
        App_printf ("\n");
        App_printf ("Payload:");
        for (uint8_t I = 0; I < (int)(Length - 1); I = I + 1)
        {
          if ((I % 8) == 0) {App_printf ("\n");}
          App_printf ("%02x ", Message->Buffer [I]);
        }
        App_printf ("\n");
        App_Print_LRWAN_Message (Message);
        Radio_Write (SX1276_RegIRQFLAGS, 0xFF);
        break;
      }

      case APP_MODE_TRANSMIT:
      {
        App_Mode = APP_MODE_TRANSMITTING;
        GPIO_Pin_Set (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        Radio_Write (SX1276_RegPACONFIG, 0xFF);
        Radio_Write (SX1276_RegDIOMAPPING1, RFLR_DIOMAPPING1_DIO0_01);
        Radio_Write (SX1276_RegMODEMCONFIG1, RFLR_MODEMCONFIG1_BW_125_KHZ | RFLR_MODEMCONFIG1_CODINGRATE_4_5 | RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF);
        Radio_Write (SX1276_RegMODEMCONFIG2, RFLR_MODEMCONFIG2_SF_12 | RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF | RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON);

        Radio_Send (Buf, 20);
        break;
      }

      case APP_MODE_JOIN_TRANSMIT:
      {
        App_Mode = APP_MODE_TRANSMITTING;
        App_TX_Counter++;
        Radio_Write (SX1276_RegPACONFIG, 0xFF);
        Radio_Write (SX1276_RegDIOMAPPING1, RFLR_DIOMAPPING1_DIO0_01);
        Radio_Write (SX1276_RegMODEMCONFIG1, RFLR_MODEMCONFIG1_BW_125_KHZ | RFLR_MODEMCONFIG1_CODINGRATE_4_5 | RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF);
        Radio_Write (SX1276_RegMODEMCONFIG2, RFLR_MODEMCONFIG2_SF_12 | RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF | RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON);
        struct LRWAN_Frame_Join_Request Data =
        {
          .MDHR     = LWAN_MDHR_MTYPE_JOIN_REQUEST | LWAN_MDHR_MAJOR_R1,
          .DevEUI   = LORAWAN_MAC,
          .AppEUI   = LORAWAN_APPLICATION_EUI
        };
        Data.DevNonce [0] = Radio_Read (SX1276_RegRSSIWIDEBAND) + App_TX_Counter;
        Data.DevNonce [1] = App_TX_Counter;
        LRWAN_Join (&Data, (uint8_t [])LORAWAN_APPLICATION_KEY);
        Radio_Send ((uint8_t *) &Data, 23);
        GPIO_Pin_Set (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
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
        
      case 't':
        if (App_Mode == APP_MODE_IDLE) {App_Mode = APP_MODE_TRANSMIT;}
        break;

      case 'l':
        if (App_Mode == APP_MODE_IDLE) {App_Mode = APP_MODE_LISTEN0;}
        break;

      case 'j':
        if (App_Mode == APP_MODE_IDLE) {App_Mode = APP_MODE_JOIN_TRANSMIT;}
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
    App_printf ("RADIO_DIO1_PIN\n");
  }
  if((EXTI->PR & (1 << RADIO_DIO2_PIN)) == (1 << RADIO_DIO2_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << RADIO_DIO2_PIN);
    GPIO_Pin_Toggle (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
    App_printf ("RADIO_DIO2_PIN\n");
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
    //Clear all flags.
    Radio_Write (SX1276_RegIRQFLAGS, 0xFF);
    switch (App_Mode)
    {
      case APP_MODE_LISTEN:
        App_Mode = APP_MODE_READ;
        break;

      case APP_MODE_LISTEN0:
        break;

      case APP_MODE_READ:
        break;

      case APP_MODE_TRANSMIT:
        GPIO_Pin_Clear (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        App_Mode = APP_MODE_IDLE;
        break;

      case APP_MODE_TRANSMITTING:
        GPIO_Pin_Clear (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN);
        App_Mode = APP_MODE_IDLE;
        break;

      default:
        App_Mode = APP_MODE_IDLE;
        break;
    }
  }
  if((EXTI->PR & (1 << RADIO_DIO3_PIN)) == (1 << RADIO_DIO3_PIN))
  {
    //This bit is cleared by writing it to 1 or by changing the sensitivity of the edge detector.
    EXTI->PR = (1 << RADIO_DIO3_PIN);
    App_printf ("RADIO_DIO3_PIN\n");
  }
  //printf ("EXTI4_15!\n");
}
