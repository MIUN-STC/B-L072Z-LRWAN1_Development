#pragma once

#include "stm32l072xx.h"
#include "utility.h"


/*
Introduction
The extended interrupts and events controller (EXTI) manages the external and internal
asynchronous events/interrupts and generates the event request to the CPU/interrupt
controller plus a wake-up request to the power controller.
The EXTI allows the management of up to 30 event lines which can wake up the device
from Stop mode.
Some of the lines are configurable: in this case the active edge can be chosen
independently, and a status flag indicates the source of the interrupt. The configurable lines
are used by the I/Os external interrupts, and by few peripherals. Some of the lines are
direct: they are used by some peripherals to generate a wakeup from Stop event or
interrupt. In this case the status flag is provided by the peripheral.
Each line can be masked independently for interrupt or event generation.
Te EXTI controller also allows to emulate, by programming to a dedicated register, events or
interrupts by software multiplexed with the corresponding hardware event line.

*/



void EXTI_Interrupt_Unmask (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->IMR |= 1 << Pin;
}


void EXTI_Rising_Edge (EXTI_TypeDef * EXTIx, uint32_t Pin)
{
  EXTIx->RTSR |= 1 << Pin;
}