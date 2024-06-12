#ifndef __IMAGE_PROCESSING_FUNCTIONS_H__
#define __IMAGE_PROCESSING_FUNCTIONS_H__

#include <stdlib.h>

extern void median_filter(uint8_t const volatile *const frame_in, uint32_t const xsize_in, uint32_t const ysize_in, uint32_t const bitsperpixel_in, uint8_t volatile *const frame_out);
extern double const conv_sharpen3[];
extern double const conv_avgxy1[];
extern double const conv_avgxy3[];
extern double const conv_avgxy5[];
extern double const conv_avgx3[];
extern double const conv_avgxy7[];
extern double const conv_gaussianblur5[];
void convolution(uint8_t const volatile *const frame_in, uint32_t const volatile xsize_in, uint32_t const volatile ysize_in, uint32_t const volatile lines_per_thread, uint32_t const volatile bytes_per_pixel,
                 double const *const f, uint32_t const fxsize, uint32_t const fysize, uint8_t volatile *const frame_out, uint32_t const frame_half, uint32_t const initial_y);
extern void scale(uint8_t const volatile *const frame_in, uint32_t const xsize_in, uint32_t const vysize_in, uint32_t const bitsperpixel_in,
                  uint32_t const xsize_out, uint32_t const ysize_out, uint8_t volatile *const frame_out);
// frame_out may be the same as frame_in
extern void greyscale(uint8_t const volatile *const frame_in, uint32_t const bytes_per_pixel, uint32_t const pixels_in_block, uint8_t volatile *const frame_out);
// frame_out may be the same as frame_in1 or frame_in2
extern void overlay(uint8_t const volatile *const frame_in1, uint8_t const volatile *const frame_in2, uint32_t volatile pixels_in_block, uint8_t volatile *const frame_out);
extern void sobel(uint8_t const volatile *const frame_in, uint32_t const xsize_in, uint32_t const ysize_in, uint32_t const block_ysize, uint32_t const bytes_per_pixel,
                  uint8_t const threshold, uint8_t volatile *const frame_out, uint32_t const frame_half, uint32_t const initial_y);
extern void offset(uint8_t const volatile *const original_address, uint8_t volatile *const final_address, uint32_t const total_pixels, uint32_t const bitsperpixel_in);

#endif
