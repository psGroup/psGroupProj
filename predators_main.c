/** \file predators_main.c
 *  \brief Puma and hare density
 *
 *  This contains code for determining the number of hares and
 *  Pumas in a grid. There will be water present on the grid, and
 *  in water squares, no important animals live.
 *
 *  \author B083194
 *  \author B084292
 *  \author B082906
 *  \author B088321
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "dataStructiars.h"
#include "equations.h"
#include "printFunctions.h"

#include "global_values.h"


/** \fn main
 * \brief  This is the main function where we run the time for-loop.
 *
 *
 */

int main(int argc, char **argv) {

  char *inputFile = NULL;
  FILE *fp = NULL;
  FILE *fpAvgLand = NULL;
  FILE *fpAvgGrid = NULL;
  struct_matrix *gameLand;
	struct_matrix *newGameLand;
  struct_cell **swap;

  int i, j;
  double t;
  double totalHares = 0.0;/**< Total hares in grid to calculate average density */
  double avgHaresLand = 0.0;
  double totalPumas = 0.0;/**< Total pumas in grid to calculate average density */
  double avgPumasLand = 0.0;
  double avgHaresGrid = 0.0;
  double avgPumasGrid = 0.0;
  configurations configs;
  
  srand(time(NULL));/**< The random number generator we use to initialise the densities over the grid for hares and pumas */

 /**
  *Expecting 2 arguments in the command line, the second argument being the input file.
  */
  if (argc != 2)
  {
    fprintf(stderr, "No input file\n");
    return -1;
  }


  inputFile = strdup(argv[1]);
  if (inputFile == NULL)
  {
    fprintf(stderr, "\\> function srtdup fail to allocate space and return \
            NULL.\n");
    return -1;
  }
  
  fp = fopen("configFile", "r");
  if (fp == NULL)
  {
    fprintf(stderr, "\\> function fopen fail to open the file: %s\n", "configFile");
    return -1;
  }
  configs = parse_configs(fp);
  free(fp);


  /**
  *open the input file
  */
  fp = fopen(inputFile, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "\\> function fopen fail to open the file: %s\n", \
            inputFile);
    return -1;
  }

  gameLand = (struct_matrix*) malloc(sizeof(struct_matrix));
  if(gameLand == NULL) {
  	fprintf(stderr, "\\> Malloc failed for gameLand \n");
  }
  
  /**
  *initialize map
  */
  init_map(fp, gameLand, configs);

    /**
  *free file pointer
  */
  fclose(fp);

  /**
  *free file name store array
  */
  free(inputFile);

  fpAvgLand=fopen("averageLand.txt", "w");
  if (fpAvgLand == NULL) {
    fprintf(stderr, "file append fail\n");
    return -1;
  }

  fpAvgGrid=fopen("averageGrid.txt", "w");
  if (fpAvgGrid == NULL) {
    fprintf(stderr, "file append fail\n");
    return -1;
  }
/**
  *Print Header
  */
  fprintf(fpAvgLand, "    Time    Average Hare Density    Average Puma Density\n");
  fprintf(fpAvgGrid, "    Time    Average Hare Density    Average Puma Density\n");

  newGameLand = (struct_matrix*) malloc(sizeof(struct_matrix));
  if(newGameLand == NULL) {
  	fprintf(stderr, "\\>Malloc failed for newGameLand\n");
  }
  
  newGameLand->x = gameLand->x;
  newGameLand->y = gameLand->y;
  newGameLand->map = dynamic_alloc_map(newGameLand->x, newGameLand->y);

/**
  *Initialising the grid. If grid is water the densities of hares and pumas is zero.
  */

  for (i = 0; i < newGameLand->x; i++)
  {
    newGameLand->map[i][0].area = \
    newGameLand->map[i][newGameLand->y - 1].area = WATER;
    newGameLand->map[i][0].hares = \
    newGameLand->map[i][newGameLand->y - 1].hares = \
    newGameLand->map[i][0].pumas = \
    newGameLand->map[i][newGameLand->y - 1].pumas = 0.0;
  }

  for (j = 0; j < newGameLand->y; j++)
  {
    newGameLand->map[0][j].area = \
    newGameLand->map[newGameLand->x - 1][j].area = WATER;
    newGameLand->map[0][j].hares = \
    newGameLand->map[newGameLand->x - 1][j].hares = \
    newGameLand->map[0][j].pumas = \
    newGameLand->map[newGameLand->x - 1][j].pumas = 0.0;
  }

/**
*calculating the number of grid points that are land.
*/
  for (i = 1; i < gameLand->x; i++) {
    for (j = 1; j < gameLand->y; j++) {
        if (gameLand->map[i][j].area == LAND) {
          land_number++;
        }
    }
}

grid_number = (gameLand->x - 2)*(gameLand->y - 2);

	char *pumas_directory = "pumasPPM";
	char *hares_directory = "haresPPM";
	char *together_directory = "togetherPPM";
	
	for (t = 0.0;  t < 500.0; t += configs.dt) {
		/**
		 * Need to initialse total number of hares / pumas to 0 for each time iteration
		 */
    totalHares = 0.0;
    totalPumas = 0.0;
		mainLoop(gameLand, newGameLand, &totalHares, &totalPumas, configs);
		/**
		 * Print outputs every T timesteps.
		 */
    if(fmod(t, configs.T) < configs.dt){
 		   printPPM(gameLand, hares_directory, pumas_directory, together_directory, configs);
       printHares(gameLand);
       printPumas(gameLand);

       /** 
        * Calculate the average densities over land only
        * 
        */
       avgHaresLand = totalHares/((double)land_number);
       avgPumasLand = totalPumas/((double)land_number);

       /** 
        * Calculate the average densities over entire grid area
        * 
        */
       avgHaresGrid = totalHares/((double)grid_number);
       avgPumasGrid = totalPumas/((double)grid_number);

       /** 
        * Output averages
        * 
        */
      printLandAvg(fpAvgLand, t, avgHaresLand, avgPumasLand);
      printGridAvg(fpAvgGrid, t, avgHaresGrid, avgPumasGrid);

    }

		/**
		*swap the arrays
		*
		*/
    swap = gameLand->map;
    gameLand->map = newGameLand->map;
    newGameLand->map = swap;
	}

  /**
   * free programme matrix
   * 
   */
  free_map(gameLand);
  free_map(newGameLand);

  fclose(fpAvgLand);
  fclose(fpAvgGrid);
  printf("Reach the end\n");
  return 0;
}
