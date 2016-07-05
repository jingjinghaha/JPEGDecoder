OPTFLAGS=-O3 -Wall


# download image, or not
DO_IMAGE_OUTPUT=1
IMAGE_OUTPUT_WIDTH=1024
IMAGE_OUTPUT_HEIGHT=768

# code for each tile.
MB1_DIR=mb1
MB2_DIR=mb2
MB3_DIR=mb3
MB4_DIR=mb4

# shared code and header files.
MB_SHARED=mb_shared

# Enable DMA mode (instead of MMIO)
USE_DMA=1
ifeq (${USE_DMA}, 1)
   OPTFLAGS+=-DUSE_DMA=1
endif

# Upload files before the run
 DATA_FILES= 9.jpg ddr_marker.txt
# DATA_FILES=7.jpg

# force use of FPGA board connected to VGA framegrabber
#USE_VGA_GRABBER?=1

# force image to be downloaded in binary form (not Huffman encoded)
#FORCE_BINARY?=1

# specify timeout
USER_TIMEOUT=1000

# Check for testing mode
ifeq ($(TESTING), 1)
        DATA_FILES=$(INPUT_IMAGE)
        IMAGE_OUTPUT_HEIGHT=$(IMAGE_HEIGHT)
        IMAGE_OUTPUT_WIDTH=$(IMAGE_WIDTH)
else
        DATA_FILES=7.jpg
        IMAGE_OUTPUT_HEIGHT=1024
        IMAGE_OUTPUT_WIDTH=768
endif

include .platform/platform.mk
