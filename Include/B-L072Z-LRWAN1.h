#pragma once

#include "stm32l072xx.h"
#include "utility.h"


#define LED_LD2_GREEN_PORT GPIOA
#define LED_LD2_GREEN_PIN 5
#define LED_LD1_GREEN_PORT GPIOB
#define LED_LD1_GREEN_PIN 5
#define LED_LD3_BLUE_PORT GPIOB
#define LED_LD3_BLUE_PIN 6
#define LED_LD4_RED_PORT GPIOB
#define LED_LD4_RED_PIN 7


//There is only one button this board.
#define BUTTON_USER_PORT GPIOB
#define BUTTON_USER_PIN 2


//PA13 and PA14 is used for debugging
//GPIO_MODE_AF_PP
#define SWCLK_PIN 13
#define SWCLK_MODE GPIO_MODE_AF_PP
#define SWCLK_PULL GPIO_PULLUP
#define SWCLK_FREQ GPIO_SPEED_FREQ_VERY_HIGH
#define SWDIO_PIN 14
#define SWDIO_MODE GPIO_MODE_AF_PP
#define SWDIO_PULL GPIO_PULLDOWN
#define SWDIO_FREQ GPIO_SPEED_FREQ_LOW


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



#define RADIO_RESET_PORT  GPIOC
#define RADIO_RESET_PIN   0
#define RADIO_RESET_MODE  GPIO_MODE_OUTPUT_PP
#define RADIO_RESET_PULL  GPIO_NOPULL
#define RADIO_RESET_SPEED GPIO_SPEED_FREQ_LOW

#define RADIO_MOSI_PORT   GPIOA
#define RADIO_MOSI_PIN    7
#define RADIO_MOSI_MODE   GPIO_MODE_AF_PP
#define RADIO_MOSI_AF     0
#define RADIO_MOSI_PULL   GPIO_PULLDOWN
#define RADIO_MOSI_SPEED  GPIO_SPEED_FREQ_HIGH

#define RADIO_MISO_PORT   GPIOA
#define RADIO_MISO_PIN    6
#define RADIO_MISO_MODE   GPIO_MODE_AF_PP
#define RADIO_MISO_AF     0
#define RADIO_MISO_PULL   GPIO_PULLDOWN
#define RADIO_MISO_SPEED  GPIO_SPEED_FREQ_HIGH

#define RADIO_SCLK_PORT   GPIOB
#define RADIO_SCLK_PIN    3
#define RADIO_SCLK_MODE   GPIO_MODE_AF_PP
#define RADIO_SCLK_AF     0
#define RADIO_SCLK_PULL   GPIO_PULLDOWN
#define RADIO_SCLK_SPEED  GPIO_SPEED_FREQ_HIGH

#define RADIO_NSS_PORT    GPIOA
#define RADIO_NSS_PIN     15
#define RADIO_NSS_MODE    GPIO_MODE_OUTPUT_PP
#define RADIO_NSS_PULL    GPIO_NOPULL
#define RADIO_NSS_SPEED   GPIO_SPEED_FREQ_HIGH

