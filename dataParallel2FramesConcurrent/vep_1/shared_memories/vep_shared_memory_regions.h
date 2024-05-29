#ifndef __VEP_PRIVATE_MEMORY_H_
#define __VEP_PRIVATE_MEMORY_H_

#include <stddef.h>
#include <stdint.h>
#include "vep_memory_map.h"

// IMPORTANT: to use a private memory region you must:
// 1- declare the shared memory region in the vep-config.txt file
//    without this, the region will not be declared in the memory map
// 2- modify the typedef of the struct containing all data to be placed in the region
//
// all fields are set to 0 when the VEP is loaded
// (it may therefore be useful to have an 'initialized' as first field in the struct)
//
// to debug, use the print_memory_map() function to print the VEP memory map
// it should be the same as the vep-config.txt from which it is generated

#define ALLOCATED_BYTES_MEMS 90000
#define ALLOCATED_BYTES_MEM_PRIV 64000
#define ALLOCATED_BYTES_MEM1_3 16000

typedef struct
{
  uint32_t x_size;
  uint32_t y_size;
  uint32_t busy;
  uint32_t sync[2];
  uint32_t bits_per_pixel;
  uint8_t *base_address;
} work_t;

typedef struct
{
  uint8_t R;
  uint8_t G;
  uint8_t B;
  uint8_t padding;
} pixel_t;

typedef struct
{
  uint8_t pixel_space[ALLOCATED_BYTES_MEM_PRIV];
} private_memory_regions_t;

#ifdef VEP_MEMSHARED0_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  work_t current_work[2];
  uint8_t pixel_space[ALLOCATED_BYTES_MEMS];
} vep_memshared0_shared_region_t;
#endif

#ifdef VEP_TILE0_PARTITION1_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile0_partition1_shared_region_t;
#endif
#ifdef VEP_TILE0_PARTITION2_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  private_memory_regions_t region;
} vep_tile0_partition2_shared_region_t;
#endif
#ifdef VEP_TILE0_PARTITION3_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile0_partition3_shared_region_t;
#endif
#ifdef VEP_TILE0_PARTITION4_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile0_partition4_shared_region_t;
#endif
#ifdef VEP_TILE1_PARTITION1_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile1_partition1_shared_region_t;
#endif
#ifdef VEP_TILE1_PARTITION2_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  private_memory_regions_t region;
} vep_tile1_partition2_shared_region_t;
#endif
#ifdef VEP_TILE1_PARTITION3_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile1_partition3_shared_region_t;
#endif
#ifdef VEP_TILE1_PARTITION4_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile1_partition4_shared_region_t;
#endif
#ifdef VEP_TILE2_PARTITION1_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile2_partition1_shared_region_t;
#endif
#ifdef VEP_TILE2_PARTITION2_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  private_memory_regions_t region;
} vep_tile2_partition2_shared_region_t;
#endif
#ifdef VEP_TILE2_PARTITION3_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile2_partition3_shared_region_t;
#endif
#ifdef VEP_TILE2_PARTITION4_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint32_t initialized;
} vep_tile2_partition4_shared_region_t;
#endif
#ifdef VEP_TILE3_PARTITION2_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  private_memory_regions_t region;
} vep_tile2_partition2_shared_region_t;
#endif
#ifdef VEP_TILE4_PARTITION2_SHARED_REGION_REMOTE_START
typedef volatile struct
{
  uint8_t pixel_space[ALLOCATED_BYTES_MEM1_2];
} vep_tile2_partition2_shared_region_t;
#endif

/***** DO NOT MODIFY THE CODE BELOW *****/

extern void print_vep_memory_map(void);

#ifdef VEP_MEMSHARED0_SHARED_REGION_REMOTE_START
extern vep_memshared0_shared_region_t volatile *const vep_memshared0_shared_region;
#endif

#ifdef VEP_TILE0_PARTITION1_SHARED_REGION_REMOTE_START
extern vep_tile0_partition1_shared_region_t volatile *const vep_tile0_partition1_shared_region;
#endif
#ifdef VEP_TILE0_PARTITION2_SHARED_REGION_REMOTE_START
extern vep_tile0_partition2_shared_region_t volatile *const vep_tile0_partition2_shared_region;
#endif
#ifdef VEP_TILE0_PARTITION3_SHARED_REGION_REMOTE_START
extern vep_tile0_partition3_shared_region_t volatile *const vep_tile0_partition3_shared_region;
#endif
#ifdef VEP_TILE0_PARTITION4_SHARED_REGION_REMOTE_START
extern vep_tile0_partition4_shared_region_t volatile *const vep_tile0_partition4_shared_region;
#endif
#ifdef VEP_TILE1_PARTITION1_SHARED_REGION_REMOTE_START
extern vep_tile1_partition1_shared_region_t volatile *const vep_tile1_partition1_shared_region;
#endif
#ifdef VEP_TILE1_PARTITION2_SHARED_REGION_REMOTE_START
extern vep_tile1_partition2_shared_region_t volatile *const vep_tile1_partition2_shared_region;
#endif
#ifdef VEP_TILE1_PARTITION3_SHARED_REGION_REMOTE_START
extern vep_tile1_partition3_shared_region_t volatile *const vep_tile1_partition3_shared_region;
#endif
#ifdef VEP_TILE1_PARTITION4_SHARED_REGION_REMOTE_START
extern vep_tile1_partition4_shared_region_t volatile *const vep_tile1_partition4_shared_region;
#endif
#ifdef VEP_TILE2_PARTITION1_SHARED_REGION_REMOTE_START
extern vep_tile2_partition1_shared_region_t volatile *const vep_tile2_partition1_shared_region;
#endif
#ifdef VEP_TILE2_PARTITION2_SHARED_REGION_REMOTE_START
extern vep_tile2_partition2_shared_region_t volatile *const vep_tile2_partition2_shared_region;
#endif
#ifdef VEP_TILE2_PARTITION3_SHARED_REGION_REMOTE_START
extern vep_tile2_partition3_shared_region_t volatile *const vep_tile2_partition3_shared_region;
#endif
#ifdef VEP_TILE2_PARTITION4_SHARED_REGION_REMOTE_START
extern vep_tile2_partition4_shared_region_t volatile *const vep_tile2_partition4_shared_region;
#endif
#ifdef VEP_TILE3_PARTITION2_SHARED_REGION_REMOTE_START
extern vep_tile3_partition2_shared_region_t volatile *const vep_tile3_partition2_shared_region;
#endif

#endif
