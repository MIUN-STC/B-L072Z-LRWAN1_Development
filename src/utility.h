#pragma once

#include <stdint.h>
#include "stm32l072xx.h"
/*
void* _sbrk() { return 0; }
void* _write_r() { return 0; }
void* _close_r() { return 0; }
void* _lseek_r() { return 0; }
void* _read_r() { return 0; }
void* _fstat_r() { return 0; }
void* _isatty_r() { return 0; }
*/
#define Bitfield_Modify(Result, Mask, Value) ((Result) = ((Result) & ~(Mask)) | ((Value) & (Mask)))
