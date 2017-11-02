/*
https://community.st.com/community/stm32-community/blog/2017/04/05/lora-discovery-kit

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

#pragma once

#include "utility.h"
#include "SX1276.h"
#include "GPIO.h"
#include "SPI.h"

#define RADIO_NAME "SX1276"

#define RADIO_SPI SPI1

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
#define RADIO_NSS_MODE    GPIO_MODE_AF_PP
#define RADIO_NSS_PULL    GPIO_PULLUP
#define RADIO_NSS_SPEED   GPIO_SPEED_FREQ_HIGH

/*
#define RADIO_ANT_TX_BOOST_PORT GPIOC
#define RADIO_ANT_TX_BOOST_PIN  1

#define RADIO_ANT_TX_RFO_PORT GPIOC
#define RADIO_ANT_TX_RFO_PIN  2

#define RADIO_ANT_RX_PORT GPIOA
#define RADIO_ANT_RX_PIN  1
*/

#define RADIO_RCC_IOPENR (RCC_IOPENR_IOPAEN | RCC_IOPENR_IOPBEN | RCC_IOPENR_IOPCEN)






enum { EU868_F1 = 868100000,      // g1   SF7-12
       EU868_F2 = 868300000,      // g1   SF7-12 FSK SF7/250
       EU868_F3 = 868500000,      // g1   SF7-12
       EU868_F4 = 868850000,      // g2   SF7-12
       EU868_F5 = 869050000,      // g2   SF7-12
       EU868_F6 = 869525000,      // g3   SF7-12
       EU868_J4 = 864100000,      // g2   SF7-12  used during join
       EU868_J5 = 864300000,      // g2   SF7-12   ditto
       EU868_J6 = 864500000,      // g2   SF7-12   ditto
};











//Read or write to address location.
uint8_t Radio_Transfer8 (uint8_t Address, uint8_t Value)
{
  uint8_t Response;
  //Set NSS to 0 by enabling the SPI.
  RADIO_SPI->CR1 |= SPI_CR1_SPE;
  SPI_Transfer8_Blocking (RADIO_SPI, Address);
  Response = SPI_Transfer8_Blocking (RADIO_SPI, Value);
  RADIO_SPI->CR1 &= ~SPI_CR1_SPE;
  return Response;
}


//Read value from address location.
//Returns read value.
uint8_t Radio_Read (uint8_t Address)
{
  uint8_t Response;
  Response = Radio_Transfer8 (Address & 0x7f, 0x00);
  return Response;
}


//Write value to address location.
void Radio_Write (uint8_t Address, uint8_t Value)
{
  Radio_Transfer8 (Address | 0x80, Value);
}


void Radio_Sleep ()
{
  uint8_t Values = 0;

  //This bit can be modified only in Sleep mode.
  //A write operation on other device modes is ignored.
  //LoRaTM Mode
  Values |= RFLR_OPMODE_LONGRANGEMODE_ON;
  Values |= RFLR_OPMODE_SLEEP;

  Radio_Write (SX1276_RegOPMODE, Values);
}


void Radio_Idle ()
{
  uint8_t Values = 0;

  //This bit can be modified only in Sleep mode.
  //A write operation on other device modes is ignored.
  //LoRaTM Mode
  Values |= RFLR_OPMODE_LONGRANGEMODE_ON;
  Values |= RFLR_OPMODE_STANDBY;

  Radio_Write (SX1276_RegOPMODE, Values);
}


void Radio_Set_Carrier_Frequency (uint32_t Frequency)
{
  //Resolution is 61.035 Hz if F(XOSC) = 32 MHz. Default value is
  //0x6c8000 = 434 MHz. Register values must be modified only
  //when device is in SLEEP or STAND-BY mode.
  uint64_t frf = ((uint64_t)Frequency << 19) / 32000000;
  Radio_Write (SX1276_RegFRFMSB, (uint8_t)(frf >> 16));
  Radio_Write (SX1276_RegFRFMID, (uint8_t)(frf >> 8));
  Radio_Write (SX1276_RegFRFLSB, (uint8_t)(frf >> 0));
}


