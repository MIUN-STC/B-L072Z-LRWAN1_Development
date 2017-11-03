#pragma once

#include "stm32l072xx.h"
#include "utility.h"
#include "SPI.h"
#include "PWR.h"
#include "USART.h"
#include "EXTI.h"
#include "SYSCFG.h"
#include "GPIO.h"
#include "Clock.h"
#include "RTC.h"
#include <stdio.h>

#define BOARD_NAME "B-L072Z-LRWAN1"


#define LED_LD2_GREEN_PORT GPIOA
#define LED_LD2_GREEN_PIN 5
#define LED_LD1_GREEN_PORT GPIOB
#define LED_LD1_GREEN_PIN 5
#define LED_LD3_BLUE_PORT GPIOB
#define LED_LD3_BLUE_PIN 6
#define LED_LD4_RED_PORT GPIOB
#define LED_LD4_RED_PIN 7


//There is only one button on this board.
#define BUTTON_USER_PORT GPIOB
#define BUTTON_USER_PIN 2


//PA13 and PA14 is used for debugging
//GPIO_MODE_AF_PP
#define SWCLK_PORT GPIOA
#define SWCLK_PIN 13
#define SWCLK_MODE GPIO_MODE_AF_PP
#define SWCLK_PULL GPIO_PULLUP
#define SWCLK_FREQ GPIO_SPEED_FREQ_VERY_HIGH
#define SWDIO_PORT GPIOA
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

void init_clock_r()
{
  Bitfield_Modify(PWR->CR, PWR_CR_VOS, PWR_CR_VOS_0); // setting power regulator to 1.2V

  // Oscillator Setting
  RCC->CR |= RCC_CR_MSION;// MSI oscillator ON
  while ((RCC->CR & RCC_CR_MSIRDY) == 0); // Waiting MSI oscillator prepared(f:broken change)
  Bitfield_Modify (RCC->ICSCR,RCC_ICSCR_MSIRANGE, RCC_ICSCR_MSIRANGE_4); // Change MSI oscillator frequency to range (4) 1.048MHz

  // Clock Source Setting
  Bitfield_Modify (RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_MSI);     // Setting SYSCLK Oscillator to MSI
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI); // Waiting SYSCLK Clock Source Change

  Bitfield_Modify (RCC->CFGR, RCC_CFGR_HPRE,RCC_CFGR_HPRE_DIV1);  // Setting HCLK(AHB) clock source to SYSCLK/1
  Bitfield_Modify (RCC->CFGR, RCC_CFGR_PPRE1,RCC_CFGR_PPRE1_DIV1);// Setting PCLK1(APB1) clock source to SYSCLK/1
  Bitfield_Modify (RCC->CFGR, RCC_CFGR_PPRE2,RCC_CFGR_PPRE2_DIV1);// Setting PCLK2(APB2) clock source to SYSCLK/1

  // Change Core System Tick
  SysTick_Config(1048); // System Tick overflow Value  1.048Mhz /1000 interrupt 1mS
  uint32_t SYSTICK_SOURCE_HCLK = (uint32_t)0x00000004U; //System Tick clock source
  SysTick->CTRL |= SYSTICK_SOURCE_HCLK; // Change System Tick clock source
  NVIC_SetPriority(SysTick_IRQn,0); // Change System Tick Interrupt Priority to Highest
}


__STATIC_INLINE void SystemClock_Config(void)
{
  uint32_t tickstart;

  //Enable power interface clock
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

  PWR_Regulator_Voltage (PWR_REGULATOR_150mV);
  //Clock_MSI_Frequency (MSI_FREQUENCY_4194000Hz);
  //Select voltage scale 1 (1.65V - 1.95V)
  //i.e. (01)  for VOS bits in PWR_CR */
  //PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0;

  //Enable HSI divided by 4 in RCC-> CR
  RCC->CR |= RCC_CR_HSION | RCC_CR_HSIDIVEN;


  tickstart = Tick;

  //Wait for HSI ready flag and HSIDIV flag
  while ((RCC->CR & (RCC_CR_HSIRDY |RCC_CR_HSIDIVF)) != (RCC_CR_HSIRDY |RCC_CR_HSIDIVF))
  {
    if ((Tick - tickstart ) > HSI_TIMEOUT_VALUE)
    {
      error = ERROR_HSI_TIMEOUT;
      return;
    }
  }

  //Set PLL on HSI, multiply by 8 and divided by 2
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSI | RCC_CFGR_PLLMUL8 | RCC_CFGR_PLLDIV2;

  //Enable the PLL in RCC_CR register
  RCC->CR |= RCC_CR_PLLON;
  tickstart = Tick;

  //Wait for PLL ready flag
  while ((RCC->CR & RCC_CR_PLLRDY)  == 0)
  {
    if ((Tick - tickstart ) > PLL_TIMEOUT_VALUE)
    {
      error = ERROR_PLL_TIMEOUT;
      return;
    }
  }

  //Select PLL as system clock
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  tickstart = Tick;

  //Wait for clock switched on PLL
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL)  == 0)
  {
    if ((Tick - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      error = ERROR_CLKSWITCH_TIMEOUT;
      return;
    }
  }

}


