 #!/usr/bin/python
 core_num = 0
 mb = ['MB1','MB2','MB3','MB4']
 timestamp = 'timestamp:'
 fp = open("response.txt",'r+')
 line = fp.readline()
 while line.strip('\n\r')!="Tiles synced.":
     line = fp.readline()
 while line.strip('\n\r') != 'Program finished':
     line = fp.readline()
     line_list = line.strip('\n\r').split()
     if line_list[0] in mb and line_list[1] == timestamp:
         if (int(line_list[-2]) < 0 ):
             core_num+=1
 print "number of cores detected is:",core_num