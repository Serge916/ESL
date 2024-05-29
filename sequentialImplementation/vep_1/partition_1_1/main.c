#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region 
#define MEM1 vep_tile1_partition2_shared_region
#define BITS_PER_PIXEL 8


int main (void)
{
  uint64_t et_start, et_end, rt_start, rt_end, t;
  t = read_global_timer();
  xil_printf("%04u/%010u: waiting for image\n", (uint32_t)(t>>32), (uint32_t)t);
  while(1) {

    if(MEM0->current_work.busy == 1){
      t = read_global_timer();
      xil_printf("%04u/%010u: work commanded!\n", (uint32_t)(t>>32), (uint32_t)t);
      uint32_t xsize = MEM0->current_work.x_size;
      uint32_t ysize = MEM0->current_work.y_size;
      uint32_t bitsperpixelIn = MEM0->current_work.bits_per_pixel;


      greyscale(MEM0->pixel_space, xsize, ysize, bitsperpixelIn, MEM0->pixel_space);

      // convolution (average or gaussian blur)
      double const * filter;
      uint32_t filter_size;
      if (bitsperpixelIn == 8) {
        filter = conv_avgxy3;
        filter_size = 3;
      } else {
        filter = conv_gaussianblur5;
        filter_size = 5;
      }
      convolution(MEM0->pixel_space, xsize, ysize, BITS_PER_PIXEL, filter, filter_size, filter_size, MEM1->pixel_space);

      // sobel
      uint8_t const threshold = (bitsperpixelIn == 8 ? 100 : 128);
      sobel(MEM1->pixel_space, xsize, ysize, BITS_PER_PIXEL, threshold, MEM0->pixel_space);

      // overlay
      overlay(MEM1->pixel_space, xsize, ysize, BITS_PER_PIXEL, MEM0->pixel_space, xsize, ysize, BITS_PER_PIXEL, 0, 0, 0.7, MEM0->pixel_space);
     
      t = read_global_timer();
      xil_printf("%04u/%010u: image delivered\n", (uint32_t)(t>>32), (uint32_t)t);
      MEM0->current_work.busy = 0;
    }
  }
}
