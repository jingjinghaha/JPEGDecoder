 #!/bin/bash    
   #	    
   LOCAL_STATIC=$("/home/emb15/bin/memory.sh" | tail -n 3);    
   
   # Printing the heap memory of MB1    
   MB1_RESP0=$(egrep ^MB1 --binary-files=text response.txt | awk '/AAAAAAAA/{getline; print $5}')    
   # MB1_RESP1=$(egrep ^MB1 --binary-files=text response.txt | awk '/A5A5A5A5/{getline; print $5}')    
   MB1_RESP2=$(egrep ^MB1 --binary-files=text response.txt | awk '/A6A6A6A6/{getline; print $5}')    
   MB1_RESP3=$(egrep ^MB1 --binary-files=text response.txt | awk '/A7A7A7A7/{getline; print $5}')    
   MB1_RESP4=$(egrep ^MB1 --binary-files=text response.txt | awk '/A8A8A8A8/{getline; print $5}')    
   MB1_RESP5=$(egrep ^MB1 --binary-files=text response.txt | awk '/A9A9A9A9/{getline; print $5}')    
   MB1_RESP6=$(egrep ^MB1 --binary-files=text response.txt | awk '/12345678/{getline; print $5}')
   
   # Printing the heap memory of MB2    
   MB2_RESP0=$(egrep ^MB2 --binary-files=text response.txt | awk '/AAAAAAAA/{getline; print $5}')    
   # MB2_RESP1=$(egrep ^MB2 --binary-files=text response.txt | awk '/A5A5A5A5/{getline; print $5}')    
   MB2_RESP2=$(egrep ^MB2 --binary-files=text response.txt | awk '/A6A6A6A6/{getline; print $5}')    
   MB2_RESP3=$(egrep ^MB2 --binary-files=text response.txt | awk '/A7A7A7A7/{getline; print $5}')    
   MB2_RESP4=$(egrep ^MB2 --binary-files=text response.txt | awk '/A8A8A8A8/{getline; print $5}')    
   MB2_RESP5=$(egrep ^MB2 --binary-files=text response.txt | awk '/A9A9A9A9/{getline; print $5}')
   MB2_RESP6=$(egrep ^MB2 --binary-files=text response.txt | awk '/12345678/{getline; print $5}')    
   
   # Printing the heap memory of MB3    
   MB3_RESP0=$(egrep ^MB3 --binary-files=text response.txt | awk '/AAAAAAAA/{getline; print $5}')    
   # MB3_RESP1=$(egrep ^MB3 --binary-files=text response.txt | awk '/A5A5A5A5/{getline; print $5}')    
   MB3_RESP2=$(egrep ^MB3 --binary-files=text response.txt | awk '/A6A6A6A6/{getline; print $5}')    
   MB3_RESP3=$(egrep ^MB3 --binary-files=text response.txt | awk '/A7A7A7A7/{getline; print $5}')    
   MB3_RESP4=$(egrep ^MB3 --binary-files=text response.txt | awk '/A8A8A8A8/{getline; print $5}')    
   MB3_RESP5=$(egrep ^MB3 --binary-files=text response.txt | awk '/A9A9A9A9/{getline; print $5}')  
   MB3_RESP6=$(egrep ^MB3 --binary-files=text response.txt | awk '/12345678/{getline; print $5}')  
   
   # Printing the heap memory of MB4    
   MB4_RESP0=$(egrep ^MB4 --binary-files=text response.txt | awk '/AAAAAAAA/{getline; print $5}')    
   # MB4_RESP1=$(egrep ^MB4 --binary-files=text response.txt | awk '/A5A5A5A5/{getline; print $5}')    
   MB4_RESP2=$(egrep ^MB4 --binary-files=text response.txt | awk '/A6A6A6A6/{getline; print $5}')    
   MB4_RESP3=$(egrep ^MB4 --binary-files=text response.txt | awk '/A7A7A7A7/{getline; print $5}')    
   MB4_RESP4=$(egrep ^MB4 --binary-files=text response.txt | awk '/A8A8A8A8/{getline; print $5}')    
   MB4_RESP5=$(egrep ^MB4 --binary-files=text response.txt | awk '/A9A9A9A9/{getline; print $5}')   
   MB4_RESP6=$(egrep ^MB4 --binary-files=text response.txt | awk '/12345678/{getline; print $5}') 
   
   echo "Local static memory Footprint (data memory and instruction memory):\n$LOCAL_STATIC \n"    
   
   # echo "DYNAMIC MEMORY USAGE BY EACH CORE:"    
   # echo "CORE1 : $MB1_RESP1 bytes"    
   # echo "CORE2 : $MB2_RESP1 bytes"    
   # echo "CORE3 : $MB3_RESP1 bytes"    
   # echo "CORE4 : $MB4_RESP1 bytes\n"    
   
   echo "MAXIMUM HEAP STORAGE BY EACH CORE:"    
   echo "CORE1 : $MB1_RESP0 bytes"    
   echo "CORE2 : $MB2_RESP0 bytes"    
   echo "CORE3 : $MB3_RESP0 bytes"    
   echo "CORE4 : $MB4_RESP0 bytes\n"    
   
   echo "NUMBER OF DYNAMIC MEMORY ALLOCATIONS BY EACH CORE:"    
   echo "CORE1 : $MB1_RESP2 times"    
   echo "CORE2 : $MB2_RESP2 times"    
   echo "CORE3 : $MB3_RESP2 times"    
   echo "CORE4 : $MB4_RESP2 times\n"    
   
   echo "NUMBER OF FREED HEAP MEMORY BY EACH CORE:"    
   echo "CORE1 : $MB1_RESP3 times"    
   echo "CORE2 : $MB2_RESP3 times"    
   echo "CORE3 : $MB3_RESP3 times"    
   echo "CORE4 : $MB4_RESP3 times\n"    
   
   echo "MAXIMUM Communication Memory Usage:"    
   echo "CORE1 : $MB1_RESP4 bytes"    
   echo "CORE2 : $MB2_RESP4 bytes"    
   echo "CORE3 : $MB3_RESP4 bytes"    
   echo "CORE4 : $MB4_RESP4 bytes\n"  
   
   echo "Stack Memory Usage:"
   echo "CORE1 : $MB1_RESP6 bytes"
   echo "CORE2 : $MB2_RESP6 bytes"
   echo "CORE3 : $MB3_RESP6 bytes"
   echo "CORE4 : $MB4_RESP6 bytes\n"  
   
   echo "MAXIMUM DDR Memory Usage:"    
   if [ $MB1_RESP5 ]    
       then    
   echo "$MB1_RESP5 bytes"    
   fi    
   if [ $MB2_RESP5 ]	    
       then    
   echo "$MB2_RESP5 bytes"    
   fi    
   if [ $MB3_RESP5 ]	    
       then    
   echo "$MB3_RESP5 bytes"    
   fi    
   if [ $MB4_RESP5 ]	    
       then    
   echo "$MB4_RESP5 bytes\n"    
   fi    
   
   echo "----------------------END OF SCRIPT--------------------"