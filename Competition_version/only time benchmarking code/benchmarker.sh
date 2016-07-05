#!/bin/bash

PATH=$PATH:/opt/Xilinx/13.3/ISE_DS/EDK/gnu/microblaze/lin64/bin/:/home/emb15/bin/

# CHANGE THIS TO YOUR GROUP NUMBER!!!!!
# |||||||||
# VVVVVVVVV
GROUP=12
# ^^^^^^^^^
# |||||||||
# CHANGE THIS TO YOUR GROUP NUMBER!!!!!

for jpeg in ./test_images/*.jpg
do
  file_name=${jpeg%%.jpg}
  short_file_name=${file_name##*/}
  jpeg_name=$file_name."jpg"
  bmp_name=$file_name."bmp"

  make clean >& /dev/null
  rm binary.bmp
  rm binary.pnm

  width=`identify -format '%w' $jpeg_name`
  height=`identify -format '%h' $jpeg_name`

  TESTING=1 INPUT_IMAGE=$jpeg_name IMAGE_HEIGHT=$height IMAGE_WIDTH=$width make >& /dev/null

  TESTING=1 INPUT_IMAGE=$jpeg_name IMAGE_HEIGHT=$height IMAGE_WIDTH=$width make run

  ppmtobmp -windows -bpp 24  binary.pnm > binary.bmp

  diff -s -q ./binary.bmp $bmp_name

  if [ $? -eq 0 ]
  then
    FUNCTIONAL=1
  else
    FUNCTIONAL=0
  fi

  python output_parser.py response.txt -G $GROUP -V $FUNCTIONAL -I $short_file_name -C 

done
