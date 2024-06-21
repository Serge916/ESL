#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region
#define MEM1 vep_tile1_partition1_shared_region
#define MEM2 vep_tile2_partition1_shared_region
#define RATIO (double)0.7

// OVERLAY NODE
int main(void)
{
  xil_printf("starting the Overlay to ARM buffer\n");

  if (fifo_init(&MEM0->admin_out, MEM0->lines_out, LINE_OUT_BUFFER_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM0->admin_out);
  // Wait for convolution and sobel fifos
  line_t line_in_conv;
  line_t line_in_sobel;
  line_t line_out;
  uint64_t t, t_start;

  while (!(fifo_initialized(&MEM2->admin_sobel_out) && fifo_initialized(&MEM1->admin_conv_out)))
  {
  }

  while (1)
  {
#ifdef TIME_MEASURE
    t_start = read_partition_timer();
#endif
    if (!(fifo_tokens(&MEM2->admin_sobel_out) && fifo_tokens(&MEM1->admin_conv_out)))
    {
      continue;
    }

    fifo_read_token(&MEM2->admin_sobel_out, &line_in_sobel);
    fifo_read_token(&MEM1->admin_conv_out, &line_in_conv);
    uint32_t length = line_in_conv.length;
    if (line_in_sobel.y_position != line_in_conv.y_position)
    {
      t = read_global_timer();
      xil_printf("%04u/%010u: Lines do not belong together. Sobel %d, conv %d\n", (uint32_t)(t >> 32), (uint32_t)t, line_in_sobel.y_position, line_in_conv.y_position);
      continue;
    }

    t = read_global_timer();
    xil_printf("%04u/%010u: Overlay: %d\n", (uint32_t)(t >> 32), (uint32_t)t, line_in_conv.y_position);

    overlay(line_in_conv.pixel_space, line_in_sobel.pixel_space, length, RATIO, line_out.pixel_space);
    line_out.length = length;
    line_out.y_position = line_in_conv.y_position;
    line_out.y_size = line_in_conv.y_size;
    line_out.isRGB = line_in_conv.isRGB;

    fifo_write_token(&MEM0->admin_out, &line_out);
#ifdef TIME_MEASURE
    // Here to avoid non steady state measurements
    t = read_partition_timer();
    if (MEM0->wcet[4] < t - t_start)
    {
      uint64_t diff = t - t_start;
      MEM0->wcet[4] = t - t_start;
    }
#endif
  }

  return 0;
}
