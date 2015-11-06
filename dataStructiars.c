/** \file dataStructiars.c
 *  \brief Explain
 *
 *
 *  \author B083194
 *  \author B084292
 *  \author B082906
 *  \author B088321
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#include "dataStructiars.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "global_values.h"

/** 
 * 
 * dynamically allocating memory to a map 
 * where we will input values for land/water, 
 * hare and puma densities
 */

struct_cell **dynamic_alloc_map(int x/**<[in] number of rows allocated memory  */, int y/**<[in] number of columns allocated memory  */) {

  struct_cell **map;
  int i;

  map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
  if (map == NULL)
  {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < x; i++)
  {
    map[i] = (struct_cell*) malloc(sizeof(struct_cell) * y);
    if (map[i] == NULL)
    {
      fprintf(stderr, "malloc failed\n");/**< We must test that the memory allocation has worked correctly  */
      exit(EXIT_FAILURE);
    }
  }

  return map;

}

/**
* function to deallocate memory when we no longer need it
*/
void free_map(struct_matrix *gameLand) {
  int i;

  for (i = 0; i < gameLand->x; i++)
  {
    free(gameLand->map[i]);
  }

}

int checkIfNumber(char *str, size_t len) {
    int i;

    for (i=0;i<len; i++) {
    	  if (str[i] == '\n') {
    	  	return 2;
    	  	}
        else if (!isdigit(str[i]))
        {
            fprintf (stderr, "Entered input is not a number -->%c<--\n", str[i]);
            return -1;
        }
    }
    return 0;
}


/** \fn init_map
 * \brief  Initialise map for from given matrix
 *
 * When we pass in a binary file indicating land/water,
 * this function turns it into a matrix of structs, each of 
 * which contains an element for land/water, a random density of hares,
 * and a random density of pumas. 
 * We also add halo data: an extra edge of all water, so that our functions all work correctly. 
 * 
 * \return n
 *
 */

int init_map(FILE *fp, struct_matrix *gameLand, configurations configs) {
  char *line1 = NULL;
  char *line2 = NULL;
  char *token;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t read;
  int i, j;
 

  if ((read = getline(&line1, &len, fp)) != -1)
  {
    token = strtok_r(line1, " ", &saveptr);
   	if (checkIfNumber(token, len) == -1) return -1;
    gameLand->y = atoi(token) + 2;
    token = strtok_r(NULL, " ", &saveptr);
   	if (checkIfNumber(token, len) == -1) return -1;
    gameLand->x = atoi(token) + 2;
  }
  free(line1);
  gameLand->map = dynamic_alloc_map(gameLand->x, gameLand->y);

  printf("Rows:\t%d\n", gameLand->x);
  printf("Columns:\t%d\n", gameLand->y);

  /**
  * apply halo data - ie add a row and column to each end, filled with type water. 
  */

  for (i = 0; i < gameLand->x; i++)
  {
    gameLand->map[i][0].area = \
    gameLand->map[i][gameLand->y - 1].area = WATER;
    gameLand->map[i][0].hares = \
    gameLand->map[i][gameLand->y - 1].hares = \
    gameLand->map[i][0].pumas = \
    gameLand->map[i][gameLand->y - 1].pumas = 0.0;
  }

  for (j = 0; j < gameLand->y; j++)
  {
    gameLand->map[0][j].area = \
    gameLand->map[gameLand->x - 1][j].area = WATER;
    gameLand->map[0][j].hares = \
    gameLand->map[gameLand->x - 1][j].hares = \
    gameLand->map[0][j].pumas = \
    gameLand->map[gameLand->x - 1][j].pumas = 0.0;
  }

  i = j = 1;
  while((read = getline(&line2, &len, fp)) != -1)
  {
    j = 1;
    for (token = strtok_r(line2, " ", &saveptr); \
         token != NULL; \
         token = strtok_r(NULL, " ", &saveptr))
    {
      if (j < gameLand->y) {
        if (atoi(token) == 1)
        {
       	if (checkIfNumber(token, len) == -1) return -2;
            gameLand->map[i][j].area = LAND;
/**
 * We need to gaurantee that the initial densities are randomly distributed between 0.1 (our lower bound, below which the animal dies) and upper (at which they saturate).
 * (upper-lower)*rand ensures a random number between 0 and upper_limit.
 * we add the lower limit to make it a random number between the two limits
*/
            gameLand->map[i][j].pumas = ((configs.crit_pumas_upper - configs.crit_pumas_lower) * rand()/RAND_MAX )  + configs.crit_pumas_lower;
            gameLand->map[i][j].hares = ((configs.crit_hares_upper - configs.crit_hares_lower) * rand()/RAND_MAX )  + configs.crit_hares_lower;
            j++;
        }
        else
        {
            gameLand->map[i][j].area = WATER;
            gameLand->map[i][j].pumas = 0.0;
            gameLand->map[i][j].hares = 0.0;
            j++;
        }
      }
    }
    i++;
  }

  free(line2);
  return 0;
}
