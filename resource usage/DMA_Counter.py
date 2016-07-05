 #!/usr/bin/python
 opnm = ['to_core1', 'to_core2', 'to_core3', 'to_core4', 'to_DDR', 'from_core1', 'from_core2', 'from_core3', 'from_core4', 'from_DDR']
 dma_num = [0] * 40
 j = [0] * 4
 i = [0] * 4
 q = 0
 mb = ['MB1','MB2','MB3','MB4']
 timestamp = 'timestamp:'
 fp = open("response.txt",'r+')
 line = fp.readline()
 while line.strip('\n\r')!="Tiles synced.":
       line = fp.readline()
 while line.strip('\n\r') != "Program finished":
       line = fp.readline()
       line_list = line.strip('\n\r').split()
       if line_list[0] in mb and line_list[-1]=="(FF444D41)":
               j[int(line_list[0][2])-1]=0
       if line_list[0] in mb and j[int(line_list[0][2])-1] == 1:
               dma_num[(int(line_list[0][2])-1)*10+i[int(line_list[0][2])-1]] = int(line_list[-2])
               i[int(line_list[0][2])-1] = i[int(line_list[0][2])-1] + 1	
       if line_list[0] in mb and line_list[-1]=="(00444D41)":
               j[int(line_list[0][2])-1]=1
 while q < 40:
       b = q//10+1
       print "For core %d, %s is: %d"% (b, opnm[q%10], dma_num[q])
       q = q+1