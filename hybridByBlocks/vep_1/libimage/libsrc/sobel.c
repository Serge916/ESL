#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <xil_printf.h>
#include "../include/image.h"

void sobel(uint8_t const volatile *const frame_in, uint32_t const xsize_in, uint32_t const ysize_in, uint32_t const block_ysize, uint32_t const bytes_per_pixel,
           uint8_t const threshold, uint8_t volatile *const frame_out, uint32_t const frame_half, uint32_t const block_flag)
{
  const int fxsize = 3, fysize = 3;
  int32_t const sobelx[] = {
      -1,
      0,
      1,
      -2,
      0,
      2,
      -1,
      0,
      1,
  };
  int32_t const sobely[] = {
      1, 2, 1,
      0, 0, 0,
      -1, -2, -1};

  uint32_t y_offset = (frame_half == 0) ? 0 : ysize_in - block_ysize;
  for (uint32_t y = 0; y < block_ysize; y++)
  {
    for (uint32_t x = 0; x < xsize_in; x++)
    {
      for (uint32_t b = 0; b < bytes_per_pixel; b++)
      {
        // don't create edge at borders
        if (x < (fxsize + 1) / 2 || x >= xsize_in - (fxsize + 1) / 2 || (y + y_offset) < (fysize + 1) / 2 || (y + y_offset) >= ysize_in - (fysize + 1) / 2)
        {
          frame_out[y * xsize_in + x] = 0;
        }
        else
        {
          int32_t xr = 0, yr = 0;
          for (int32_t ty = 0; ty < fysize; ty++)
          {
            for (int32_t tx = 0; tx < fxsize; tx++)
            {
              xr += sobelx[ty * fxsize + tx] * frame_in[(y + ty - fysize / 2) * xsize_in + x + tx - fxsize / 2];
              yr += sobely[ty * fxsize + tx] * frame_in[(y + ty - fysize / 2) * xsize_in + x + tx - fxsize / 2];
            }
          }
          // gradient magnitude
          double const r = yr * yr + xr * xr;
          // clip/saturate from int32_t to uint8_t
          if (r < threshold * threshold)
            frame_out[y * xsize_in + x] = 0;
          else
            frame_out[y * xsize_in + x] = UINT8_MAX;
        }
      }
    }
  }
}
