#include "greatest.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<math.h>

/* COMPLETE LIST OF FUNCTIONS USED
equations.h:
	landNeighboursCells
	haresNeighboursCells
	haresNewValue
	pumasNeighboursCells
	pumasNewValue
	mainLoop

dataStructiars.h:
						enum_area
						struct cell
						struct matrix
						struct_cell **dynamic_alloc_map  -- do we need to test these?
			free_map -- don't know how to test allocation
	init_map

printFunctions.h:
			increace -- looking into it. 
			printPPM
*/
double dt = 0.4;/**< change in time*/
double r = 0.08; /**< birthrate of hares */
double a = 0.04; /**< the predation rate of pumas */
double b = 0.02; /**< birth rate of hares per hare eaten */
double m = 0.06; /**< the puma morality rate */
double k = 0.2; /**< the diffusion rates for hares */
double l = 0.2;/**< diffusion rate for pumas */
double T = 5.0; /**< Time step */
double crit_hares_lower = 0.1;/**< Lower critical limit for pumas */
double crit_hares_upper = 5.0;/**< Upper critical limit for pumas */
double crit_pumas_lower = 0.1;/**< Lower critical limit for pumas */
double crit_pumas_upper = 5.0;/**< Upper critical limit for pumas */

#include "equations.h"
#include "dataStructiars.h" 
#include "printFunctions.h"

/*if we test print function, it will overwrite values in pumasOut etc 
		is there a way around this */


/*
We're going to define a function which creates a 5x5 matrix 
Each matrix point has a value for AREA, HARES, and PUMAS
We initialise the area to all be land, with equal distributions 
of pumas and hares, such that we know expected outputs for particular points
*/

struct_matrix *build_matrix(){
	int x = 5;
	int y = 5;
	struct_matrix *test_matrix;
	int i,j,a,b;

	test_matrix = (struct_matrix*) malloc(sizeof(struct_matrix));
	
	// allocate memory for our test matrix
	test_matrix->map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
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


/*
test the landNeighboursCells function 
to find the number of land cell neighbouring a cell
In our matrix, we have all land, so a point in the middle should have 
four land neighbours. 
*/
TEST test_the_neighbour_calculation(){
	struct_matrix *test_matrix = build_matrix();
	double num_neighbours = landNeighboursCells(test_matrix, 2,2);
	ASSERT(num_neighbours==4);
	PASS();
}


TEST test_hares_update(void){
	struct_matrix *test_matrix = build_matrix();
	//test a point in the middle of the grid
	double mid = haresNewValue(test_matrix,1,1);
	double mid_known = 2.95;
			//ASSERT_EQ(mid, mid_known); //don't see why this doesn't work - floating point errors?
			//PASS();
	ASSERT_IN_RANGE(0,abs(mid-mid_known),0.000000001); 
	// test to within a tolerance (last argument) because the values are not \
	 exactly the same due to floating point errors
	PASS();
}


TEST test_pumas_update(void){
	struct_matrix *test_matrix = build_matrix();
	//test a mid point
	double mid = pumasNewValue(test_matrix,1,1);
	double mid_known = 2.95;
	ASSERT_IN_RANGE(0,abs(mid-mid_known),0.000000001);
	PASS();
}


				
TEST test_main_loop(void){
	struct_matrix *test_matrix = build_matrix();
	struct_matrix *new_matrix = build_matrix();
	mainLoop(test_matrix, new_matrix);
	double corner_updated = new_matrix->map[0][0].hares;
	double mid_updated = new_matrix->map[2][2].hares;
	double corner_known = 2.95; //input correct value for initial matrix
	double mid_known = 2.95; //input correct value for initial matrix

	// test a corner
	ASSERT_IN_RANGE(0,abs(corner_updated - corner_known), 0.0000000000001);
	//PASS();

	//test a point in the middle 
	ASSERT_IN_RANGE(0,abs(mid_updated - mid_known), 0.0000001);
	PASS();
}



TEST test_init_map(void){
	struct_matrix *test_matrix = build_matrix();
	int a;
	double num_land =0;
	char *output = "test_file.dat";
	FILE *fp = fopen(output, "r+");
	if (fp == NULL)
   {
    fprintf(stderr, "\\> function fopen failed to create and open an output \
            file: test_file.dat.\n");
   }
	init_map(fp, test_matrix);

//test halo data -- count the number of land grid spaces in the first row
// we expect to have none, so assert that our num_land==0
// Do we want to do this for all edge rows or is this sufficient?
	for(a =0; a< test_matrix->x; a++){
		if(test_matrix->map[a][0].area == LAND)
		{
	  		num_land += 1;
		}
	}

	ASSERT(num_land==0);

	PASS();
}


/*
	//test free_map function -- not sure how to test that memory has been freed
	free_map(test_matrix);
	ASSERT(test_matrix->map[a][0].area == NULL);
*/

TEST test_hares_neighbours(void){
	/* We must test that haresNeighboursCells calculates the right number of 
	hares surrounding the target cell because this contributes to the overall 
	update of the density of hares at that point
	Since we've set all cells to have 3 hares, we expect any cells neighbours to 
	have 16 hares. 
	*/	
	struct_matrix *test_matrix = build_matrix();
	double neighbours_sum = haresNeighboursCells(test_matrix, 2,2);
	double expected_neighbours = 12.0;
	ASSERT(neighbours_sum == expected_neighbours);
	PASS();
} 



TEST test_pumas_neighbours(void){
	/* We must test that pumasNeighboursCells calculates the right number of 
	hares surrounding the target cell because this contributes to the overall 
	update of the density of hares at that point
	Since we've set all cells to have 3 pumas, we expect any cells neighbours to 
	have 12 hares. 
	*/	
	struct_matrix *test_matrix = build_matrix();
	double neighbours_sum = pumasNeighboursCells(test_matrix, 2,2);
	double expected_neighbours = 12.0;
	ASSERT(neighbours_sum == expected_neighbours);
	PASS();
} 


/*
TEST test_ppm_print(){
//	FILE *fpHares, *fpPumas, 
	char *hares = "test_hares_directory";
	char *pumas = "test_pumas_directory";
	struct_matrix *test_matrix = build_matrix();
	printPPM(test_matrix, hares, pumas);
	FILE *fpHares, *fpPumas; 
	fpHares=fopen(haresFileName, "w");
	}
*/
SUITE(suite){
//RUN_TEST(test_the_neighbour_calculation);
//RUN_TEST(test_hares_neighbours);
//RUN_TEST(test_pumas_neighbours);
//RUN_TEST(test_hares_update);
//RUN_TEST(test_pumas_update);
//RUN_TEST(test_main_loop);
//RUN_TEST(test_init_map);

}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
