/* DO NOT EDIT
 * this file was automatically generated by ../../tools/generate-json from vep-config.txt for VEP_ID=1
 */

#include <vep_memory_map.h>

uint32_t const vep_memories_shared_remote_start[NUM_SHARED_MEMORIES][NUM_PARTITIONS]= {
  { NOT_SHARED, NOT_SHARED, NOT_SHARED, NOT_SHARED, },
  { 0x90018000, NOT_SHARED, NOT_SHARED, NOT_SHARED, },
  { 0xA001B000, NOT_SHARED, NOT_SHARED, NOT_SHARED, },
  { 0xB0009000, 0xB0009000, 0xB0009000, 0xB0009000, },
};
uint32_t const vep_memories_shared_remote_size[NUM_SHARED_MEMORIES][NUM_PARTITIONS]= {
  { NOT_SHARED, NOT_SHARED, NOT_SHARED, NOT_SHARED, },
  { 0x00008000, NOT_SHARED, NOT_SHARED, NOT_SHARED, },
  { 0x00005000, NOT_SHARED, NOT_SHARED, NOT_SHARED, },
  { 0x00017000, 0x00017000, 0x00017000, 0x00017000, },
};

#ifdef PARTITION_ID
#include <xil_printf.h>
#define print xil_printf
#else
#include <stdio.h>
#define print printf
#endif

void print_vep_memory_map(void)
{
  // regions shared by partitions
  for (uint32_t m=0; m < NUM_TILES; m++) {
    for (uint32_t p=0; p < NUM_PARTITIONS; p++) {
      if (vep_memories_shared_remote_start[m][p] != NOT_SHARED)
        print("in memory %s partition %u of vep %u shares region of %uKiB starting at 0x%08X\n",
          shared_mems[m], p, VEP_ID, vep_memories_shared_remote_size[m][p]/1024, vep_memories_shared_remote_start[m][p]);
    }
  }
  // region shared by the VEP
  // no need for a partition loop since all partitions access the region at the same address
  for (uint32_t m=NUM_TILES; m < NUM_SHARED_MEMORIES; m++) {
    if (vep_memories_shared_remote_start[m][0] != NOT_SHARED)
        print("in memory %s vep %u shares region of %uKiB starting at 0x%08X\n",
          shared_mems[m], VEP_ID, vep_memories_shared_remote_size[m][0]/1024, vep_memories_shared_remote_start[m][0]);
  }
}