//https://community.st.com/community/stm32-community/blog/2017/04/05/lora-discovery-kit
/*
CMWX1ZZABZ-091 (MURATA LORA)
 1  PA12/USB_DP
 2  PA11/USB_DM
 3  GND
 4  VDD_USB
 5  VDD_MCU
 6  VDD_RF
 7  GND
 8  PB4/DBG_SX1276_DIO2
 9  PC13/DBG_SX1276_DIO3
10  PA5/DBG_SX1276_DIO4 ?? UNUSED
11  PA4/DBG_SX1276_DIO5 ?? UNUSED
12  PB1/DBG_SX1276_DIO1
13  PB4/DBG_SX1276_DIO0
14  PB15/SPI2_MOSI
15  PB14/SPI2_MISO
16  PB13/SPI2_SCK
17  PB12/SPI2_NSS
18  PA10/USART1_RX
19  PA9/USART1_TX
20  PA8/MCO
21  PA5/ADC5/DAC2
22  PA4/ADC4/DAC1
23  PA3/ADC3/LPUART1_RX
24  PA2/ADC2/LPUART1_TX
25  GND
26  ANT
27  GND
28  PA1/DBG_CRF1 (ANT_SWITCH_RX)
29  PC1/DBG_CRF3 (ANT_SWITCH_TX_BOOST)
30  PC2/DBG_CRF2 (ANT_SWITCH_TX_RFO)
31  STSAFE_NRST
32  VREF+
33  PA0/WKUP1
34  MCU_NRST
35  PB8/I2C1_SCL
36  PB9/I2C1_SDA
37  PB2/LPTIM1_OUT
38  PB7/LPTIM1_IN2
39  PB6/LPTIM1_ETR
40  PB5/LPTIM1_IN1
41  PA13/SWDIO
42  PA14/SWCLK
43  BOOT0
44  GND
45  PH1/OSC_OUT
46  PH0/OSC_IN
47  TCXO_OUT
48  VDD_TCXO
49  GND
..
57  GND

WLCSP49
 1  A1  VDD_USB
 2  A2  PA15  RASIO_NSS
 3  A3  PB3   RADIO_SCK
 4  A4  PB5   LPTIM1_IN1
 5  A5  BOOT0
 6  A6  PB9   I2C1_SDA
 7  A7  VDD
 8  B1  PA12  USB_DP RADIO_TCXO_VCC ??
 9  B2  PA14  SWCLK
10  B3  PB4   RADIO_DIO_0
11  B4  PB6   LPTIM1_ETR
12  B5  PB8   I2C1_SCL
13  B6  VDD
14  B7  PC13  RADIO_DIO_3
15  C1  PA10  USART1_RX
16  C2  PA13  SWDIO
17  C3  PB7   LPTIM1_IN2
18  C4  PC1   RADIO_ANY_SWITCH_TX_BOOST (CRF3)
19  C5  PC0   RADIO_RESET
20  C6  PC14  OSC32_IN
21  C7  PC15  OSC32_OUT
22  D1  PA8   MSO
23  D2  PA11  USB_DM
24  D3  PB1   RADIO_DIO_1
25  D4  VSS
26  D5  NRST
27  D6  PH0   OSC_IN
28  D7  PH1   OSC_OUT
29  E1  PB15  SPI2_MOSI
30  E2  PA9   USART1_TX
31  E3  PB2   LPTIM1_OUT
32  E4  PA1   RADIO_ANT_SWITCH_RX (CRF1)
33  E5  PA0   WKUP1
34  E6  VREF+
35  E7  PC2   RADIO_ANT_SWITCH_TX_RFO (CRF2)
36  F1  PB14  SPI2_MISO
37  F2  PB13  SPI2_SCK
38  F3  PB11  ?? I2C2_SCL
39  F4  PA7   RADIO_MOSI
40  F5  PA4   RADIO_DIO_5
41  F6  PA2   ADC2/LPUART1_TX
42  F7  VDDA
43  G1  PB12  SPI2_NSS
44  G2  VDD
45  G3  PB10  ?? I2C2_SDA
46  G4  PB0   RADIO_DIO_2
47  G5  PA6   RADIO_MISO
48  G6  PA5   RADIO_DIO_4
49  G7  PA3   ADC2/LPUART1_TX
 */

#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000) /* 5 s    */

// Error codes used to make the red led blinking
#define ERROR_USART_TRANSMIT 0x01
#define ERROR_HSI_TIMEOUT 0x02
#define ERROR_PLL_TIMEOUT 0x03
#define ERROR_CLKSWITCH_TIMEOUT 0x04


static __IO uint32_t Tick;
volatile uint16_t error = 0;

void Delay1 (__IO uint32_t Delay)
{
  uint32_t tickstart = 0U;
  tickstart = Tick;
  while((Tick - tickstart) < Delay)
  {
  }
}


