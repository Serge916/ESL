#!/bin/bash
# run the images in order of increasing size
# you can omit the larger ones in your initial testing
time ./arm_img_proc \
	test-images/triangle-100x100x08.bmp \
	test-images/surfer-32x24x24.bmp \
	test-images/marbles-130x92x24.bmp º	
