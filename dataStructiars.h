#include "dataStructiars.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "global_values.h"

struct_cell **dynamic_alloc_map(int x, int y) {

  struct_cell **map;
  int i;

  map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
  if (map == NULL)
  {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  /**
  *Give a brief description of what is going on here
  */

  for (i = 0; i < x; i++)
  {
    map[i] = (struct_cell*) malloc(sizeof(struct_cell) * y);
    if (map[i] == NULL)
    {
      fprintf(stderr, "malloc failed\n");
      exit(EXIT_FAILURE);
    }
  }

  return map;

}


void free_map(struct_matrix *gameLand) {
  int i;

  for (i = 0; i < gameLand->x; i++)
  {
    free(gameLand->map[i]);
  }
  //TODO check that
//  free(gameLand->map);

}

int checkIfNumber(char *str) {
    int length = strlen (str);
    int i;

    for (i=0;i<length; i++) {
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

int init_map(FILE *fp, struct_matrix *gameLand, configurations configs) {
  char *line1 = NULL;
  char *line2 = NULL;
  char *token;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t read;
  int i, j;
  /**
  * \brief talk awhat happens next
  */

  //TODO add coments
  if ((read = getline(&line1, &len, fp)) != -1)
  {
    token = strtok_r(line1, " ", &saveptr);
   	if (checkIfNumber(token) == -1) return -1;
    gameLand->y = atoi(token) + 2;
    token = strtok_r(NULL, " ", &saveptr);
   	if (checkIfNumber(token) == -1) return -1;
    gameLand->x = atoi(token) + 2;
  }
  free(line1);
  gameLand->map = dynamic_alloc_map(gameLand->x, gameLand->y);

  printf("Rows:\t%d\n", gameLand->x);
  printf("Columns:\t%d\n", gameLand->y);

  /**
  * \brief talk awhat happens next
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
       	if (checkIfNumber(token) == -1) return -2;
            gameLand->map[i][j].area = LAND;
// We need to gaurantee that the initial densities are randomly distributed between 0.1 (our lower bound, below which the animal dies) and upper (at which they saturate).
// (upper-lower)*rand ensures a random number between 0 and upper_limit.
// we add the lower limit to make it a random number between the two limits
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
