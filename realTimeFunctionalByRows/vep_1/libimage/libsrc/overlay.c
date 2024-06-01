#include <stdio.h>
#include <string.h>
#include <stdint.h>

void overlay(uint8_t const volatile *const line_in_1, uint8_t const volatile *const line_in_2, uint32_t volatile length, double const ratio, uint8_t volatile *const line_out)
{
  double const ratio_complementary = 1 - ratio;
  for (uint32_t i = 0; i < length; i++)
  {
    line_out[i] = ratio * line_in_1[i] + ratio_complementary * line_in_2[i];
  }
}