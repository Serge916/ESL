#include <stddef.h>
#include <stdint.h>
#include <platform.h>
#include <vep_memory_map.h>
#include <vep_shared_memory_regions.h>

/***** DO NOT MODIFY THIS FILE *****/

#ifdef VEP_MEMSHARED0_SHARED_REGION_REMOTE_START
vep_memshared0_shared_region_t volatile * const vep_memshared0_shared_region = (vep_memshared0_shared_region_t volatile *) VEP_MEMSHARED0_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_memshared0_shared_region_t) <= VEP_MEMSHARED0_SHARED_REGION_SIZE, "vep_memshared0_shared_region does not fit in the VEP_MEMSHARED0_SHARED_REGION");
#endif

#ifdef VEP_TILE0_PARTITION1_SHARED_REGION_REMOTE_START
vep_tile0_partition1_shared_region_t volatile * const vep_tile0_partition1_shared_region = (vep_tile0_partition1_shared_region_t volatile *) VEP_TILE0_PARTITION1_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile0_partition1_shared_region_t) <= VEP_TILE0_PARTITION1_SHARED_REGION_SIZE, "vep_tile0_partition1_shared_region does not fit in the VEP_TILE0_PARTITION1_SHARED_REGION");
#endif
#ifdef VEP_TILE0_PARTITION2_SHARED_REGION_REMOTE_START
vep_tile0_partition2_shared_region_t volatile * const vep_tile0_partition2_shared_region = (vep_tile0_partition2_shared_region_t volatile *) VEP_TILE0_PARTITION2_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile0_partition2_shared_region_t) <= VEP_TILE0_PARTITION2_SHARED_REGION_SIZE, "vep_tile0_partition2_shared_region does not fit in the VEP_TILE0_PARTITION2_SHARED_REGION");
#endif
#ifdef VEP_TILE0_PARTITION3_SHARED_REGION_REMOTE_START
vep_tile0_partition3_shared_region_t volatile * const vep_tile0_partition3_shared_region = (vep_tile0_partition3_shared_region_t volatile *) VEP_TILE0_PARTITION3_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile0_partition3_shared_region_t) <= VEP_TILE0_PARTITION3_SHARED_REGION_SIZE, "vep_tile0_partition3_shared_region does not fit in the VEP_TILE0_PARTITION3_SHARED_REGION");
#endif
#ifdef VEP_TILE0_PARTITION4_SHARED_REGION_REMOTE_START
vep_tile0_partition4_shared_region_t volatile * const vep_tile0_partition4_shared_region = (vep_tile0_partition4_shared_region_t volatile *) VEP_TILE0_PARTITION4_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile0_partition4_shared_region_t) <= VEP_TILE0_PARTITION4_SHARED_REGION_SIZE, "vep_tile0_partition4_shared_region does not fit in the VEP_TILE0_PARTITION4_SHARED_REGION");
#endif
#ifdef VEP_TILE1_PARTITION1_SHARED_REGION_REMOTE_START
vep_tile1_partition1_shared_region_t volatile * const vep_tile1_partition1_shared_region = (vep_tile1_partition1_shared_region_t volatile *) VEP_TILE1_PARTITION1_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile1_partition1_shared_region_t) <= VEP_TILE1_PARTITION1_SHARED_REGION_SIZE, "vep_tile1_partition1_shared_region does not fit in the VEP_TILE1_PARTITION1_SHARED_REGION");
#endif
#ifdef VEP_TILE1_PARTITION2_SHARED_REGION_REMOTE_START
vep_tile1_partition2_shared_region_t volatile * const vep_tile1_partition2_shared_region = (vep_tile1_partition2_shared_region_t volatile *) VEP_TILE1_PARTITION2_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile1_partition2_shared_region_t) <= VEP_TILE1_PARTITION2_SHARED_REGION_SIZE, "vep_tile1_partition2_shared_region does not fit in the VEP_TILE1_PARTITION2_SHARED_REGION");
#endif
#ifdef VEP_TILE1_PARTITION3_SHARED_REGION_REMOTE_START
vep_tile1_partition3_shared_region_t volatile * const vep_tile1_partition3_shared_region = (vep_tile1_partition3_shared_region_t volatile *) VEP_TILE1_PARTITION3_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile1_partition3_shared_region_t) <= VEP_TILE1_PARTITION3_SHARED_REGION_SIZE, "vep_tile1_partition3_shared_region does not fit in the VEP_TILE1_PARTITION3_SHARED_REGION");
#endif
#ifdef VEP_TILE1_PARTITION4_SHARED_REGION_REMOTE_START
vep_tile1_partition4_shared_region_t volatile * const vep_tile1_partition4_shared_region = (vep_tile1_partition4_shared_region_t volatile *) VEP_TILE1_PARTITION4_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile1_partition4_shared_region_t) <= VEP_TILE1_PARTITION4_SHARED_REGION_SIZE, "vep_tile1_partition4_shared_region does not fit in the VEP_TILE1_PARTITION4_SHARED_REGION");
#endif
#ifdef VEP_TILE2_PARTITION1_SHARED_REGION_REMOTE_START
vep_tile2_partition1_shared_region_t volatile * const vep_tile2_partition1_shared_region = (vep_tile2_partition1_shared_region_t volatile *) VEP_TILE2_PARTITION1_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile2_partition1_shared_region_t) <= VEP_TILE2_PARTITION1_SHARED_REGION_SIZE, "vep_tile2_partition1_shared_region does not fit in the VEP_TILE2_PARTITION1_SHARED_REGION");
#endif
#ifdef VEP_TILE2_PARTITION2_SHARED_REGION_REMOTE_START
vep_tile2_partition2_shared_region_t volatile * const vep_tile2_partition2_shared_region = (vep_tile2_partition2_shared_region_t volatile *) VEP_TILE2_PARTITION2_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile2_partition2_shared_region_t) <= VEP_TILE2_PARTITION2_SHARED_REGION_SIZE, "vep_tile2_partition2_shared_region does not fit in the VEP_TILE2_PARTITION2_SHARED_REGION");
#endif
#ifdef VEP_TILE2_PARTITION3_SHARED_REGION_REMOTE_START
vep_tile2_partition3_shared_region_t volatile * const vep_tile2_partition3_shared_region = (vep_tile2_partition3_shared_region_t volatile *) VEP_TILE2_PARTITION3_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile2_partition3_shared_region_t) <= VEP_TILE2_PARTITION3_SHARED_REGION_SIZE, "vep_tile2_partition3_shared_region does not fit in the VEP_TILE2_PARTITION3_SHARED_REGION");
#endif
#ifdef VEP_TILE2_PARTITION4_SHARED_REGION_REMOTE_START
vep_tile2_partition4_shared_region_t volatile * const vep_tile2_partition4_shared_region = (vep_tile2_partition4_shared_region_t volatile *) VEP_TILE2_PARTITION4_SHARED_REGION_REMOTE_START;
_Static_assert(sizeof(vep_tile2_partition4_shared_region_t) <= VEP_TILE2_PARTITION4_SHARED_REGION_SIZE, "vep_tile2_partition4_shared_region does not fit in the VEP_TILE2_PARTITION4_SHARED_REGION");
#endif
