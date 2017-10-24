#include <stdint.h>
//#include "stm32l0xx.h"
#include "stm32l072xx.h"
#include "core_cm0plus.h"
#include "main_utility.h"


void SystemInit (void)
{    
  // Set MSION bit
  RCC->CR |= (uint32_t)0x00000100U;
  // Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits
  RCC->CFGR &= (uint32_t) 0x88FF400CU;
  // Reset HSION, HSIDIVEN, HSEON, CSSON and PLLON bits
  RCC->CR &= (uint32_t)0xFEF6FFF6U;
  // Reset HSI48ON  bit
  RCC->CRRCR &= (uint32_t)0xFFFFFFFEU;
  // Reset HSEBYP bit
  RCC->CR &= (uint32_t)0xFFFBFFFFU;
  // Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits
  RCC->CFGR &= (uint32_t)0xFF02FFFFU;
  // Disable all interrupts
  RCC->CIER = 0x00000000U;
  // Configure the Vector Table location add offset address
  //SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM
  //Vector Table Relocation in Internal FLASH
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
}


void NMI_Handler(void)
{}


//brief This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
  while (1) {}
}


//This function handles System service call via SWI instruction.
void SVC_Handler(void)
{}


//brief This function handles Pendable request for system service.
void PendSV_Handler(void)
{}



uint32_t uwTick1;

__attribute__((weak)) uint32_t GetTick(void)
{
  return uwTick1;
}

void SysTick_Handler(void)
{
  uwTick1 = uwTick1 + 1;
}

void Delay1 (__IO uint32_t Delay)
{
  uint32_t tickstart = 0U;
  tickstart = GetTick ();
  while((GetTick () - tickstart) < Delay)
  {
  }
}


// Reset of all peripherals, Initializes the Flash interface and the Systick.
void App_Init ()
{
  //Enable the FLASH preread buffer.
  SET_BIT((FLASH->ACR), FLASH_ACR_PRE_READ);
  //Use systick as time base source and configure 1ms tick (default clock after Reset is MSI)
  //Configure the SysTick to have interrupt in 1ms time basis
  uint32_t SystemCoreClock = 2000000U;
  SysTick_Config (SystemCoreClock/1000U);
  
  SET_BIT(RCC->APB2ENR, (RCC_APB2ENR_SYSCFGEN));
  SET_BIT(RCC->APB2ENR, (RCC_APB2ENR_DBGMCUEN));
  
  //Configure the SysTick IRQ priority
  NVIC_SetPriority(SVC_IRQn, 0);
  /* PendSV_IRQn interrupt configuration */
  NVIC_SetPriority(PendSV_IRQn, 0);
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, 0);
  
  //GPIO Ports Clock Enable
  SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
  SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN);
}


static void App_Init_GPIO (void)
{
  
  //GPIO Port A settings
  GPIOA->MODER = 
  GPIO_Mode (GPIO_MODE_ANALOG, 0) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 1) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 2) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 3) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 4) | 
  GPIO_Mode (GPIO_MODE_OUTPUT_PP, 5) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 6) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 7) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 8) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 9) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 10) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 11) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 12) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 13) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 14) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 15);

  GPIOA->PUPDR = 
  GPIO_Pull (GPIO_NOPULL, 0) | 
  GPIO_Pull (GPIO_NOPULL, 1) | 
  GPIO_Pull (GPIO_NOPULL, 2) | 
  GPIO_Pull (GPIO_NOPULL, 3) | 
  GPIO_Pull (GPIO_NOPULL, 4) | 
  GPIO_Pull (GPIO_NOPULL, 5) | 
  GPIO_Pull (GPIO_NOPULL, 6) | 
  GPIO_Pull (GPIO_NOPULL, 7) | 
  GPIO_Pull (GPIO_NOPULL, 8) | 
  GPIO_Pull (GPIO_NOPULL, 9) | 
  GPIO_Pull (GPIO_NOPULL, 10) | 
  GPIO_Pull (GPIO_NOPULL, 11) | 
  GPIO_Pull (GPIO_NOPULL, 12) | 
  GPIO_Pull (GPIO_NOPULL, 13) | 
  GPIO_Pull (GPIO_NOPULL, 14) | 
  GPIO_Pull (GPIO_NOPULL, 15);
  
  GPIOA->OSPEEDR = 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 0) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 1) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 2) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 3) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 4) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 5) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 6) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 7) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 8) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 9) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 10) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 11) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 12) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 13) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 14) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 15);
  
  //GPIO Port B settings
  GPIOB->MODER = 
  GPIO_Mode (GPIO_MODE_ANALOG, 0) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 1) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 2) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 3) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 4) | 
  GPIO_Mode (GPIO_MODE_OUTPUT_PP, 5) | 
  GPIO_Mode (GPIO_MODE_OUTPUT_PP, 6) | 
  GPIO_Mode (GPIO_MODE_OUTPUT_PP, 7) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 8) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 9) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 10) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 11) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 12) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 13) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 14) | 
  GPIO_Mode (GPIO_MODE_ANALOG, 15);

  GPIOB->PUPDR = 
  GPIO_Pull (GPIO_NOPULL, 0) | 
  GPIO_Pull (GPIO_NOPULL, 1) | 
  GPIO_Pull (GPIO_NOPULL, 2) | 
  GPIO_Pull (GPIO_NOPULL, 3) | 
  GPIO_Pull (GPIO_NOPULL, 4) | 
  GPIO_Pull (GPIO_NOPULL, 5) | 
  GPIO_Pull (GPIO_NOPULL, 6) | 
  GPIO_Pull (GPIO_NOPULL, 7) | 
  GPIO_Pull (GPIO_NOPULL, 8) | 
  GPIO_Pull (GPIO_NOPULL, 9) | 
  GPIO_Pull (GPIO_NOPULL, 10) | 
  GPIO_Pull (GPIO_NOPULL, 11) | 
  GPIO_Pull (GPIO_NOPULL, 12) | 
  GPIO_Pull (GPIO_NOPULL, 13) | 
  GPIO_Pull (GPIO_NOPULL, 14) | 
  GPIO_Pull (GPIO_NOPULL, 15);
  
  GPIOB->OSPEEDR = 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 0) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 1) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 2) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 3) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 4) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 5) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 6) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 7) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 8) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 9) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 10) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 11) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 12) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 13) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 14) | 
  GPIO_Speed (GPIO_SPEED_FREQ_LOW, 15);
}


int main(void)
{
  App_Init ();
  App_Init_GPIO ();
  while (1)
  {
    GPIOA->BSRR = GPIO_Pin (5);
    GPIOB->BSRR = GPIO_Pin (5)|GPIO_Pin (6)|GPIO_Pin (7);
    //HAL_Delay (1000);
    Delay1 (1000);
    GPIOA->BRR = GPIO_Pin (5);
    GPIOB->BRR = GPIO_Pin (5)|GPIO_Pin (6)|GPIO_Pin (7);
    //HAL_Delay (1000);
    Delay1 (1000);
  }
}