__STATIC_INLINE void SystemClock_Config(void)
{
  uint32_t tickstart;
  /* (1) Enable power interface clock */
  /* (2) Select voltage scale 1 (1.65V - 1.95V)
         i.e. (01)  for VOS bits in PWR_CR */
  /* (3) Enable HSI divided by 4 in RCC-> CR */
  /* (4) Wait for HSI ready flag and HSIDIV flag */
  /* (5) Set PLL on HSI, multiply by 8 and divided by 2 */
  /* (6) Enable the PLL in RCC_CR register */
  /* (7) Wait for PLL ready flag */
  /* (8) Select PLL as system clock */
  /* (9) Wait for clock switched on PLL */
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN); /* (1) */
  PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0; /* (2) */

  RCC->CR |= RCC_CR_HSION | RCC_CR_HSIDIVEN; /* (3) */
  tickstart = Tick;
  while ((RCC->CR & (RCC_CR_HSIRDY |RCC_CR_HSIDIVF)) != (RCC_CR_HSIRDY |RCC_CR_HSIDIVF)) /* (4) */
  {
    if ((Tick - tickstart ) > HSI_TIMEOUT_VALUE)
    {
      error = ERROR_HSI_TIMEOUT; /* Report an error */
      return;
    }
  }
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL8 | RCC_CFGR_PLLDIV2; /* (5) */
  RCC->CR |= RCC_CR_PLLON; /* (6) */
  tickstart = Tick;
  while ((RCC->CR & RCC_CR_PLLRDY)  == 0) /* (7) */
  {
    if ((Tick - tickstart ) > PLL_TIMEOUT_VALUE)
    {
      error = ERROR_PLL_TIMEOUT; /* Report an error */
      return;
    }
  }
  RCC->CFGR |= RCC_CFGR_SW_PLL; /* (8) */
  tickstart = Tick;
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL)  == 0) /* (9) */
  {
    if ((Tick - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      error = ERROR_CLKSWITCH_TIMEOUT; /* Report an error */
      return;
    }
  }
}


void Board_Radio_Init ()
{
  GPIO_Pin_Mode (RADIO_RESET_PORT, RADIO_RESET_PIN, RADIO_RESET_MODE);
  GPIO_Pin_Pull (RADIO_RESET_PORT, RADIO_RESET_PIN, RADIO_RESET_PULL);
  GPIO_Pin_Speed (RADIO_RESET_PORT, RADIO_RESET_PIN, RADIO_RESET_SPEED);
  GPIO_Pin_Clear (RADIO_RESET_PORT, RADIO_RESET_PIN);

  GPIO_Pin_Mode (RADIO_MOSI_PORT, RADIO_MOSI_PIN, RADIO_MOSI_MODE);
  GPIO_Pin_Pull (RADIO_MOSI_PORT, RADIO_MOSI_PIN, RADIO_MOSI_PULL);
  GPIO_Pin_Speed (RADIO_MOSI_PORT, RADIO_MOSI_PIN, RADIO_MOSI_SPEED);
  GPIO_Alternate_Function (RADIO_MOSI_PORT, RADIO_MOSI_PIN, RADIO_MOSI_AF);

  GPIO_Pin_Mode (RADIO_MISO_PORT, RADIO_MISO_PIN, RADIO_MISO_MODE);
  GPIO_Pin_Pull (RADIO_MISO_PORT, RADIO_MISO_PIN, RADIO_MISO_PULL);
  GPIO_Pin_Speed (RADIO_MISO_PORT, RADIO_MISO_PIN, RADIO_MISO_SPEED);
  GPIO_Alternate_Function (RADIO_MISO_PORT, RADIO_MISO_PIN, RADIO_MISO_AF);

  GPIO_Pin_Mode (RADIO_SCLK_PORT, RADIO_SCLK_PIN, RADIO_SCLK_MODE);
  GPIO_Pin_Pull (RADIO_SCLK_PORT, RADIO_SCLK_PIN, RADIO_SCLK_PULL);
  GPIO_Pin_Speed (RADIO_SCLK_PORT, RADIO_SCLK_PIN, RADIO_SCLK_SPEED);
  GPIO_Alternate_Function (RADIO_SCLK_PORT, RADIO_SCLK_PIN, RADIO_SCLK_AF);

  GPIO_Pin_Mode (RADIO_NSS_PORT, RADIO_NSS_PIN, RADIO_NSS_MODE);
  GPIO_Pin_Pull (RADIO_NSS_PORT, RADIO_NSS_PIN, RADIO_NSS_PULL);
  GPIO_Pin_Speed (RADIO_NSS_PORT, RADIO_NSS_PIN, RADIO_NSS_SPEED);
  GPIO_Pin_Set (RADIO_NSS_PORT, RADIO_NSS_PIN);
  GPIO_Pin_Clear (RADIO_NSS_PORT, RADIO_NSS_PIN);

  /* Enable the peripheral clock of GPIOA and GPIOB */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;


 /* Enable the peripheral clock SPI1 */
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  /* Configure SPI1 in master */
  /* (1) Master selection, BR: Fpclk/256 (due to C13 on the board, SPI_CLK is set to the minimum)
         CPOL and CPHA at zero (rising first edge), 8-bit data frame */
  /* (2) Slave select output enabled, RXNE IT */
  /* (3) Enable SPI1 */
  SPI1->CR1 =
  SPI_CR1_MSTR |
  SPI_CR1_BR |
  SPI_CR1_SSM |
  0; /* (1) */
  SPI1->CR2 =
  SPI_CR2_SSOE |
  //SPI_CR2_RXNEIE|
  0; /* (2) */
  SPI1->CR1 |= SPI_CR1_SPE; /* (3) */

  /* Configure IT */
  /* (4) Set priority for SPI1_IRQn */
  /* (5) Enable SPI1_IRQn */
  NVIC_SetPriority(SPI1_IRQn, 0); /* (4) */
  NVIC_EnableIRQ(SPI1_IRQn); /* (5) */
}


