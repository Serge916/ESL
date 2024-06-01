#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

double const conv_sharpen3[] = {
    -1,
    -1,
    -1,
    -1,
    9,
    -1,
    -1,
    -1,
    -1,
};
double const conv_avgxy1[] = {1};
double const conv_avgx3[] = {
    0.33,
    0.33,
    0.33,
};
double const conv_avgxy3[] = {
    0.11,
    0.11,
    0.11,
    0.11,
    0.11,
    0.11,
    0.11,
    0.11,
    0.11,
};
double const conv_avgxy5[] = {
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
};
double const conv_avgxy7[] = {
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
    0.02,
};
double const conv_gaussianblur5[] = {
    1 / 256.0,
    4 / 256.0,
    6 / 256.0,
    4 / 256.0,
    1 / 256.0,
    4 / 256.0,
    16 / 256.0,
    24 / 256.0,
    16 / 256.0,
    4 / 256.0,
    6 / 256.0,
    24 / 256.0,
    36 / 256.0,
    24 / 256.0,
    6 / 256.0,
    4 / 256.0,
    16 / 256.0,
    24 / 256.0,
    16 / 256.0,
    4 / 256.0,
    1 / 256.0,
    4 / 256.0,
    6 / 256.0,
    4 / 256.0,
    1 / 256.0,
};

void convolution(uint8_t const volatile *const *const lines_in, uint32_t const volatile length, uint32_t const volatile y_position, uint32_t const volatile y_size,
                 double const *const f, uint32_t const fxsize, uint32_t const fysize, uint8_t volatile *const line_out)
{
  uint32_t fx_half_size = fxsize / 2;
  uint32_t fy_half_size = fysize / 2;
  for (uint32_t x = 0; x < length; x++)
  {
    double r = 0;
    for (uint32_t ty = 0; ty < fysize; ty++)
    {
      for (uint32_t tx = 0; tx < fxsize; tx++)
      {
        if (x + tx >= fx_half_size && x + tx - fx_half_size < length && y_position + ty >= fy_half_size &&
            y_position + ty - fy_half_size < y_size)
        {
          r += f[ty * fxsize + tx] * lines_in[ty][tx + x - fx_half_size];
        }
        else
        {
          r += f[ty * fxsize + tx] * lines_in[fy_half_size][x];
        }
        // clip/saturate to uint8_t
        if (r < 0)
          line_out[x] = 0;
        else if (r > UINT8_MAX)
          line_out[x] = UINT8_MAX;
        else
          line_out[x] = (uint8_t)r;
      }
    }
  }
}
