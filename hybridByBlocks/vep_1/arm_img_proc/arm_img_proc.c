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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s infile ...\n", argv[0]);
        return 1;
    }

    data_in_shared_mem = arm_shared_memory_init();
    if (data_in_shared_mem == NULL)
    {
        fprintf(stderr, "Something went wrong when initializing the shared memory pointer\n");
    }
    printf("Commanded %d jobs\n", argc - 1);

    for (int32_t f = 1; f <= argc - 1; f++)
    {
        // Insert blocks
        uint32_t xsize_snd, ysize_snd, bitsperpixel_snd;
        uint8_t *frame_snd = NULL;
        uint8_t *frame_buffer = NULL;
        if (!readBMP(argv[f], &frame_snd, &xsize_snd, &ysize_snd, &bitsperpixel_snd))
        {
            printf("Cannot read file %s\n", argv[f]);
            return 1; // To break the loop and escape
        }
        printf("Processing %s\n", argv[f]);
        frame_buffer = (uint8_t *)malloc(xsize_snd * ysize_snd);
        if (frame_buffer == NULL)
        {
            printf("Unsuccessful memory allocation\n");
            return 1;
        }

        uint32_t bytes_per_pixel = bitsperpixel_snd / 8;
        uint32_t const bytes_snd = xsize_snd * ysize_snd * bytes_per_pixel;
        // If frame is RGB, fits better
        uint32_t convolution_buffer = (bytes_per_pixel == 1) ? xsize_snd * 1 : xsize_snd * 2;
        uint32_t sobel_buffer = xsize_snd;
        uint32_t block_size = (bytes_per_pixel == 1) ? ALLOCATED_BYTES_MEM_PRIV : ALLOCATED_BYTES_MEMS;
        block_size -= (2 * convolution_buffer);

        uint32_t block_nr = bytes_snd / block_size;
        block_nr += (bytes_snd % block_size > 0) ? 1 : 0;
        uint32_t lines_per_block = ysize_snd / block_nr;
        printf("Frame divided into %d blocks of %d lines and one last of %d lines\n", block_nr, lines_per_block, ysize_snd % lines_per_block);
        if (bytes_snd > block_size)
        {
            lines_per_block = ysize_snd / block_nr;
        }
        data_in_shared_mem->current_work.bytes_per_pixel = bytes_per_pixel;
        data_in_shared_mem->current_work.x_size = xsize_snd;
        data_in_shared_mem->current_work.y_size = ysize_snd;

        // Equal size blocks
        uint32_t initial_y = 0;
        uint32_t lines_in_block = 0;
        for (uint32_t block = 0; block < block_nr; block++)
        {
            initial_y = lines_per_block * block;
            lines_in_block = (initial_y + lines_per_block < ysize_snd) ? lines_per_block : ysize_snd - initial_y;
            data_in_shared_mem->current_work.initial_y = initial_y;
            data_in_shared_mem->current_work.lines_in_block = lines_in_block;
            printf("Working on block %d. Initial y is %d and lines are %d\n", block, data_in_shared_mem->current_work.initial_y, data_in_shared_mem->current_work.lines_in_block);
            // Block + upper buffer
            for (uint32_t index = 0; index < (lines_in_block * xsize_snd + convolution_buffer) * bytes_per_pixel && index + initial_y * xsize_snd * bytes_per_pixel < bytes_snd; index++)
            {
                data_in_shared_mem->pixel_space[index + convolution_buffer] = frame_snd[index + (initial_y * xsize_snd * bytes_per_pixel)];
            }

            // for (uint32_t index = 0; index < lines_in_block * xsize_snd * bytes_per_pixel; index++)
            // {
            //     data_in_shared_mem->pixel_space[index + convolution_buffer] = frame_snd[index + (initial_y * xsize_snd * bytes_per_pixel)];
            // }

            data_in_shared_mem->current_work.busy = 1;

            while (data_in_shared_mem->current_work.busy != 0)
            {
            }
            for (uint32_t index = 0; index < lines_in_block * xsize_snd; index++)
            {
                frame_buffer[index + initial_y * xsize_snd] = data_in_shared_mem->pixel_space[index + convolution_buffer];
            }
            printf("Inserted from %d to %d\n", initial_y * xsize_snd, initial_y * xsize_snd + lines_in_block * xsize_snd);
        }

        printf("File delivered!\n");

        char outfile[200];
        const char *prefix = "out-";
        strcpy(outfile, prefix);
        strcpy(&outfile[strlen(prefix)], argv[f]);
        outfile[strlen(argv[f]) + strlen(prefix)] = '\0';

        if (writeBMP(outfile, frame_buffer, xsize_snd, ysize_snd, 8))
        {
            printf("Cannot write file %s\n", outfile);
        }
        free(frame_buffer);
    }

    // close the shared memory handle before exiting
    arm_shared_memory_close();
    return 0;
}