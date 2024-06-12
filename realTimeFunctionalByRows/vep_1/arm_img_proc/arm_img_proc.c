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

#define MAX_IMG_SIZE 185000

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
  uint32_t nr_files = argc;
  data_in_shared_mem = arm_shared_memory_init();
  fifo_t volatile *const admin_in = &data_in_shared_mem->admin_in;
  fifo_t volatile *const admin_out = &data_in_shared_mem->admin_out;
  line_t volatile *const lines_in = data_in_shared_mem->lines_in;
  line_t volatile *const lines_out = data_in_shared_mem->lines_out;

  // wait until the RISC-V has initialised both fifos
  printf("Initializing FIFOs\n");
  while (fifo_initialized(admin_in) == false || fifo_initialized(admin_out) == false)
  {
  }
  printf("FIFOs ready!\n");

  for (uint32_t f = 1; f <= nr_files; f++)
  {
    // read BMP
    uint32_t xsize_snd, ysize_snd, bitsperpixel_snd;
    uint8_t *frame_snd = NULL;
    uint32_t image_finished = 0;
    uint32_t inserted_lines = 0;
    uint32_t processed_lines = 0;

    // read file
    if (!readBMP(argv[f], &frame_snd, &xsize_snd, &ysize_snd, &bitsperpixel_snd))
    {
      printf("Cannot read file %s\n", argv[f]);
      arm_shared_memory_close();
      return 1;
    }
    uint32_t const bytes_snd = xsize_snd * ysize_snd * (bitsperpixel_snd / 8);
    uint32_t const bytes_per_line = xsize_snd * bitsperpixel_snd / 8;
    if (xsize_snd > MAX_LINE_SIZE)
    {
      printf("Image (%s) doesn't fit in allocated memory\n", argv[f]);
      continue;
    }
    // Allocate space for frame out. In stack to avoid malloc.
    uint8_t frame_out[MAX_IMG_SIZE] = {0};
    printf("Processing file %s\n", argv[f]);

    uint32_t line_dimension = bitsperpixel_snd / 8;

    while (image_finished == 0)
    {
      uint32_t available_slots_in = fifo_spaces(admin_in);
      if (available_slots_in > line_dimension && inserted_lines < ysize_snd)
      {
        // printf("Inserted lines: %d", inserted_lines);
        for (uint32_t i = 0; i < line_dimension; i++)
        {
          uint32_t space_index = fifo_claim_space(admin_in);
          line_t volatile *line_in = &lines_in[space_index];
          line_in->length = xsize_snd;
          line_in->y_position = inserted_lines;
          line_in->y_size = ysize_snd;
          line_in->isRGB = bitsperpixel_snd > 8 ? 1 : 0;
          for (uint32_t j = 0; j < xsize_snd; j++)
          {
            line_in->pixel_space[j] = frame_snd[inserted_lines * bytes_per_line + (j + i) * line_dimension];
          }
          fifo_release_token(admin_in);
        }
        inserted_lines++;
      }

      if (fifo_tokens(admin_out))
      {
        uint32_t volatile token_index = fifo_claim_token(admin_out);
        line_t volatile const *line_out = &lines_out[token_index];
        uint32_t volatile y_position = line_out->y_position;
        uint32_t volatile length = line_out->length;
        uint32_t volatile y_size = line_out->y_size;
        for (uint32_t j = 0; j < length; j++)
        {
          frame_out[y_position * length + j] = line_out->pixel_space[j];
        }
        fifo_release_space(admin_out);
        printf("Line out: %d/%d\n", y_position, y_size);
        printf("Pixel value: 0x%x\n", frame_out[20]);
        processed_lines++;
        // Conditional in this scope to avoid True without having taken any line out
        if (y_position >= ysize_snd - 1)
        {
          image_finished = 1;
        }
      }

      usleep(20);
    }

    printf("Finished after inserting %d and processing %d lines\n", inserted_lines, processed_lines);
    char outfile[200];
    const char *prefix = "out-";
    strcpy(outfile, prefix);
    strcpy(&outfile[strlen(prefix)], argv[f]);
    outfile[strlen(argv[f]) + strlen(prefix)] = '\0';
    if (writeBMP(outfile, frame_out, xsize_snd, ysize_snd, 8))
    {
      printf("Cannot write file %s\n", outfile);
      arm_shared_memory_close();
      return 1;
    }
  }
  arm_shared_memory_close();
  return 0;
}