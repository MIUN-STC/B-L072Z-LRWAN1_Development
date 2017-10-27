#include "stm32l072xx.h"
#include "system_stm32l0xx.h"
#include "main_utility.h"
#include "B-L072Z-LRWAN1.h"

#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000) /* 5 s    */

/* Delay value : short one is used for the error coding, long one (~1s) in case 
   of no error or between two bursts */
#define SHORT_DELAY 200
#define LONG_DELAY 2000

/* Error codes used to make the red led blinking */
#define ERROR_USART_TRANSMIT 0x01
#define ERROR_HSI_TIMEOUT 0x02
#define ERROR_PLL_TIMEOUT 0x03
#define ERROR_CLKSWITCH_TIMEOUT 0x04




static __IO uint32_t Tick;
volatile uint16_t error = 0;

uint8_t send = 0;
const uint8_t stringtosend[] = "ST\n";


void SystemClock_Config(void);
void Configure_GPIO_LED(void);
void Configure_EXTI(void);


void Delay1 (__IO uint32_t Delay)
{
  uint32_t tickstart = 0U;
  tickstart = Tick;
  while((Tick - tickstart) < Delay)
  {
  }
}

static void App_Init_GPIO (void)
{
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

}


void App_Init_USART ()
{

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
  while((USART2->ISR & USART_ISR_TC) != USART_ISR_TC)
  { 
    //[ST] add time out here for a robust application
  }
  
  
  //Clears the TC flag in the USART_ISR register.
  USART2->ICR = USART_ICR_TCCF;
  
  //USART_CR1_TCIE : A USART interrupt is generated whenever TC=1 in the USART_ISR register
  //[Kenth Johan] Note that when USART transmitts then TC=1
  //[Kenth Johan] Executing this before ICR_TCCF causes error.
  STLINK_USART->CR1 |= USART_CR1_TCIE;
  
  NVIC_SetPriority (USART2_IRQn, 0);
  NVIC_EnableIRQ (USART2_IRQn);
}


__INLINE static void App_Init_SPI (void)
{
  /* Enable the peripheral clock of GPIOA and GPIOB */
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	
  /* (1) Select AF mode (10) on PA4, PA6, PA7 */
  /* (2) AF0 for SPI1 signals */
  /* (3) Select AF mode (10) on PB3 */
  /* (4) AF0 for SPI1 signals */
  GPIOA->MODER = (GPIOA->MODER 
		  & ~(GPIO_MODER_MODE4 | \
		      GPIO_MODER_MODE6 | GPIO_MODER_MODE7))\
  | (GPIO_MODER_MODE4_1 | \
     GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1); /* (1) */
  GPIOA->AFR[0] = (GPIOA->AFR[0] & \
		   ~((0xF<<(4*4)) | \
		     (0xF<<(4*6)) | ((uint32_t)0xF<<(4*7)))); /* (2) */
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE3)) | GPIO_MODER_MODE3_1; /* (3) */
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~((0xF<<(4*3)))); /* (4) */
  
 /* Enable the peripheral clock SPI1 */
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  /* Configure SPI1 in master */
  /* (1) Master selection, BR: Fpclk/256 (due to C13 on the board, SPI_CLK is set to the minimum)
         CPOL and CPHA at zero (rising first edge), 8-bit data frame */
  /* (2) Slave select output enabled, RXNE IT */
  /* (3) Enable SPI1 */
  SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR; /* (1) */
  SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE; /* (2) */
  SPI1->CR1 |= SPI_CR1_SPE; /* (3) */
 
  /* Configure IT */
  /* (4) Set priority for SPI1_IRQn */
  /* (5) Enable SPI1_IRQn */
  NVIC_SetPriority(SPI1_IRQn, 0); /* (4) */
  NVIC_EnableIRQ(SPI1_IRQn); /* (5) */ 
}



int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l0xx.c file
   */
  
  // 1ms config
  SysTick_Config(2000);
  SystemClock_Config();
  
  App_Init_GPIO ();
  
  if (error != 0) {while(1) {}}
  
  //1ms config
  SysTick_Config(16000);
  
  App_Init_USART ();
  Configure_EXTI();

  for (int I = 0; I < 5; I = I + 1)
  {
    GPIO_Pin_Set (GPIOB, LED_LD1_GREEN_PIN);
    GPIO_Pin_Set (GPIOB, LED_LD3_BLUE_PIN);
    GPIO_Pin_Set (GPIOB, LED_LD4_RED_PIN);
    Delay1 (100);
    GPIO_Pin_Clear (GPIOB, LED_LD1_GREEN_PIN);
    GPIO_Pin_Clear (GPIOB, LED_LD3_BLUE_PIN);
    GPIO_Pin_Clear (GPIOB, LED_LD4_RED_PIN);
    Delay1 (100);
    USART2->TDR = 'a';
  }
  
  while (1) {}
}


