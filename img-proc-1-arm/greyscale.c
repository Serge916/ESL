#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

void greyscale(uint8_t const * const frame_in, uint32_t const xsize_in, uint32_t const ysize_in, uint32_t const bitsperpixel_in, uint8_t * const frame_out)
{
  uint32_t const bytes = bitsperpixel_in/8;
  uint8_t const * current_pixel_in = frame_in;
  uint8_t * current_pixel_out = frame_out;
  uint8_t const * const end_pixel = &frame_in[(xsize_in*ysize_in)*bytes];
  // frame_in & frame_out can be the same
  // it's therefore crucial to have x & y incrementing
  while (current_pixel_in < end_pixel) {
      uint32_t s = 0;
      for (uint32_t b = 0; b < bytes; b++) {
        s += current_pixel_in[b];
      }
      current_pixel_in += bytes;
      *current_pixel_out = s / bytes;
      current_pixel_out++;
    }
  }
