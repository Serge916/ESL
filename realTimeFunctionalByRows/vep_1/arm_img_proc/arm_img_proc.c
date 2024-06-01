#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arm_shared_memory.h>
#include <unistd.h>
/* warning: this is the RISC-V memory map!
 * use only in conjuction with arm_shared_memory_write/read
 */
#include "vep_memory_map.h"

// two useful functions
extern uint32_t readBMP(char const *const file, uint8_t **outframe, uint32_t *const x_size, uint32_t *const y_size, uint32_t *const bitsperpixel);
extern uint32_t writeBMP(char const *const file, uint8_t const *const outframe, uint32_t const x_size, uint32_t const y_size, uint32_t const bitsperpixel);

vep_memshared0_shared_region_t volatile *data_in_shared_mem = NULL;

int main()
{
  printf("Hello :)");
  return 0;
}