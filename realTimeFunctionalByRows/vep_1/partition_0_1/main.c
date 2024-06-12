#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>
#include <fifo.h>

#define MEM0 vep_memshared0_shared_region
#define MEM1 vep_tile1_partition1_shared_region
// GREYSCALE NODE
int main(void)
{
  uint64_t t;

  xil_printf("starting the ARM to Greyscale buffer\n");
  if (fifo_init(&MEM0->admin_in, MEM0->lines_in, LINE_IN_BUFFER_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM0->admin_in);
  // Wait for Convolution buffer initialization
  // while (!fifo_initialized(&MEM1->admin_conv_in))
  while (!fifo_initialized(&MEM0->admin_out))
  {
  }
  line_t lines_in[3];
  line_t line_out;
  uint32_t y_positions[3] = {UINT8_MAX};
  uint32_t length = 0;

  while (1)
  {
    if (fifo_tokens(&MEM0->admin_in))
    {
      // Store first line
      fifo_read_token(&MEM0->admin_in, &lines_in[0]);
      // If RGB, greyscale
      if (lines_in[0].isRGB == 1)
      {
        fifo_read_token(&MEM0->admin_in, &lines_in[1]);
        fifo_read_token(&MEM0->admin_in, &lines_in[2]);
        // Check if the lines belong together
        if (!(lines_in[0].y_position == lines_in[1].y_position && lines_in[1].y_position == lines_in[2].y_position))
        {
          xil_printf("The lines do not belong together!\n");
          continue;
        }

        t = read_global_timer();
        xil_printf("%04u/%010u: Greyscaling: %d\n", (uint32_t)(t >> 32), (uint32_t)t, lines_in[2].y_position);

        length = lines_in[0].length;
        uint8_t *bytes_in[3] = {lines_in[0].pixel_space, lines_in[1].pixel_space, lines_in[2].pixel_space};
        greyscale(bytes_in, length, 24, line_out.pixel_space);
        line_out.y_position = lines_in[0].y_position;
        line_out.y_size = lines_in[0].y_size;
        line_out.length = length;
        line_out.isRGB = 1;

        fifo_write_token(&MEM0->admin_out, &line_out);
        // fifo_write_token(&MEM1->admin_conv_in, &line_out);
      } // If greyscale, output it
      else
      {
        fifo_write_token(&MEM1->admin_conv_in, &lines_in[0]);
      }
    }
  }

  return 0;
}