void Radio_Init (uint32_t Frequency)
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
  //TODO: Why this causes USART2 RX not being able to receive?
  //TODO: Why is SPI still working without this?
  //GPIO_Alternate_Function (RADIO_SCLK_PORT, RADIO_SCLK_PIN, RADIO_SCLK_AF);

  GPIO_Pin_Mode (RADIO_NSS_PORT, RADIO_NSS_PIN, RADIO_NSS_MODE);
  GPIO_Pin_Pull (RADIO_NSS_PORT, RADIO_NSS_PIN, RADIO_NSS_PULL);
  GPIO_Pin_Speed (RADIO_NSS_PORT, RADIO_NSS_PIN, RADIO_NSS_SPEED);

  // Enable the peripheral clock of GPIOA and GPIOB
  RCC->IOPENR |= RADIO_RCC_IOPENR;

  SPI_Init (RADIO_SPI);


  //TODO: Why is this required when IRQ is not used?
  NVIC_SetPriority(SPI1_IRQn, 0);
  NVIC_EnableIRQ(SPI1_IRQn);

  Radio_Sleep ();

  Radio_Set_Carrier_Frequency (Frequency);

  //write base address in FIFO data buffer for TX modulator
  Radio_Write (SX1276_RegFIFOTXBASEADDR, 0);
  //read base address in FIFO data buffer for RX demodulator
  Radio_Write (SX1276_RegFIFORXBASEADDR, 0);



  uint8_t Value;

  // set LNA boost
  Value = 0;
  Value |= RFLR_LNA_GAIN_G1;
  Value |= RFLR_LNA_BOOST_HF_ON;
  Radio_Write (SX1276_RegLNA, Value);

  // set auto AGC
  Radio_Write (SX1276_RegMODEMCONFIG3, 0x04);


  //set output power
  Value = 0;
  Value |= RFLR_PACONFIG_PASELECT_PABOOST;
  Radio_Write (SX1276_RegPACONFIG, Value);

  Radio_Idle ();
}


void Radio_Header_Implicit ()
{
  uint8_t Value;
  Value = Radio_Read (SX1276_RegMODEMCONFIG1);
  Value |= RFLR_MODEMCONFIG1_IMPLICITHEADER_ON;
  Radio_Write (SX1276_RegMODEMCONFIG1, Value);
}


void Radio_Send (uint8_t * Data, uint8_t Count)
{
  Radio_Write (SX1276_RegFIFOADDRPTR, 0);
  Radio_Write (SX1276_RegPAYLOADLENGTH, 0);
  //int Length;
  //Length = Radio_Read (SX1276_RegPAYLOADLENGTH);
  for (int I = 0; I < Count; I = I + 1)
  {
    Radio_Write (SX1276_RegFIFO, Data [I]);
  }
  uint8_t Value = 0;
  Value |= RFLR_OPMODE_LONGRANGEMODE_ON;
  Value |= RFLR_OPMODE_TRANSMITTER;
  Radio_Write (SX1276_RegOPMODE, Value);

  // wait for TX done
  while ((Radio_Read (SX1276_RegIRQFLAGS) & RFLR_IRQFLAGS_TXDONE) == 0);

  // clear IRQ's
  Radio_Write (SX1276_RegIRQFLAGS, RFLR_IRQFLAGS_TXDONE_MASK);
}


uint8_t Radio_Receive (uint8_t * Data, uint8_t Count)
{
  uint8_t Length;
  Length = Radio_Read (SX1276_RegRXNBBYTES);
  for (uint8_t I = 0; I < Length; I = I + 1)
  {
    Data [I] = Radio_Read (SX1276_RegFIFO);
  }
  return Length;
}


uint8_t Radio_Parse_Packet (uint8_t Size)
{
  uint8_t Length = 0;
  uint8_t irqFlags = Radio_Read (SX1276_RegIRQFLAGS);

  Radio_Header_Implicit ();

  Radio_Write (SX1276_RegPAYLOADLENGTH, Size);

  Radio_Write (SX1276_RegIRQFLAGS, irqFlags);


  if ((irqFlags & RFLR_IRQFLAGS_RXDONE_MASK) && (irqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK) == 0)
  {
    Length = Radio_Read (SX1276_RegPAYLOADLENGTH);

    // set FIFO address to current RX address
    Radio_Write (SX1276_RegFIFOADDRPTR, Radio_Read (SX1276_RegFIFORXCURRENTADDR));

    // put in standby mode
    Radio_Idle ();
  }
  else if (Radio_Read (SX1276_RegOPMODE) != (RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER_SINGLE))
  {
    // not currently in RX mode
    // reset FIFO address
    Radio_Write (SX1276_RegFIFOADDRPTR, 0);
    // put in single RX mode
    Radio_Write (SX1276_RegOPMODE, RFLR_OPMODE_LONGRANGEMODE_ON | RFLR_OPMODE_RECEIVER_SINGLE);
  }

  return Length;
}


int32_t Radio_RSSI (uint32_t Frequency)
{
  int32_t Value;
  Value = Radio_Read (SX1276_RegPKTRSSIVALUE);
  Value = Value - Frequency < 868E6 ? 164 : 157;
  return Value;
}
