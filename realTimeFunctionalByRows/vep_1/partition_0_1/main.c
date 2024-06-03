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

// GREYSCALE NODE
int main(void)
{
  // uint64_t t;
  // t = read_global_timer();
  // xil_printf("%04u/%010u: \n", (uint32_t)(t >> 32), (uint32_t)t);

  xil_printf("starting on the RISC-V\n");
  if (fifo_init(&MEM0->admin_in, MEM0->lines_in, LINE_IN_BUFFER_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  fifo_print_status(&MEM0->admin_in);

  if (fifo_init(&MEM0->admin_out, MEM0->lines_out, LINE_OUT_BUFFER_SIZE, sizeof(line_t)) == NULL)
  {
    xil_printf("fifo_init failed\n");
    return EXIT_FAILURE;
  }
  // wait for the ARM to initialise its part of the fifo
  fifo_print_status(&MEM0->admin_out);

  // greyscale();
  return 0;
}
