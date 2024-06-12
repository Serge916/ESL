#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <timers.h>
#include <xil_printf.h>
#include <platform.h>
#include <vep_shared_memory_regions.h>
#include <image.h>

#define MEM0 vep_memshared0_shared_region
#define MEM1 vep_tile1_partition2_shared_region
#define MEM2 vep_tile2_partition2_shared_region
// GREYSCALE NODE
int main(void)
{
    uint64_t t;
    uint32_t pixels_in_block;

    while (1)
    {
        switch (MEM0->current_work.busy)
        {
        case 0:
            break;
        case 1:
            xil_printf("%04u/%010u: Greyscaling\n", (uint32_t)(t >> 32), (uint32_t)t);
            pixels_in_block = MEM0->current_work.bytes_per_pixel * MEM0->current_work.lines_in_block * MEM0->current_work.x_size;
            if (MEM0->current_work.bytes_per_pixel == 3)
            {
                greyscale(MEM0->pixel_space, MEM0->current_work.bytes_per_pixel, pixels_in_block, MEM0->pixel_space);
            }
            MEM0->current_work.busy = 2;
            break;
        case 2:
            break;
        case 3:
            xil_printf("%04u/%010u: Overlaying\n", (uint32_t)(t >> 32), (uint32_t)t);
            pixels_in_block = MEM0->current_work.lines_in_block * MEM0->current_work.x_size;
            overlay(MEM1->pixel_space, MEM2->pixel_space, pixels_in_block, MEM0->pixel_space);
            MEM0->current_work.busy = 0;
            break;

        default:
            xil_printf("%04u/%010u: Unknown value in busy flag: %d\n", (uint32_t)(t >> 32), (uint32_t)t, MEM0->current_work.busy);
            break;
        }
    }

    return 0;
}
