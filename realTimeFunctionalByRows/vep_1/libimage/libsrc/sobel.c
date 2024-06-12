#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <xil_printf.h>
#include "../include/image.h"

void sobel(uint8_t const volatile *const lines_in, uint32_t const length, uint32_t const y_position, uint32_t const y_size,
           uint8_t const threshold, uint8_t volatile *const line_out)
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

  for (uint32_t x = 0; x < length; x++)
  {
    // don't create edge at borders
    if (x < (fxsize + 1) / 2 || x >= length - (fxsize + 1) / 2 || (y_position) < (fysize + 1) / 2 || (y_position) >= y_size - (fysize + 1) / 2)
    {
      line_out[x] = 0;
    }
    else
    {
      int32_t xr = 0, yr = 0;
      for (int32_t ty = 0; ty < fysize; ty++)
      {
        for (int32_t tx = 0; tx < fxsize; tx++)
        {
          xr += sobelx[ty * fxsize + tx] * lines_in[ty * fysize + x + tx - fxsize / 2];
          yr += sobely[ty * fxsize + tx] * lines_in[ty * fysize + x + tx - fxsize / 2];
        }
      }
      // gradient magnitude
      double const r = yr * yr + xr * xr;
      // clip/saturate from int32_t to uint8_t
      if (r < threshold * threshold)
        line_out[x] = 0;
      else
        line_out[x] = UINT8_MAX;
    }
  }
}
