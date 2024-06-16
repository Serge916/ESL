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
// GREYSCALE NODE
int main(void)
{
    uint64_t t;
    uint32_t pixels_in_block;
    uint32_t convolution_buffer;

    while (1)
    {
        switch (MEM0->current_work.busy)
        {
        case 0:
            break;
        case 1:
            t = read_global_timer();
            xil_printf("%04u/%010u: Greyscaling\n", (uint32_t)(t >> 32), (uint32_t)t);
            pixels_in_block = MEM0->current_work.lines_in_block * MEM0->current_work.x_size;
            convolution_buffer = (MEM0->current_work.bytes_per_pixel == 1) ? MEM0->current_work.x_size : 2 * MEM0->current_work.x_size;
            if (MEM0->current_work.bytes_per_pixel == 3)
            {
                // Normal block
                greyscale(&MEM0->pixel_space[convolution_buffer], MEM0->current_work.bytes_per_pixel, pixels_in_block + convolution_buffer, &MEM0->pixel_space[convolution_buffer]);
                // Upper buffer
                // greyscale(&MEM0->pixel_space[convolution_buffer + pixels_in_block], MEM0->current_work.bytes_per_pixel, convolution_buffer, &MEM0->pixel_space[convolution_buffer + pixels_in_block]);
            }

            for (uint32_t index = 0; index < convolution_buffer; index++)
            {
                MEM0->pixel_space[index] = MEM0->pixel_space[pixels_in_block - convolution_buffer - 1 + index];
            }

            t = read_global_timer();
            xil_printf("%04u/%010u: Finished greyscaling\n", (uint32_t)(t >> 32), (uint32_t)t);
            MEM0->current_work.busy = 0;
            break;
        case 2:
            break;
        case 3:
            t = read_global_timer();
            xil_printf("%04u/%010u: Overlaying\n", (uint32_t)(t >> 32), (uint32_t)t);
            pixels_in_block = MEM0->current_work.lines_in_block * MEM0->current_work.x_size;
            // xil_printf("Overlaying %d pixels\n", pixels_in_block);
            convolution_buffer = MEM0->current_work.bytes_per_pixel == 1 ? MEM0->current_work.x_size : 2 * MEM0->current_work.x_size;
            uint32_t sobel_buffer = MEM0->current_work.x_size;

            overlay(&MEM1->pixel_space[sobel_buffer], &MEM0->pixel_space[convolution_buffer], pixels_in_block, &MEM0->pixel_space[convolution_buffer]);
            MEM0->current_work.busy = 0;
            break;

        default:
            t = read_global_timer();
            xil_printf("%04u/%010u: Unknown value in busy flag: %d\n", (uint32_t)(t >> 32), (uint32_t)t, MEM0->current_work.busy);
            break;
        }
    }

    return 0;
}
