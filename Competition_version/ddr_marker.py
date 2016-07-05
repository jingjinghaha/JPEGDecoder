f = open("ddr_marker.txt", "w+")
for i in range(1, 0x200001, 1):
   f.write(chr(0xAA))
f.close()