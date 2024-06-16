#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <xil_printf.h>
#include "../include/image.h"

void sobel(uint8_t const volatile *const frame_in, uint32_t const xsize_in, uint32_t const ysize_in, uint32_t const lines_in_frame,
           uint8_t const threshold, uint8_t volatile *const frame_out, uint32_t const frame_half, uint32_t const initial_y, uint32_t const lines_in_block)
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

  uint32_t y_offset = (frame_half == 0) ? 0 : lines_in_block - lines_in_frame;
  uint32_t y_frame = y_offset;
  y_offset += initial_y;
  for (uint32_t y = 0; y < lines_in_frame; y++)
  {
    for (uint32_t x = 0; x < xsize_in; x++)
    {
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
            xr += sobelx[ty * fxsize + tx] * frame_in[((y + y_frame + ty - 1) * xsize_in + x + tx - 1)]; // filter_size/2=1
            yr += sobely[ty * fxsize + tx] * frame_in[((y + y_frame + ty - 1) * xsize_in + x + tx - 1)]; // filter_size/2=1
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
