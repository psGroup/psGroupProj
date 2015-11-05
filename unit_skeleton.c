#include "greatest.h" 
#include <stdlib.h>
#include <stdio.h>
// define a struct_matrix of all 1's: can be used to test the count number of neighbours fnc, update hares/pumas fncs, and the halo fnc

/* assumed we'd have to include equations.c to actually test the functions but it returns undefined reference to all variables */

//#include "global_values.h"
//#include "equations.c"
#include "equations.h"

//SUITE(suite);

TEST sample_test() {
int x = 1;
ASSERT(x==1);
PASS();
}


//test the landNeighboursCells function - to find the number of land cell neighbouring a cell
TEST test_the_neighbour_calculation(void){

int i = 2;
int j = 2;
struct_matrix test_matrix[i][j];

//what is a struct_matrix? it has i, j and struct_cell: what simple input can we test against?

for(int a =0; a<i; a++){
	for(int b=0; b<j; b++){
		test_matrix[a][b]->area = 1;
		}
	}	
	
int test_var = landNeighboursCells( *test_matrix, 0,0);
ASSERT(test_var==2);
PASS();
}


TEST test_hares_update(coid){
//define a 3x3 struct_matrix of all 1s

//test a corner
double corner = haresNeighbourCells( *test_matrix, 0,0);
double corner_known = 1; //input known value
ASSERT(corner==corner_known);
PASS();
//test a mid pt
double mid = haresNeighbourCells(*test_matrix, 1,1);
double mid_known = 1; //input known value
ASSERT(mid == mid_known)
PASS();
}

TEST test_pumas_update(void){
//test a corner
double corner = pumasNeighbourCells( *test_matrix, 0,0);
double corner_known = 1; //input known value
ASSERT(corner==corner_known);
PASS();
//test a mid pt
double mid = pumasNeighbourCells(*test_matrix, 1,1);
double mid_known = 1; //input known value
ASSERT(mid == mid_known)
PASS();
}

TEST test_main_loop(void){
//input initial test matrix, and blank matrix as second argument
struct_matrix updated_matrix = mainLoop( *first_matrix, *second_matrix);

// test a corner
corner_known == //input correct value for initial matrix
ASSERT(updated_matrix[0][0] == corner_known)
//test middle 
mid_known == //input correct value for initial matrix
ASSERT(updated_matrix[0][0] == mid_known)

}


TEST test_halo(void){

}




SUITE(suite){
RUN_TEST(sample_test);
RUN_TEST(test_the_neighbour_calculation);
RUN_TEST(test_pumas_update);
RUN_TEST(test_pumas_update);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
