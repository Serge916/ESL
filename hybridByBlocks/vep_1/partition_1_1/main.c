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
  volatile uint8_t *frameA;
  volatile uint8_t *frameB;

  uint32_t y_size;
  uint32_t x_size;
  uint32_t lines_per_frame;
  uint32_t lines_in_block;
  uint32_t lines_for_convolution;
  uint32_t initial_y;
  uint32_t bytes_per_pixel;
  uint32_t convolution_buffer;
  double const *filter;
  uint32_t filter_size;
  uint8_t threshold;

  uint64_t t;
  t = read_global_timer();
  xil_printf("%04u/%010u: waiting for image\n", (uint32_t)(t >> 32), (uint32_t)t);

  while (1)
  {
    if (MEM0->current_work.busy != 2)
    {
      continue;
    }
    barrier(++barrier_counter);

    initial_y = MEM0->current_work.initial_y;
    // If first block, configure the execution
    if (initial_y == 0)
    {
      y_size = MEM0->current_work.y_size;
      x_size = MEM0->current_work.x_size;
      lines_per_frame = MEM0->current_work.lines_in_block / 2 + (MEM0->current_work.lines_in_block % 2) * FRAME_HALF;
      bytes_per_pixel = MEM0->current_work.bytes_per_pixel;
      convolution_buffer = (bytes_per_pixel == 1) ? x_size * 1 : x_size * 2;

      frameA = MEM0->pixel_space;
      frameB = MEM1->pixel_space;

      // convolution (average or gaussian blur)
      filter = (bytes_per_pixel == 1) ? conv_avgxy3 : conv_gaussianblur5;
      filter_size = (bytes_per_pixel == 1) ? 3 : 5;
      threshold = (bytes_per_pixel == 1 ? 100 : 128);

      barrier_counter = 0;

      // for (uint32_t i = 0; i < y_size * x_size; i++)
      // {
      //   frameB[i + x_size] = 0;
      // }
    }

    if (FRAME_HALF == 1)
    {
      if (initial_y + lines_in_block < y_size - 1)
      {
        lines_for_convolution = (bytes_per_pixel == 1) ? lines_per_frame - 1 : lines_per_frame - 2;
        xil_printf("Reducing line amount! line no. %d\n", lines_for_convolution);
      }
      else
      {
        lines_for_convolution = lines_per_frame;
        xil_printf("Reached final line! line no. %d\n", lines_for_convolution);
      }
    }

    barrier(++barrier_counter);
    lines_in_block = MEM0->current_work.lines_in_block;

    t = read_global_timer();
    xil_printf("%04u/%010u: Convoluting\n", (uint32_t)(t >> 32), (uint32_t)t);

    convolution(&frameA[convolution_buffer], x_size, y_size, lines_for_convolution, filter, filter_size, &frameB[x_size], FRAME_HALF, initial_y, lines_in_block);

    // barrier(++barrier_counter);

    // if (FRAME_HALF == 1)
    // {
    //   // Convolution
    //   for (uint32_t index = 0; index < convolution_buffer; index++)
    //   {
    //     frameA[index] = frameA[index + convolution_buffer + lines_in_block * x_size];
    //   }
    // }

    // barrier(++barrier_counter);
    // // sobel
    // sobel(&frameB[x_size], x_size, y_size, lines_per_frame, bytes_per_pixel, threshold, &frameA[x_size * FRAME_HALF * (y_size - lines_in_thread) + function_buffer], FRAME_HALF, initial_y);
    // // Save lines for next block arrival
    // if (initial_y == 0 && FRAME_HALF == 1)
    // {
    //   for (uint32_t index = x_size * (y_size - filter_size / 2); index < x_size * y_size - 1; index++)
    //   {
    //     frameB[index - x_size * (y_size - filter_size / 2)] = frameB[index];
    //   }
    // }

    barrier(++barrier_counter);

    // Store previous lines for convolution and sobel
    if (FRAME_HALF == 1)
    {
      // Convolution
      // for (uint32_t index = 0; index < convolution_buffer; index++)
      // {
      //   frameA[index] = frameA[index + convolution_buffer + lines_in_block * x_size];
      //   // frameA[convolution_buffer - (index + 1)] = frameA[x_size * lines_in_block - (index + 1) + convolution_buffer];
      //   // xil_printf("moving pixel from %p to %p\n", &frameA[convolution_buffer - (index + 1)], &frameA[x_size * lines_in_block - (index + 1) + convolution_buffer]);
      // }
      // Sobel
      // for (uint32_t index = 0; index < x_size; index++)
      // {
      //   frameB[x_size - (index + 1)] = frameB[x_size * lines_per_frame - (index + 1)];
      // }
      // Notify the next partition
      MEM0->current_work.busy = 3;
    }

    barrier(++barrier_counter);
  }
}
