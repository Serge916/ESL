#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region

// OVERLAY NODE
int main(void)
{
  uint64_t t;
  t = read_global_timer();
  xil_printf("%04u/%010u: waiting for image\n", (uint32_t)(t >> 32), (uint32_t)t);
  return 0;
}
