#pragma once

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define  GPIO_MODE_INPUT                        ((uint32_t)0x00000000U)   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP                    ((uint32_t)0x00000001U)   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_AF_PP                        ((uint32_t)0x00000002U)   /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_ANALOG                       ((uint32_t)0x00000003U)   /*!< Analog Mode  */

#define  GPIO_SPEED_FREQ_LOW              ((uint32_t)0x00000000U)  /*!< range up to 0.4 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_MEDIUM           ((uint32_t)0x00000001U)  /*!< range 0.4 MHz to 2 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_HIGH             ((uint32_t)0x00000002U)  /*!< range   2 MHz to 10 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_VERY_HIGH        ((uint32_t)0x00000003U)  /*!< range  10 MHz to 35 MHz, please refer to the product datasheet */

#define  GPIO_NOPULL        ((uint32_t)0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        ((uint32_t)0x00000001U)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      ((uint32_t)0x00000002U)   /*!< Pull-down activation                */

#define VECT_TAB_OFFSET  0x00U

#define HSE_VALUE    ((uint32_t)8000000U) /*!< Value of the External oscillator in Hz */
#define MSI_VALUE    ((uint32_t)2000000U) /*!< Value of the Internal oscillator in Hz*/
#define HSI_VALUE    ((uint32_t)16000000U) /*!< Value of the Internal oscillator in Hz*/


uint32_t GPIO_Mode (uint32_t Mode, uint32_t Pin)
{
  return Mode << (Pin * 2U);
}


uint32_t GPIO_Pull (uint32_t Pull, uint32_t Pin)
{
  return Pull << (Pin * 2U);
}


uint32_t GPIO_Speed (uint32_t Speed, uint32_t Pin)
{
  return Speed << (Pin * 2U);
}

uint32_t GPIO_Pin (uint32_t Pin)
{
  return 1 << (Pin * 1U);
}
