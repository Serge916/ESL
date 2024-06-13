/* DO NOT EDIT
 * this file was automatically generated by ../../tools/generate-json from vep-config.txt for VEP_ID=1
 */

#ifndef __VEP_MEMORY_MAP_H_
#define __VEP_MEMORY_MAP_H_

#include <stdint.h>
#include <platform.h>

#define VEP_MEMSHARED0_SHARED_REGION_SIZE                0x00017000
#define VEP_MEMSHARED0_SHARED_REGION_REMOTE_START        0xB0009000
#define VEP_TILE0_PARTITION1_IDMEM_SIZE                  0x00008000
#define VEP_TILE0_PARTITION1_IDMEM_START                 0x00000000 /* always 0 */
#define VEP_TILE1_PARTITION1_IDMEM_SIZE                  0x00008000
#define VEP_TILE1_PARTITION1_IDMEM_START                 0x00000000 /* always 0 */
#define VEP_TILE1_PARTITION2_IDMEM_SIZE                  0x00000000 /* without shared region */
#define VEP_TILE1_PARTITION2_IDMEM_START                 0x00000000 /* always 0 */
#define VEP_TILE1_PARTITION2_SHARED_REGION_SIZE          0x00010000
#define VEP_TILE1_PARTITION2_SHARED_REGION_LOCAL_START   0x00000000 /* only for partition_1_2 */
#define VEP_TILE1_PARTITION2_SHARED_REGION_REMOTE_START  0x90010000
#define VEP_TILE2_PARTITION1_IDMEM_SIZE                  0x00008000
#define VEP_TILE2_PARTITION1_IDMEM_START                 0x00000000 /* always 0 */

// the following arrays contain the same information as the #defines
// start/size of memory regions shared by partitions for memories 0..2: mem0 mem1 mem2 
// and by the vep (i.e. same for all partitions) for the shared memories 3..3: memshared0 
// thus you can access tile t, partition p's shared region at vep_memories_shared_remote_start[t][p]
// and the vep's shared region in memory m at vep_memories_shared_remote_start[m][p] (same for all partitions p)
// note that the system partition 0 is excluded; shown are user partitions 1..4
extern uint32_t const vep_memories_shared_remote_start[NUM_SHARED_MEMORIES][NUM_PARTITIONS];
extern uint32_t const vep_memories_shared_remote_size[NUM_SHARED_MEMORIES][NUM_PARTITIONS];
extern void print_vep_memory_map(void);
#endif
