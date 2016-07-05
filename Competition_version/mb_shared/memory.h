#ifndef MEMORY_H
   #define  MEMORY_H
   
   #define BENCHMARK_EN		1
   
   #define MALLOC_MAX_INFO		0xAAAAAAAA
   #define MALLOC_USE_INFO	0xA5A5A5A5
   #define MALLOC_NUM_MALLOCS_INFO		0xA6A6A6A6
   #define MALLOC_NUM_FREES_INFO	0xA7A7A7A7A7
   #define CMEM_USAGE 0xA8A8A8A8
   #define MASK_VALUE		0xAAAAAAAA
   #define DDR_USAGE_INFO		0xA9A9A9A9
   #define STACK_PAINT_VALUE	0x55555555
   #define STACK_USAGE_INFO		0x12345678
   
   /* Print out max heap usage
   */
   void benchmark_malloc_max_usage();
   
   /* Print out current heap usage
   */
   void benchmark_malloc_usage();
   
   /* Print out number of malloc calls
   */
   void benchmark_malloc_num_mallocs();
   
   /* Print out number of malloc free calls
   */
   void benchmark_malloc_num_malloc_free();
   
    /*                                                                                               
    function that returns the maximum heap usage.                                                    
                                                                                                         
    @returns the maximum heap used in words.                                                         
    */                                                                                                 
    int mk_malloc_max_usage();                                                                          
                                                                                                          
    /*                                                                                          
    function that returns the current heap usage.                                                    
                                                                                                     
    Note that, even though heap_size - usage is big enough                                           
    an allocation can still fail if no consecutive block is available                                
    in memory.                                                                                       
                                                                                                     
    @returns the current heap used in words.                                                         
    */                                                                                                 
    int mk_malloc_usage();                                                                              
                                                                                                      
    /*                                                                                           
    function that returns the number of mallocs                                                      
                                                                                                     
    @returns the number of mallocs                                                                   
    */                                                                                                 
    int mk_malloc_num_mallocs();                                                                        
                                                                                                      
    /*                                                                                           
    function that returns the number of frees                                                        
                                                                                                     
    @returns the number of frees                                                                     
    */                                                                                                 
    int mk_malloc_num_frees();
   
   /*
   Set the communication memory with initial value 0xAA
   */
   void benchmark_cmem_pattern();
   
   /*
   measure the communication memory usage by detecting 
   the byte value which is not 0x88
   */
   void benchmark_cmem_usage();
   
   /*
       Mask DDR memory with 0xAAAAAAAA pattern
   */
   void mask_DDR();
   /*
       Read back DDR memory and compare values with 0xAAAAAAAA pattern
   */
   void readback_DDR();
   /*
       Print out how many bytes has been used in DDR 
   */
   void print_DDR_usage();
   
   /*
       Paints the value 0x55555555 in the existing unused places of the Stack memory.
   */
   void paint_stack();
   /*
       Print out how many bytes has been used in Stack memory.
   */
   void count_stack();
   
   #endif