#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region
#define BITS_PER_PIXEL 8
#define WORK_NO (TILE_ID < 2 ? 0 : 1)
// #define FRAME_HALF (TILE_ID+PARTITION_ID % 2 == 0 ? 0 : 1)
#if TILE_ID == 0
#define FRAME_HALF 0
#elif TILE_ID == 1
#define FRAME_HALF 1
#elif TILE_ID == 2
#define FRAME_HALF (PARTITION_ID < 2 ? 0 : 1)
#endif

void barrier(uint32_t state)
{
  MEM0->current_work[WORK_NO].sync[FRAME_HALF] = state;
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: Stopped at barrier %d. Flag state 0x%08x/0x%08x\n", (uint32_t)(t >> 32), (uint32_t)t, state, MEM0->current_work[WORK_NO].sync[1], MEM0->current_work[WORK_NO].sync[0]);
  // Wait for all threads to arrive
  for (uint32_t i = 0; i < 2; i++)
  {
    while (MEM0->current_work[WORK_NO].sync[i] < state)
    {
    }
  }
}

int main(void)
{
  uint8_t volatile *mem1 = (uint8_t *)vep_memories_shared_remote_start[WORK_NO][1];
  uint32_t barrier_counter = 0;
  uint64_t t;
  t = read_global_timer();
  xil_printf("%04u/%010u: waiting for image\n", (uint32_t)(t >> 32), (uint32_t)t);
  while (1)
  {

    while (MEM0->current_work[WORK_NO].busy == 0)
    {
    }
    t = read_global_timer();
    xil_printf("%04u/%010u: work commanded!\n", (uint32_t)(t >> 32), (uint32_t)t);
    uint32_t xsize = MEM0->current_work[WORK_NO].x_size;
    uint32_t ysize = MEM0->current_work[WORK_NO].y_size;
    uint32_t bitsperpixelIn = MEM0->current_work[WORK_NO].bits_per_pixel;
    MEM0->current_work[WORK_NO].sync[FRAME_HALF] = 0;
    // frameA for WORK_2 starts at end location minus the pixels
    volatile uint8_t *const frameA = (WORK_NO == 0) ? MEM0->pixel_space : (&MEM0->pixel_space[ALLOCATED_BYTES_MEMS - 1 - xsize * ysize * bitsperpixelIn / 8]);
    barrier_counter = 0;
    uint32_t block_ysize = ysize / 2 + (ysize % 2) * FRAME_HALF;
    uint32_t block_size = xsize * block_ysize;
    xil_printf("Block size is %d\n", block_size);
    // xil_printf("x: %d, y: %d, bits: %d\n", xsize, ysize, bitsperpixelIn);
    // xil_printf("frameA points to %p, having being offset by %d\n", &frameA[bitsperpixelIn / 8 * block_size * FRAME_HALF], ALLOCATED_BYTES_MEMS - 1 - xsize * ysize * bitsperpixelIn);
    t = read_global_timer();
    xil_printf("%04u/%010u: Greyscaling\n", (uint32_t)(t >> 32), (uint32_t)t);
    greyscale(&frameA[bitsperpixelIn / 8 * block_size * FRAME_HALF], xsize, block_ysize, bitsperpixelIn, &frameA[bitsperpixelIn / 8 * block_size * FRAME_HALF]);

    barrier(++barrier_counter);

    if (bitsperpixelIn != 8)
    {
      t = read_global_timer();
      xil_printf("%04u/%010u: Offsetting\n", (uint32_t)(t >> 32), (uint32_t)t);
      offset(&frameA[bitsperpixelIn / 8 * block_size * FRAME_HALF], &frameA[(xsize * ysize - block_size) * FRAME_HALF], block_size, bitsperpixelIn);
    }
    barrier(++barrier_counter);
    if (FRAME_HALF == 0)
    {
      MEM0->current_work[WORK_NO].busy = 2;
    }

    barrier(++barrier_counter);

    t = read_global_timer();
    xil_printf("%04u/%010u: Convoluting\n", (uint32_t)(t >> 32), (uint32_t)t);

    // convolution (average or gaussian blur)
    double const *filter;
    uint32_t filter_size;
    if (bitsperpixelIn == 8)
    {
      filter = conv_avgxy3;
      filter_size = 3;
    }
    else
    {
      filter = conv_gaussianblur5;
      filter_size = 5;
    }

    convolution(&frameA[(xsize * ysize - block_size) * FRAME_HALF], xsize, ysize, block_ysize, BITS_PER_PIXEL, filter, filter_size, filter_size, &mem1[(xsize * ysize - block_size) * FRAME_HALF], FRAME_HALF);

    barrier(++barrier_counter);

    t = read_global_timer();
    xil_printf("%04u/%010u: Sobel\n", (uint32_t)(t >> 32), (uint32_t)t);
    // sobel
    uint8_t const threshold = (bitsperpixelIn == 8 ? 100 : 128);
    sobel(&mem1[(xsize * ysize - block_size) * FRAME_HALF], xsize, ysize, block_ysize, BITS_PER_PIXEL, threshold, &frameA[(xsize * ysize - block_size) * FRAME_HALF], FRAME_HALF);
    barrier(++barrier_counter);

    t = read_global_timer();
    xil_printf("%04u/%010u: Overlay\n", (uint32_t)(t >> 32), (uint32_t)t);
    // overlay
    overlay(&mem1[(xsize * ysize - block_size) * FRAME_HALF], xsize, block_ysize, BITS_PER_PIXEL, &frameA[(xsize * ysize - block_size) * FRAME_HALF], xsize, block_ysize, BITS_PER_PIXEL, 0, 0, 0.7, &frameA[(xsize * ysize - block_size) * FRAME_HALF]);

    barrier(++barrier_counter);

    if (FRAME_HALF == 0)
    {
      MEM0->current_work[WORK_NO].busy = 0;
    }
    barrier(++barrier_counter);
    t = read_global_timer();
    xil_printf("%04u/%010u: image delivered\n", (uint32_t)(t >> 32), (uint32_t)t);
  }
}