void Board_Init ()
{
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
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;
  //RCC->CFGR &= (uint32_t) RCC_CFGR_MCOSEL;
  //RCC->CFGR |= RCC_CFGR_MCO_SYSCLK | RCC_CFGR_MCO_PRE_4;
  Bitfield_Modify (RCC->CFGR, RCC_CFGR_MCOSEL_Msk, RCC_CFGR_MCOSEL_MSI);
  Bitfield_Modify (RCC->CFGR, RCC_CFGR_MCOPRE_Msk, RCC_CFGR_MCOPRE_DIV1);
  GPIO_Pin_Mode (GPIOB, 13, GPIO_MODE_AF_PP);
  GPIO_Pin_Pull (GPIOB, 13, GPIO_PULLUP);
  GPIO_Pin_Speed (GPIOB, 13, GPIO_SPEED_FREQ_HIGH);
  GPIO_Alternate_Function (GPIOB, 13, 1);


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
  STLINK_USART->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;

  //[ST] polling idle frame Transmission
  //USART_ISR_TC bit is set by hardware if the transmission of a frame containing data is complete and if TXE is set.
  //An interrupt is generated if TCIE=1 in the USART_CR1 register.
  //It is cleared by software, writing 1 to the TCCF in the USART_ICR register or by a write to the USART_TDR register.
  //0: Transmission is not complete
  //1: Transmission is complete
  //Note: If TE bit is reset and no transmission is on going, the TC bit will be set immediately.
  //while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC)
  //{
    //[ST] add time out here for a robust application
  //}

  //Clears the TC flag in the USART_ISR register.
  //USART2->ICR = USART_ICR_TCCF;

  //USART_CR1_TCIE : A USART interrupt is generated whenever TC=1 in the USART_ISR register
  //[Kenth Johan] Note that when USART transmitts then TC=1
  //[Kenth Johan] Executing this before ICR_TCCF causes error.
  //STLINK_USART->CR1 |= USART_CR1_TCIE;

  NVIC_SetPriority (USART2_IRQn, 0);
  NVIC_EnableIRQ (USART2_IRQn);



  //Extended interrupt and event controller (EXTI)
  //The EXTI allows the management of up to 30 event lines.

  //CONFIRMED: This line is required to trigger transmitt interrupt.
  //EXTI_Interrupt_Unmask (EXTI, STLINK_USART_RX_PIN);

  //TODO: Find out why receive triggers a interrupt without unmasking the IMR for TX pin.
  //EXTI_Interrupt_Unmask (EXTI, STLINK_USART_TX_PIN);

  //This affects the button
  //EXTI_Rising_Edge (EXTI, STLINK_USART_RX_PIN);


  //EXTI_Rising_Edge (EXTI, STLINK_USART_TX_PIN);


  //System configuration controller (SYSCFG)
  SYSCFG_Source_Input (SYSCFG, BUTTON_USER_PORT, BUTTON_USER_PIN);
  EXTI_Interrupt_Unmask (EXTI, BUTTON_USER_PIN);
  EXTI_Rising_Edge (EXTI, BUTTON_USER_PIN);
  NVIC_SetPriority (EXTI2_3_IRQn, 0);
  NVIC_EnableIRQ (EXTI2_3_IRQn);
}


//Weak functions can be overloaded.
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


void SPI1_IRQHandler(void)
{
  if((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
  {
    uint8_t Data = (uint8_t)SPI1->DR;
    (void) Data;
  }
  else
  {
    //error = ERROR_SPI;
    NVIC_DisableIRQ(SPI1_IRQn);
  }
}


void RTC_IRQHandler(void)
{
  /* Check WUT flag */
  if((RTC->ISR & (RTC_ISR_WUTF)) == (RTC_ISR_WUTF))
  {
    RTC->ISR =~ RTC_ISR_WUTF; /* Reset Wake up flag */
    EXTI->PR = EXTI_PR_PR20; /* clear exti line 20 flag */
    //GPIOB->ODR ^= (1 << 4) ; /* Toggle Green LED */
    GPIO_Pin_Toggle (LED_LD4_RED_PORT, LED_LD4_RED_PIN);
    printf ("RTC_IRQHandler\n");
  }
}


void Board_Enter_Standby ()
{
  //Enable Clocks
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
  //Set PWR_CSR_EWUP1 bit which represent wake up pin PA0 and will be pulled down.
  //The device will wake up in Reset_Handler when PA0 goes HIGH.
  //[Kenth Johan] The STM32L072CZ can only wake up from PWR_CSR_EWUP1.
  //Do not enable PWR_CSR_EWUP1 | PWR_CSR_EWUP2 | PWR_CSR_EWUP3 bits, it will not work.
  PWR->CSR |= PWR_CSR_EWUP1;
  
  //Clear the WUF flag after 2 clock cycles
  PWR->CR |= PWR_CR_CWUF;
  
  //V_{REFINT} is off in low-power mode
  PWR->CR |= PWR_CR_ULP;   
  
  //Enter Standby mode when the CPU enters deepsleep
  PWR->CR |= PWR_CR_PDDS;
  
  //Low-power mode = stop mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  
  //Reenter low-power mode after ISR
  SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
  
  __WFI();
}
