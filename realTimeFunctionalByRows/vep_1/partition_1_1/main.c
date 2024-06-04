#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region
#define MEM1 vep_tile1_partition1_shared_region // To be replaced for a private memory pointer (faster)
#define MEM2 vep_tile2_partition1_shared_region

// CONVOLUTION NODE
int main(void)
{

  xil_printf("starting the Greyscale to Convolution buffer\n");
  if (fifo_init(&MEM1->admin_conv_in, MEM1->lines_in, CONVOLUTION_BUFFER_IN_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM1->admin_conv_in);

  xil_printf("starting the Convolution to Overlay buffer\n");
  if (fifo_init(&MEM1->admin_conv_out, MEM1->lines_out, CONVOLUTION_BUFFER_OUT_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM1->admin_conv_out);
  // Check if Convolution to Sobel buffer is ready
  while (!(fifo_initialized(&MEM2->admin_sobel_in)))
  {
  }

  // uint64_t t;
  // t = read_global_timer();
  // xil_printf("%04u/%010u: Hello :)\n", (uint32_t)(t >> 32), (uint32_t)t);

  double const *filter;
  uint32_t filter_size;
  line_t line_in;
  line_t line_out;
  uint8_t bytes_in[5][MAX_LINE_SIZE] = {0};

  while (1)
  {
    if (!fifo_tokens(&MEM1->admin_conv_in))
    {
      continue;
    }
    fifo_read_token(&MEM1->admin_conv_in, &line_in);
    // Move the lines one down
    filter = line_in.isRGB ? conv_gaussianblur5 : conv_avgxy3;
    filter_size = line_in.isRGB ? 5 : 3;
    for (uint32_t i = 0; i < filter_size - 1; i++)
    {
      for (uint32_t j = 0; j < MAX_LINE_SIZE; j++)
      {
        bytes_in[i][j] = bytes_in[i + 1][j];
      }
    }
    // Insert new line
    for (uint32_t j = 0; j < MAX_LINE_SIZE; j++)
    {
      bytes_in[filter_size - 1][j] = line_in.pixel_space[j];
    }
    // If there are not enough lines yet, continue
    if (line_in.y_position < filter_size)
    {
      continue;
    }
    // Y position is half filter lower than last line
    uint32_t y_position = line_in.y_position - filter_size / 2;
    uint32_t length = line_in.length;
    uint32_t y_size = line_in.y_size;
    xil_printf("Convoluting: %d\n", y_position);
    convolution(bytes_in, length, y_position, y_size, filter, filter_size, filter_size, line_out.pixel_space);
    // Format line out
    line_out.y_position = y_position;
    line_out.y_size = y_size;
    line_out.length = length;
    line_out.isRGB = line_in.isRGB;
    fifo_write_token(&MEM2->admin_sobel_in, &line_out);
    fifo_write_token(&MEM1->admin_conv_out, &line_out);
  }

  return 0;
}