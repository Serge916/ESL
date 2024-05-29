#!/bin/bash
# run the images in order of increasing size
# you can omit the larger ones in your initial testing
make

time ./arm_img_proc \
	test-images/surfer-32x24x24.bmp \
	# test-images/pixel-1x1x24.bmp \
# 	test-images/lena-130x68x08.bmp \
# 	test-images/triangle-100x100x08.bmp \
# 	test-images/lines-120x210x08.bmp \
# 	test-images/marbles-130x92x08.bmp \
# 	test-images/noise-128x96x08.bmp \
# 	test-images/dots-110x110x24.bmp \
# 	test-images/ray-140x105x08.bmp \
# 	test-images/flag-124x124x08.bmp \
# 	test-images/headset-128x128x08.bmp \
# 	test-images/bitmap-120x156x08.bmp \

# diff out-test-images/surfer-32x24x24.bmp own-test-images/surfer-32x24x24.bmp 
