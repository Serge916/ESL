#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void offset(uint8_t const volatile *const original_address, uint8_t volatile *const final_address, uint32_t const total_pixels, uint32_t const bitsperpixel_in)
{
  uint32_t const bytes = bitsperpixel_in / 8;
  for (uint32_t i = 0; i < total_pixels; i++)
  {
    final_address[i] = original_address[i];
  }
}
