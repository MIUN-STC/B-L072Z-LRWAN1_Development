#pragma once

#include <stdint.h>
#include "mini_fprint.h"
#include "USART.h"

/*
#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

int _write(int file, char *data, int len)
{
  USART_Transmit8v_Blocking (USART2, (uint8_t *) data, (uint32_t) len);
  // return # of bytes written - as best we can tell
  return len;
}
*/

int App_printf (const char * Format, ...)
{
  char Buffer [100];
  va_list ap;
  int retval;
  va_start(ap, Format);
  retval = mini_vsnprintf (Buffer, sizeof (Buffer), Format, ap);
  va_end(ap);
  USART_Transmit8v_Blocking (USART2, (uint8_t *) Buffer, (uint32_t) retval);
  return retval;
}