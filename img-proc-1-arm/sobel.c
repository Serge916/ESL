#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "image_processing_functions.h"

void sobel(uint8_t const * const frame_in, uint32_t const xsize_in, uint32_t const ysize_in, uint32_t const bitsperpixel_in, uint8_t const threshold, uint8_t * const frame_out)
{
  const int fxsize = 3, fysize = 3;
  int32_t const sobelx[] = {
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1,
  };
  int32_t const sobely[] = {
     1,  2,  1,
     0,  0,  0,
    -1, -2, -1
  };

  for (uint32_t y=0; y < ysize_in; y++) {
    for (uint32_t x=0; x < xsize_in; x++) {
        // don't create edge at borders
        if (x < (fxsize+1)/2 || x >= xsize_in-(fxsize+1)/2 || y < (fysize+1)/2 || y >= ysize_in-(fysize+1)/2) {
          frame_out[y*xsize_in+x] = 0;
        } else {
          int32_t xr = 0, yr = 0;
          for (int32_t ty=0; ty < fysize; ty++) {
            for (int32_t tx=0; tx < fxsize; tx++) {
              xr += sobelx[ty * fxsize + tx] * frame_in[(y + ty - fysize/2) * xsize_in + x + tx - fxsize/2];
              yr += sobely[ty * fxsize + tx] * frame_in[(y + ty - fysize/2) * xsize_in + x + tx - fxsize/2];
            }
          }
          // gradient magnitude
          double const r = yr*yr + xr*xr; 
          // clip/saturate from int32_t to uint8_t
          frame_out[y*xsize_in + x] = (r<(threshold*threshold))? 0:UINT8_MAX;
        
      }
    }
  }
}

