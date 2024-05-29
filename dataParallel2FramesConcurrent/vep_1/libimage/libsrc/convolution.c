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

void convolution(uint8_t const volatile *const frame_in, uint32_t const volatile xsize_in, uint32_t const volatile ysize_in, uint32_t const volatile block_ysize, uint32_t const volatile bitsperpixel_in,
                 double const *const f, uint32_t const fxsize, uint32_t const fysize, uint8_t volatile *const frame_out, uint32_t const frame_half)
{
  uint32_t y_offset = (frame_half == 0) ? 0 : ysize_in - block_ysize;
  for (uint32_t y = 0; y < block_ysize; y++)
  {
    for (uint32_t x = 0; x < xsize_in; x++)
    {
      double r = 0;
      for (uint32_t ty = 0; ty < fysize; ty++)
      {
        for (uint32_t tx = 0; tx < fxsize; tx++)
        {
          if (x + tx >= fxsize / 2 && x + tx - fxsize / 2 < xsize_in &&
              y + y_offset + ty >= fysize / 2 && y + y_offset + ty - fysize / 2 < ysize_in)
          {
            r += f[ty * fxsize + tx] * frame_in[((y + ty - fysize / 2) * xsize_in + x + tx - fxsize / 2)];
          }
          else
          {
            // use centre pixel when over the border
            r += f[ty * fxsize + tx] * frame_in[(y * xsize_in + x)];
          }
        }
      }
      // clip/saturate to uint8_t
      if (r < 0)
        frame_out[(y * xsize_in + x)] = 0;
      else if (r > UINT8_MAX)
        frame_out[(y * xsize_in + x)] = UINT8_MAX;
      else
        frame_out[(y * xsize_in + x)] = (uint8_t)r;
    }
  }
}