__INLINE void SystemClock_Config(void)
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


__INLINE void Configure_EXTI(void)
{
  
  //System configuration controller (SYSCFG)
  SYSCFG_Source_Input (SYSCFG, GPIOB, 2);
  
  //Extended interrupt and event controller (EXTI)
  //The EXTI allows the management of up to 30 event lines.
  
  //CONFIRMED: This line is required to trigger transmitt interrupt.
  EXTI_Interrupt_Unmask (EXTI, STLINK_USART_RX_PIN);
  
  //TODO: Find out why receive triggers a interrupt without unmasking of IMR for TX pin.
  //EXTI_Interrupt_Unmask (EXTI, STLINK_USART_TX_PIN);
  
  EXTI_Rising_Edge (EXTI, STLINK_USART_RX_PIN);
  EXTI_Rising_Edge (EXTI, STLINK_USART_TX_PIN);
  
  NVIC_SetPriority(EXTI2_3_IRQn, 0);
  NVIC_EnableIRQ(EXTI2_3_IRQn);
}


void NMI_Handler(void){}


void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void){}


void PendSV_Handler(void){}


void SysTick_Handler(void)
{
  static uint32_t long_counter = LONG_DELAY;
  static uint32_t short_counter = SHORT_DELAY;  
  static uint16_t error_temp = 0;
  
  Tick++;
  if (long_counter-- == 0) 
  {
    if(error == 0)
    {
      // the following instruction can only be used if no ISR modifies GPIOC ODR
      // either by writing directly it or by using GPIOC BSRR or BRR 
      // else a toggle mechanism must be implemented using GPIOC BSRR and/or BRR
      //GPIOB->ODR ^= (1 << 4);//toggle green led on PB4
      //GPIOA->ODR ^= GPIO_Pin (LED_LD2_GREEN_PIN);
      GPIO_Pin_Toggle (GPIOA, LED_LD2_GREEN_PIN);
      long_counter = LONG_DELAY;
    }
    else if (error != 0xFF)
    {
      // red led blinks according to the code error value
      error_temp = (error << 1) - 1;
      short_counter = SHORT_DELAY;
      long_counter = LONG_DELAY << 1;
      
      GPIO_Pin_Set (GPIOB, LED_LD4_RED_PIN);
      GPIO_Pin_Clear (GPIOB, LED_LD1_GREEN_PIN);
    }
  }
  if (error_temp > 0)
  {
    if (short_counter-- == 0) 
    {
      GPIO_Pin_Toggle (GPIOB, LED_LD4_RED_PIN);
      short_counter = SHORT_DELAY;
      error_temp--;
    }  
  }
  
}


void EXTI0_1_IRQHandler ()
{
  //__asm__("BKPT");
}


void EXTI2_3_IRQHandler(void)
{
  if((EXTI->PR & EXTI_PR_PR2) == EXTI_PR_PR2)
  {
    /* Clear EXTI 0 flag */
    EXTI->PR = EXTI_PR_PR2;	
	
    /* start USART transmission */
    USART2->TDR = stringtosend[send++]; /* Will inititiate TC if TXE */
  }
}


void USART2_IRQHandler(void)
{
  GPIO_Pin_Toggle (GPIOB, LED_LD4_RED_PIN);
  uint8_t chartoreceive = 0;
  if((USART2->ISR & USART_ISR_TC) == USART_ISR_TC)
  {
    if(send == sizeof(stringtosend))
    {
      send=0;
      USART2->ICR = USART_ICR_TCCF; /* Clear transfer complete flag */
      GPIO_Pin_Toggle (GPIOB, LED_LD1_GREEN_PIN);
    }
    else
    {
      /* clear transfer complete flag and fill TDR with a new char */
      USART2->TDR = stringtosend[send++];
    }
  }
  else if((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
    chartoreceive = (uint8_t)(USART2->RDR); /* Receive data, clear flag */
          
    switch(chartoreceive)
    {
      case 'b':
      case 'B': 
	GPIO_Pin_Toggle (GPIOB, LED_LD3_BLUE_PIN);
	break;
      default: break;
    }
  }
  else
  {
    error = ERROR_USART_TRANSMIT; /* Report an error */
    NVIC_DisableIRQ(USART2_IRQn); /* Disable USART2_IRQn */
  }
	
}


void SPI1_IRQHandler(void)
{
  if((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
  {
    //SPI1_ByteReceived = 1;
    //SPI1_Data = (uint8_t)SPI1->DR; /* receive data, clear flag */
  }
  else
  {
    //error = ERROR_SPI;
    NVIC_DisableIRQ(SPI1_IRQn); /* Disable SPI1_IRQn */
  }
}
