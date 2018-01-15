/**********************************************************************************************
 *     This example shows how to use some of Jeda system functions, get_plusarg()
 *     system_exec()
 *     
 *     Author: EZ
 *     Copyright (c) 2003 Jeda Technologies, Inc
 **********************************************************************************************/

/*
 * In the following example, random(seed) is called, seed is passed from the plus arguments
 * at run time, or as a default, the current timestamp of hour, minute and second.
 */
void init()  {

  bit [31:0] ntransactions;
  int seed ; 
  string parm ;
  int time; 

  printf("=====  calling init ====\n");
	
	// SEED STUFF
   	parm = get_plusarg( "seed_" ) ; 

   	if( parm != null ) { 
   	  	seed = atoi( parm ) ; 
       	random( seed ) ; 
       	printf( "randmt is initialized with seed %d\n", seed ) ; 
		printf("%0d\n", random());
	} 
	else
	{

		time += system_exec("date +%H%M%S");  

		printf(" time read as %h\n", time);
		seed = time;
		printf( "rand is initialized with seed %d\n", seed ) ; 
		random(seed);
		printf("%0d\n", random());
	}	
}


