#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void overlay(uint8_t const * const frame_in1, uint32_t const xsize_in1, uint32_t const ysize_in1, uint32_t const bitsperpixel_in1,
             uint8_t const * const frame_in2, uint32_t const xsize_in2, uint32_t const ysize_in2, uint32_t const bitsperpixel_in2,
             uint32_t const xoffset, uint32_t yoffset, double const ratio, uint8_t * const frame_out)
{
  if (xoffset + xsize_in1 < xsize_in2 || yoffset + ysize_in1 < ysize_in2 ||
      bitsperpixel_in1 < bitsperpixel_in2 || ratio < 0 || ratio > 1) {
    fprintf(stderr,"overlay: invalid parameters\n");
    return;
  }
  uint8_t const * current_pixel_in1 = frame_in1;
  uint8_t const * current_pixel_in2 = frame_in2;
  uint8_t * current_pixel_out = frame_out;
  uint8_t const * const end_pixel1 = &frame_in1[xsize_in1*ysize_in1];
  double const ratio1 = ratio;
  double const ratio2 = 1-ratio;
  while (current_pixel_in1 < end_pixel1) {
      *current_pixel_out = *current_pixel_in1 * ratio1 + *current_pixel_in2 * ratio2;
      current_pixel_in1++;
      current_pixel_in2++;
      current_pixel_out++;
    }
  }

