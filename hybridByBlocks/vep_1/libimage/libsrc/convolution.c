#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <xil_printf.h>

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

void convolution(uint8_t const volatile *const frame_in, uint32_t const volatile xsize_in, uint32_t const volatile ysize_in, uint32_t const volatile lines_in_frame,
                 double const *const f, uint32_t const filter_size, uint8_t volatile *const frame_out, uint32_t const frame_half, uint32_t const initial_y, uint32_t const lines_in_block)
{
  uint32_t y_offset = (frame_half == 0) ? 0 : lines_in_block - lines_in_frame;
  uint32_t y_frame = y_offset;
  y_offset += initial_y;
  for (uint32_t y = 0; y < lines_in_frame; y++)
  {
    for (uint32_t x = 0; x < xsize_in; x++)
    {
      double r = 0;
      for (uint32_t ty = 0; ty < filter_size; ty++)
      {
        for (uint32_t tx = 0; tx < filter_size; tx++)
        {
          if (x + tx >= filter_size / 2 && x + tx - filter_size / 2 < xsize_in &&
              y + y_offset + ty >= filter_size / 2 && y + y_offset + ty - filter_size / 2 < ysize_in)
          {
            r += f[ty * filter_size + tx] * frame_in[((y + y_frame + ty - filter_size / 2) * xsize_in + x + tx - filter_size / 2)];
          }
          else
          {
            // use center pixel when over the border
            r += f[ty * filter_size + tx] * frame_in[((y + y_frame) * xsize_in + x)];
          }
        }
      }
      // clip/saturate to uint8_t
      if (r < 0)
        frame_out[((y + y_frame) * xsize_in + x)] = 0;
      else if (r > UINT8_MAX)
        frame_out[((y + y_frame) * xsize_in + x)] = UINT8_MAX;
      else
        frame_out[((y + y_frame) * xsize_in + x)] = (uint8_t)r;
    }
  }
}
