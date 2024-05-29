#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <arm_shared_memory.h>
/* warning: this is the RISC-V memory map!
 * use only in conjuction with arm_shared_memory_write/read
 */
#include "vep_memory_map.h"

#define FRAME_SLOTS 2
#define INPUT_IMAGE_CAPACITY 40

// two useful functions
extern uint32_t readBMP(char const *const file, uint8_t **outframe, uint32_t *const x_size, uint32_t *const y_size, uint32_t *const bitsperpixel);
extern uint32_t writeBMP(char const *const file, uint8_t const *const outframe, uint32_t const x_size, uint32_t const y_size, uint32_t const bitsperpixel);

vep_memshared0_shared_region_t volatile *data_in_shared_mem = NULL;
uint32_t available_space = ALLOCATED_BYTES_MEMS;
char **remaining_files;
uint32_t occupied_slots[2] = {0};
uint32_t index_in_slot[2] = {0};
uint32_t nr_files;

void insert_frame(uint32_t const slot_number, uint32_t const index)
{
  // read BMP
  uint32_t xsize_snd, ysize_snd, bitsperpixel_snd;
  uint8_t *frame_snd = NULL;

  // read file
  printf("ONE LOOP. FIles: %s\n", remaining_files[index]);
  if (!readBMP(remaining_files[index], &frame_snd, &xsize_snd, &ysize_snd, &bitsperpixel_snd))
  {
    printf("Cannot read file %s\n", remaining_files[index]);
    nr_files = 0;
    return;
  }
  printf("Processing file %s\n", remaining_files[index]);
  uint32_t const bytes_snd = xsize_snd * ysize_snd * (bitsperpixel_snd / 8);
  // check if it fits in the memory on the RISC-V side
  // send image to RISC-V
  // wait for the RISC-V to produce the output data

  if (bytes_snd > ALLOCATED_BYTES_MEMS)
  {
    printf("Image doesn't fit in the space\n");
    remaining_files[index] = NULL;
    return;
  }
  if (bytes_snd > available_space)
  {
    printf("Image doesn't fit at the moment\n");
    return;
  }
  occupied_slots[slot_number] = 1;
  index_in_slot[slot_number] = index;

  data_in_shared_mem->current_work[slot_number].x_size = xsize_snd;
  data_in_shared_mem->current_work[slot_number].y_size = ysize_snd;
  data_in_shared_mem->current_work[slot_number].bits_per_pixel = bitsperpixel_snd;

  // If frame 1, place on top. If frame 2, place at the end
  if (slot_number == 0)
  {
    for (size_t i = 0; i < bytes_snd; i++)
    {
      data_in_shared_mem->pixel_space[i] = frame_snd[i];
    }
  }
  else
  {
    for (size_t i = ALLOCATED_BYTES_MEMS - bytes_snd; i < ALLOCATED_BYTES_MEMS; i++)
    {
      data_in_shared_mem->pixel_space[i] = frame_snd[i];
    }
  }

  data_in_shared_mem->current_work[slot_number].busy = 1;

  printf("Waiting for the job to be delivered\n");
}

void check_status(uint32_t const slot_number)
{
  switch (data_in_shared_mem->current_work[slot_number].busy)
  {
  case 1:
    // Still busy
    break;
  case 2:
    // Shrink space
    if (data_in_shared_mem->current_work[slot_number].bits_per_pixel == 24)
    {
      available_space += 2 * data_in_shared_mem->current_work[slot_number].x_size * data_in_shared_mem->current_work[slot_number].y_size;
    }
    break;
  case 0:
    // Finished
    printf("Job delivered!\n");
    char outfile[200];
    const char *prefix = "out-";
    strcpy(outfile, prefix);
    strcpy(&outfile[strlen(prefix)], remaining_files[index_in_slot[slot_number]]);
    outfile[strlen(remaining_files[index_in_slot[slot_number]]) + strlen(prefix)] = '\0';
    if (writeBMP(outfile, data_in_shared_mem->pixel_space, data_in_shared_mem->current_work[slot_number].x_size, data_in_shared_mem->current_work[slot_number].y_size, 8))
    {
      printf("Cannot write file %s\n", outfile);
      nr_files = 0;
      return;
    }
    nr_files--;
    break;
  default:
    printf("Unkown value in busy flag: %d\n", data_in_shared_mem->current_work[slot_number].busy);
  }
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s infile ...\n", argv[0]);
    return 1;
  }

  nr_files = argc - 1;

  // map the region in shared memory memshared0 to a memory region in the memory of the ARM
  // cast the resulting pointer to the same data structure vep_memshared0_t that the RISC-V uses
  // Linux ensures that the original data in memshared0 and the copy in the ARM caches remain consistent
  vep_memshared0_shared_region_t *data_in_shared_mem = arm_shared_memory_init();
  if (data_in_shared_mem == NULL)
  {
    fprintf(stderr, "Something went wrong when initializing the shared memory pointer\n");
  }

  remaining_files = argv[1];

  // Fill unoccupied slots in the array
  // for (uint32_t i = nr_files; i < INPUT_IMAGE_CAPACITY; i++)
  // {
  //   remaining_files[i] = 1;
  // }
  printf("Commanded %d works!\n", nr_files);
  while (nr_files > 0)
  {
    for (uint32_t slot_number = 0; slot_number < FRAME_SLOTS; slot_number++)
    {
      switch (occupied_slots[slot_number])
      {
      case 0:
        // Check for remaining files
        for (uint32_t index = 0; index < nr_files; index++)
        {
          if (remaining_files[index] != NULL)
          {
            insert_frame(slot_number, index);
            break;
          }
        }
        break;
      case 1:
        check_status(slot_number);
      default:
        fprintf(stderr, "Unkown value in occupied_slots: %d\n", occupied_slots[slot_number]);
        break;
      }
    }
  }

  // close the shared memory handle before exiting
  arm_shared_memory_close();
  return 0;
}