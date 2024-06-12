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
#define MEM2 vep_tile2_partition2_shared_region
#define FRAME_HALF (TILE_ID < 2 ? 0 : 1)

void barrier(uint32_t state)
{
  MEM0->current_work.sync[FRAME_HALF] = state;
  uint64_t t = read_global_timer();
  xil_printf("%04u/%010u: Stopped at barrier %d. Flag state 0x%08x/0x%08x\n", (uint32_t)(t >> 32), (uint32_t)t, state, MEM0->current_work.sync[1], MEM0->current_work.sync[0]);
  // Wait for all threads to arrive
  for (uint32_t i = 0; i < 2; i++)
  {
    while (MEM0->current_work.sync[i] < state)
    {
    }
  }
}

int main(void)
{
  uint32_t barrier_counter = 0;
  volatile uint8_t *const frameA = MEM0->pixel_space;
  volatile uint8_t *const frameB = MEM1->pixel_space;
  volatile uint8_t *const frameC = MEM2->pixel_space;
  uint64_t t;
  t = read_global_timer();
  xil_printf("%04u/%010u: waiting for image\n", (uint32_t)(t >> 32), (uint32_t)t);
  while (1)
  {
    if (MEM0->current_work.busy != 2)
    {
      continue;
    }
    uint32_t y_size = MEM0->current_work.y_size;
    uint32_t x_size = MEM0->current_work.x_size;
    uint32_t lines_in_thread = MEM0->current_work.lines_in_block / 2 + (MEM0->current_work.lines_in_block % 2) * FRAME_HALF;
    uint32_t bytes_per_pixel = MEM0->current_work.bytes_per_pixel;
    uint32_t initial_y = MEM0->current_work.initial_y;
    uint32_t function_buffer = (bytes_per_pixel == 3) ? x_size * 1 : x_size * 2;
    barrier_counter = 0;
    // Asign block type

    // convolution (average or gaussian blur)
    double const *filter;
    uint32_t filter_size;
    filter = (bytes_per_pixel == 1) ? conv_avgxy3 : conv_gaussianblur5;
    filter_size = (bytes_per_pixel == 1) ? 3 : 5;

    convolution(&frameA[x_size * FRAME_HALF * (y_size - lines_in_thread) + function_buffer], x_size, y_size, lines_in_thread, bytes_per_pixel, filter, filter_size, filter_size, &frameB[x_size * FRAME_HALF * (y_size - lines_in_thread) + function_buffer], FRAME_HALF, block_flag);
    // Save lines for next block arrival
    if (initial_y == 0 && FRAME_HALF == 1)
    {
      for (uint32_t index = x_size * (y_size - filter_size / 2); index < x_size * y_size - 1; index++)
      {
        frameB[index - x_size * (y_size - filter_size / 2)] = frameB[index];
      }
    }

    barrier(++barrier_counter);

    // sobel
    uint8_t const threshold = (bytes_per_pixel == 1 ? 100 : 128);
    sobel(&frameB[x_size * FRAME_HALF * (y_size - lines_in_thread) + function_buffer], x_size, y_size, lines_in_thread, bytes_per_pixel, threshold, &frameA[x_size * FRAME_HALF * (y_size - lines_in_thread) + function_buffer], FRAME_HALF, block_flag);
    // Save lines for next block arrival
    if (initial_y == 0 && FRAME_HALF == 1)
    {
      for (uint32_t index = x_size * (y_size - filter_size / 2); index < x_size * y_size - 1; index++)
      {
        frameB[index - x_size * (y_size - filter_size / 2)] = frameB[index];
      }
    }
    barrier(++barrier_counter);
    if (FRAME_HALF == 0)
    {
      MEM0->current_work.busy = 3;
    }
    barrier(++barrier_counter);
  }
}
