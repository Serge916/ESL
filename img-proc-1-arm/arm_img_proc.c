#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "image_processing_functions.h"

#define BITS_PER_PIXEL 8

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s infile ...\n", argv[0]);
    return 1;
  }

  int nr_files;
  nr_files = argc - 1;

  for (int f = 1; f <= nr_files; f++)
  {
    uint32_t xsize, ysize, bitsperpixelIn;

    // read BMP
    printf("Processing file %s\n", argv[f]);
    uint8_t *frameIn = NULL;
    if (!readBMP(argv[f], &frameIn, &xsize, &ysize, &bitsperpixelIn))
    {
      printf("Cannot read file %s\n", argv[f]);
      return 1;
    }

    uint32_t bytesOut = xsize * ysize;
    uint8_t *frameA;
    uint8_t *frameB;

    // convert to greyscale
    uint32_t const orig_bits = bitsperpixelIn;
    frameA = (uint8_t *)malloc(bytesOut);
    frameB = (uint8_t *)malloc(bytesOut);
    greyscale(frameIn, xsize, ysize, bitsperpixelIn, frameA);

    // convolution (average or gaussian blur)
    double const *filter;
    uint32_t filter_size;
    if (orig_bits == 8)
    {
      filter = conv_avgxy3;
      filter_size = 3;
    }
    else
    {
      filter = conv_gaussianblur5;
      filter_size = 5;
    }
    convolution(frameA, xsize, ysize, BITS_PER_PIXEL, filter, filter_size, filter_size, frameB);

    // sobel
    uint8_t const threshold = (orig_bits == 8 ? 100 : 128);
    // sobel(frameB, xsize, ysize, BITS_PER_PIXEL, threshold, frameA);

    // overlay
    // overlay(frameB, xsize, ysize, BITS_PER_PIXEL, frameA, xsize, ysize, BITS_PER_PIXEL, 0, 0, 0.7, frameA);

    // write BMP
    // note that reading & then writing doesn't always result in the same image
    // - a grey-scale (8-bit pixel) image will be written as a 24-bit pixel image too
    // - the header of colour images may change too
    char outfile[200];
    const char *prefix = "own-";
    strcpy(outfile, prefix);
    strcpy(&outfile[strlen(prefix)], argv[f]);
    outfile[strlen(argv[f]) + strlen(prefix)] = '\0';
    (void)writeBMP(outfile, frameB, xsize, ysize, BITS_PER_PIXEL);
  }
  return 0;
}
