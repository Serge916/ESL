#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region
#define MEM2 vep_tile2_partition1_shared_region

// SOBEL NODE
int main(void)
{
  xil_printf("starting the Convolution to Sobel buffer\n");
  if (fifo_init(&MEM2->admin_sobel_in, MEM2->lines_in, SOBEL_BUFFER_IN_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM2->admin_sobel_in);

  xil_printf("starting the Sobel to Overlay buffer\n");
  if (fifo_init(&MEM2->admin_sobel_out, MEM2->lines_in, SOBEL_BUFFER_OUT_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM2->admin_sobel_out);

  uint64_t t;
  line_t line_in;
  line_t line_out;
  uint8_t bytes_in[3][MAX_LINE_SIZE] = {0};
  while (1)
  {
    if (!fifo_tokens(&MEM2->admin_sobel_in))
    {
      continue;
    }
    fifo_read_token(&MEM2->admin_sobel_in, &line_in);
    // Move the lines one down
    for (uint32_t i = 0; i < 3 - 1; i++)
    {
      for (uint32_t j = 0; j < MAX_LINE_SIZE; j++)
      {
        bytes_in[i][j] = bytes_in[i + 1][j];
      }
    }
    // Insert new line
    for (uint32_t j = 0; j < MAX_LINE_SIZE; j++)
    {
      bytes_in[2][j] = line_in.pixel_space[j];
    }
    // If there are not enough lines yet, continue
    if (line_in.y_position < 1)
    {
      continue;
    }
    // First run
    if (line_in.y_position == 1)
    {
      // Y position is one position lower than last line
      uint32_t y_position = 0;
      uint32_t length = line_in.length;
      uint32_t y_size = line_in.y_size;

      t = read_global_timer();
      xil_printf("%04u/%010u: Sobel: %d\n", (uint32_t)(t >> 32), (uint32_t)t, y_position);

      // sobel
      uint8_t threshold = line_in.isRGB ? 128 : 100;
      sobel(&bytes_in[1], length, y_position, y_size, threshold, line_out.pixel_space);
      // Format line out
      line_out.y_position = y_position;
      line_out.y_size = y_size;
      line_out.length = length;
      line_out.isRGB = line_in.isRGB;
      // fifo_write_token(&MEM0->admin_out, &line_out);
      fifo_write_token(&MEM2->admin_sobel_out, &line_out);
      continue;
    }

    // Y position is one position lower than last line
    uint32_t y_position = line_in.y_position - 1;
    uint32_t length = line_in.length;
    uint32_t y_size = line_in.y_size;

    t = read_global_timer();
    xil_printf("%04u/%010u: Sobel: %d\n", (uint32_t)(t >> 32), (uint32_t)t, y_position);

    // sobel
    uint8_t threshold = line_in.isRGB ? 128 : 100;
    sobel(bytes_in, length, y_position, y_size, threshold, line_out.pixel_space);
    // Format line out
    line_out.y_position = y_position;
    line_out.y_size = y_size;
    line_out.length = length;
    line_out.isRGB = line_in.isRGB;
    // fifo_write_token(&MEM0->admin_out, &line_out);
    fifo_write_token(&MEM2->admin_sobel_out, &line_out);

    if (line_in.y_position == line_in.y_size - 1)
    {
      y_position++;
      t = read_global_timer();
      xil_printf("%04u/%010u: Sobel: %d\n", (uint32_t)(t >> 32), (uint32_t)t, y_position);

      sobel((uint8_t *)bytes_in, length, y_position, y_size, threshold, line_out.pixel_space);

      line_out.y_position = y_position;
      line_out.y_size = y_size;
      line_out.length = length;
      line_out.isRGB = line_in.isRGB;
      // fifo_write_token(&MEM0->admin_out, &line_out);
      fifo_write_token(&MEM2->admin_sobel_out, &line_out);
    }
  }

  return 0;
}
