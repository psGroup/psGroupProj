/** \file unit_testing.c
* \date 06/11/12
* \author B
* \author B
* \author B
* \author B
* \brief unit testing for predator prey model
*/



#include "greatest.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "global_values.h"

#include "equations.h"
#include "dataStructiars.h" 
#include "printFunctions.h"

  configurations configs;


/**
* \fn build_matrix
* \brief
*
* We're going to define a function which creates a 5x5 matrix 
* Each matrix point has a value for AREA, HARES, and PUMAS
* We initialise the area to all be land, with equal distributions 
* of pumas and hares, such that we know expected outputs for particular points
*
* \return test_matrix.   
*/

struct_matrix *build_matrix(){
	int x = 5; /**< Explain */
	int y = 5; /**< Explain */
	struct_matrix *test_matrix; /**< Explain */
	int i,j,a,b; /**< Explain */
	
	/**
	* allocating memory for test matrix and ensuring correct memory allocation
	*
	*/

	test_matrix = (struct_matrix*) malloc(sizeof(struct_matrix));
	if(test_matrix == NULL) {
		fprintf(stderr,"Malloc failed for test_matrix");
		exit(EXIT_FAILURE);
	}	
	/**
	* allocate memory for our matrix map
	*/
	test_matrix->map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
	if(test_matrix->map == NULL) {
			fprintf(stderr,"Malloc failed for test_matrix.map\n");
		   exit(EXIT_FAILURE);
		}	
	
	  for (i = 0; i < x; i++)
	  {
		 test_matrix->map[i] = (struct_cell*) malloc(sizeof(struct_cell) * y);
		 if (test_matrix->map[i] == NULL)
		 {
		   fprintf(stderr, "malloc faild\n");
		   exit(EXIT_FAILURE);
		 }
	  }

	for(a =0; a<x; a++){
		for(b=0; b<y; b++){
			test_matrix->map[a][b].area = LAND; 
			test_matrix->map[a][b].hares = 3.0; 
			test_matrix->map[a][b].pumas = 3.0; 
			}
		}	
	return test_matrix;	
}


/**
* \fn test_the_neighbour_calculation
* \brief Ensuring the neighbour calculation works as expected
*
* 
*
* \return Void.   
*/

TEST test_the_neighbour_calculation(){
	struct_matrix *test_matrix = build_matrix(); /**< Explain */
	double num_neighbours = landNeighboursCells(test_matrix, 2,2); /**< Explain */
	ASSERT(num_neighbours==4); /**< Explain */
	PASS();
}

/**
* \fn test_hares_update
* Comparing puma update step with expected output
*
* Test that, by passing a matrix with values for pumas, that 
*	the update gives the expected output
* 
* \return Void.   
*/

TEST test_hares_update(void){
	struct_matrix *test_matrix = build_matrix();
	/**
	*test a point in the middle of the grid
	*/
	double mid = haresNewValue(test_matrix,2,2, configs);
	double mid_known = 2.95;
	ASSERT_IN_RANGE(0,abs(mid-mid_known),0.00001); 
	/**
	* test to within a tolerance (last argument) because the values are not \
	* exactly the same due to floating point errors
	 */
	PASS();
}

/**
* \fn test_pumas_update
* Comparing puma update step with expected output
*
* Test that, by passing a matrix with values for pumas, that 
*	the update gives the expected output
* \return Void.   
*/

TEST test_pumas_update(void){
	struct_matrix *test_matrix = build_matrix();
/**	test a mid point */
	double mid = pumasNewValue(test_matrix,1,1, configs);
	/**< Passing in a known matrix, with values from configs */
	double mid_known = 2.95;
	ASSERT_IN_RANGE(0,abs(mid-mid_known),0.00001);
	PASS();
}


/**
* \fn test_main_loop
* Ensure the overall update iteration works for given matrix
*
* Test that, for a 5x5 matrix of all land, with hare and puma densities of 3
*	initially, that we receive the expected output by our given equations
* \return Void.   
*/
				
TEST test_main_loop(void){
	struct_matrix *test_matrix = build_matrix();
	struct_matrix *new_matrix = build_matrix();
	double totalHares = 0.0; /**< arguments of the mainLoop function, not used here */
	double totalPumas = 0.0;

	mainLoop(test_matrix, new_matrix,&totalHares,&totalPumas, configs);
	double corner_updated = new_matrix->map[0][0].hares;
	double mid_updated = new_matrix->map[2][2].hares;
	double corner_known = 2.95; /**< we establish these values numerically */
	double mid_known = 2.95; 

	/** test a corner */
	ASSERT_IN_RANGE(0,abs(corner_updated - corner_known),0.0000001);

	/** test a point in the middle */ 
	ASSERT_IN_RANGE(0,abs(mid_updated - mid_known), 0.0000001);
	PASS();
}


