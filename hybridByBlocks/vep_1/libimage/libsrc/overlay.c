#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <xil_printf.h>

#define RATIO 1
#define RATIO_COMP 1 - RATIO

void overlay(uint8_t const volatile *const frame_in1, uint8_t const volatile *const frame_in2, uint32_t volatile pixels_in_block, uint8_t volatile *const frame_out)
{

  for (uint32_t i = 0; i < pixels_in_block; i++)
  {
    frame_out[i] = RATIO * frame_in1[i] + RATIO_COMP * frame_in2[i];
  }
}