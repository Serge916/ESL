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
extern void convolution(uint8_t const volatile *const lines_in, uint32_t const volatile length, uint32_t const volatile y_position, uint32_t const volatile y_size,
                        double const *const f, uint32_t const fxsize, uint32_t const fysize, uint8_t volatile *const line_out);
extern void scale(uint8_t const volatile *const frame_in, uint32_t const xsize_in, uint32_t const vysize_in, uint32_t const bitsperpixel_in,
                  uint32_t const xsize_out, uint32_t const ysize_out, uint8_t volatile *const frame_out);
// frame_out may be the same as frame_in
extern void greyscale(uint8_t const volatile *const bytes_in, uint32_t const length, uint32_t const bitsperpixel_in,
                      uint8_t volatile *const bytes_out);
// frame_out may be the same as frame_in1 or frame_in2
extern void overlay(uint8_t const volatile *const line_in_1, uint8_t const volatile *const line_in_2, uint32_t volatile length, double const ratio, uint8_t volatile *const line_out);
extern void sobel(uint8_t const volatile *const lines_in, uint32_t const length, uint32_t const y_position, uint32_t const y_size,
                  uint8_t const threshold, uint8_t volatile *const line_out);
extern void offset(uint8_t const volatile *const original_address, uint8_t volatile *const final_address, uint32_t const total_pixels, uint32_t const bitsperpixel_in);

#endif
