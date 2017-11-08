#pragma once

#include <stdint.h>
#include "stm32l072xx.h"


#define Bitfield_Modify(Result, Mask, Value) ((Result) = ((Result) & ~(Mask)) | ((Value) & (Mask)))