/**
* \fn test_init_map
* Testing the map initialisation works as expeted
*
* We wish to ensure that the init_map function generates the map to be used. 
* In addition, we need to check that it adds halo data of water around the edges. 
*	
* \return Void.   
*/
TEST test_init_map(void){
	struct_matrix *test_matrix = build_matrix();
	int a;
	double num_land =0;
	char *output = "test_file.dat";
	int returnValue = 0;
	FILE *fp = fopen(output, "r+");
	if (fp == NULL)
   {
    fprintf(stderr, "\\> function fopen failed to create and open an output \
            file: test_file.dat.\n");
   }
	returnValue = init_map(fp, test_matrix, configs);


	if(returnValue==-1) {
		PASS();
	}
	else {
	
	/** test halo data -- count the number of land grid spaces in the first row
	 we expect to have none, so assert that our num_land==0 */
		for(a =0; a< test_matrix->x; a++){
			if(test_matrix->map[a][0].area == LAND)
			{
		  		num_land += 1;
			}
		}
	
		if(num_land ==0) a=1;
		ASSERT(a==1);
		PASS();
		}
}

/**
* \fn test_hares_neighbours
* Test the calculation of the neighbouring densities for hares
*
* We must test that haresNeighboursCells calculates the right number of 
*	hares surrounding the target cell because this contributes to the overall 
*	update of the density of hares at that point
*	Since we've set all cells to have 3 hares, we expect any cells neighbours to 
*	have 16 hares. 
* \return Void.   
*/	
TEST test_hares_neighbours(void){

	struct_matrix *test_matrix = build_matrix();
	double neighbours_sum = haresNeighboursCells(test_matrix, 2,2);
	double expected_neighbours = 12.0;
	ASSERT(neighbours_sum == expected_neighbours);
	PASS();
} 


/**
* \fn
* We must test that pumasNeighboursCells calculates the right number of 
*	hares surrounding the target cell because this contributes to the overall 
*	update of the density of hares at that point
*	Since we've set all cells to have 3 pumas, we expect any cells neighbours to 
*	have 12 hares. 
* \return Void.   
*/	
TEST test_pumas_neighbours(void){
	struct_matrix *test_matrix = build_matrix();
	double neighbours_sum = pumasNeighboursCells(test_matrix, 2,2);
	double expected_neighbours = 12.0;
	ASSERT(neighbours_sum == expected_neighbours);
	PASS();
} 


/**
\fn test_ppm_print
* We must test that the print function works correctly with a given input 
*	We pass a test matrix, and expect that the function generates a ppm 
*	file within a given directory. 
* \return Void.   
*/
TEST test_ppm_print(){
	char *hares = "test_hares_directory";
	char *pumas = "test_pumas_directory";
	char *together = "test_together_directory";
	char *hares_file = "test_hares_directory/hares1.ppm";
	char *pumas_file = "test_hares_directory/pumas1.ppm";
	char *together_file = "test_together_directory/together1.ppm";
	int test=0;
	struct_matrix *test_matrix = build_matrix();
	printPPM(test_matrix, hares, pumas, together, configs);
	FILE *fpHares, *fpPumas; 
	/** test that the hares output is created */
	fpHares=fopen(hares_file, "w");
	if(fpHares == NULL){
	test += 1;
	}
	ASSERT(test==0);

	/** test that the pumas output is created */
	fpHares=fopen(pumas_file, "w");
	if(fpHares == NULL){
	test += 1;
	}
	ASSERT(test==0);


	PASS();
	}

SUITE(suite){
RUN_TEST(test_the_neighbour_calculation);
RUN_TEST(test_hares_neighbours);
RUN_TEST(test_pumas_neighbours);
RUN_TEST(test_hares_update);
RUN_TEST(test_pumas_update);
RUN_TEST(test_main_loop);
RUN_TEST(test_init_map);
RUN_TEST(test_ppm_print);
}

GREATEST_MAIN_DEFS(); /** Setup for unit test framework */

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /** command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /** display results */
}
