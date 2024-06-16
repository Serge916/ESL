#!/bin/bash
# run the images in order of increasing size
# you can omit the larger ones in your initial testing
rm out-test-images/*.bmp

time ./arm_img_proc \
	test-images/cat-160x185x24.bmp \
	test-images/surfer-32x24x24.bmp \
	test-images/triangle-100x100x08.bmp \
	test-images/marbles-130x92x24.bmp \
	test-images/mary-100x213x08.bmp \
	test-images/cat-400x462x24.bmp \
	test-images/alps-300x225x8.bmp \