void Board_Init ()
{
  //At this stage the microcontroller clock setting is already configured,
  //this is done through SystemInit() function which is called from
  // startup file (startup_stm32l0xx.s) before to branch to application main.
  //To reconfigure the default setting of SystemInit() function, refer to system_stm32l0xx.c file
  // 1ms config
  SysTick_Config(2000);
  SystemClock_Config();
  if (error != 0) {while(1) {}}



  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;

  GPIO_Pin_Mode (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN, GPIO_MODE_OUTPUT_PP);
  GPIO_Pin_Pull (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN, GPIO_NOPULL);
  GPIO_Pin_Speed (LED_LD2_GREEN_PORT, LED_LD2_GREEN_PIN, GPIO_SPEED_FREQ_LOW);

  GPIO_Pin_Mode (LED_LD1_GREEN_PORT, LED_LD1_GREEN_PIN, GPIO_MODE_OUTPUT_PP);
  GPIO_Pin_Pull (LED_LD1_GREEN_PORT, LED_LD1_GREEN_PIN, GPIO_NOPULL);
  GPIO_Pin_Speed (LED_LD1_GREEN_PORT, LED_LD1_GREEN_PIN, GPIO_SPEED_FREQ_LOW);

  GPIO_Pin_Mode (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN, GPIO_MODE_OUTPUT_PP);
  GPIO_Pin_Pull (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN, GPIO_NOPULL);
  GPIO_Pin_Speed (LED_LD3_BLUE_PORT, LED_LD3_BLUE_PIN, GPIO_SPEED_FREQ_LOW);

  GPIO_Pin_Mode (LED_LD4_RED_PORT, LED_LD4_RED_PIN, GPIO_MODE_OUTPUT_PP);
  GPIO_Pin_Pull (LED_LD4_RED_PORT, LED_LD4_RED_PIN, GPIO_NOPULL);
  GPIO_Pin_Speed (LED_LD4_RED_PORT, LED_LD4_RED_PIN, GPIO_SPEED_FREQ_LOW);

  GPIO_Pin_Mode (BUTTON_USER_PORT, BUTTON_USER_PIN, GPIO_MODE_INPUT);
  GPIO_Pin_Pull (BUTTON_USER_PORT, BUTTON_USER_PIN, GPIO_NOPULL);
  GPIO_Pin_Speed (BUTTON_USER_PORT, BUTTON_USER_PIN, GPIO_SPEED_FREQ_LOW);

  //Enable the peripheral clock USART2
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

  //USART requires alternate function GPIO mode.
  //See alternate function at DocID027100 Rev 4, Page 45, Table 16, Pin definition.
  GPIO_Pin_Mode (STLINK_USART_TX_PORT, STLINK_USART_TX_PIN, GPIO_MODE_AF_PP);
  GPIO_Pin_Mode (STLINK_USART_RX_PORT, STLINK_USART_RX_PIN, GPIO_MODE_AF_PP);
  GPIO_Alternate_Function (STLINK_USART_TX_PORT, STLINK_USART_TX_PIN, STLINK_USART_TX_AF);
  GPIO_Alternate_Function (STLINK_USART_RX_PORT, STLINK_USART_RX_PIN, STLINK_USART_RX_AF);

  // Oversampling by 16, 9600 baud
  STLINK_USART->BRR = 160000 / 96;

  //USART_CR1_UE : Enable USART
  //USART_CR1_TE : Enable transmitter
  //USART_CR1_RE : Enable receiver
  //USART_CR1_RXNEIE : A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_ISR register
  //USART_CR1_TCIE : A USART interrupt is generated whenever TC=1 in the USART_ISR register
  //[Kenth Johan] Note that when USART receives then ORE=1 or RXNE=1
  STLINK_USART->CR1 =
  USART_CR1_TE |
  USART_CR1_RE |
  USART_CR1_RXNEIE |
  USART_CR1_UE;

  //[ST] polling idle frame Transmission
  //USART_ISR_TC bit is set by hardware if the transmission of a frame containing data is complete and if TXE is set.
  //An interrupt is generated if TCIE=1 in the USART_CR1 register.
  //It is cleared by software, writing 1 to the TCCF in the USART_ICR register or by a write to the USART_TDR register.
  //0: Transmission is not complete
  //1: Transmission is complete
  //Note: If TE bit is reset and no transmission is on going, the TC bit will be set immediately.
  while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC)
  {
    //[ST] add time out here for a robust application
  }

  //Clears the TC flag in the USART_ISR register.
  USART2->ICR = USART_ICR_TCCF;

  //USART_CR1_TCIE : A USART interrupt is generated whenever TC=1 in the USART_ISR register
  //[Kenth Johan] Note that when USART transmitts then TC=1
  //[Kenth Johan] Executing this before ICR_TCCF causes error.
  //STLINK_USART->CR1 |= USART_CR1_TCIE;

  NVIC_SetPriority (USART2_IRQn, 0);
  NVIC_EnableIRQ (USART2_IRQn);

  //System configuration controller (SYSCFG)
  SYSCFG_Source_Input (SYSCFG, BUTTON_USER_PORT, BUTTON_USER_PIN);

  //Extended interrupt and event controller (EXTI)
  //The EXTI allows the management of up to 30 event lines.

  //CONFIRMED: This line is required to trigger transmitt interrupt.
  EXTI_Interrupt_Unmask (EXTI, STLINK_USART_RX_PIN);

  //TODO: Find out why receive triggers a interrupt without unmasking the IMR for TX pin.
  //EXTI_Interrupt_Unmask (EXTI, STLINK_USART_TX_PIN);

  EXTI_Rising_Edge (EXTI, STLINK_USART_RX_PIN);
  EXTI_Rising_Edge (EXTI, STLINK_USART_TX_PIN);

  NVIC_SetPriority (EXTI2_3_IRQn, 0);
  NVIC_EnableIRQ (EXTI2_3_IRQn);

  //1ms config
  SysTick_Config(16000);
}


__attribute__((weak)) void Board_Button ();


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


void SysTick_Handler(void)
{
  Tick = Tick + 1;
}


void NMI_Handler(void){}


void HardFault_Handler(void) {while (1){}}


void SVC_Handler(void){}


void PendSV_Handler(void){}


void EXTI0_1_IRQHandler ()
{
  //__asm__("BKPT");
}
