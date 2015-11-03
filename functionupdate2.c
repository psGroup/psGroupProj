/** \file doxygen.c
 *  \brief Puma and hare density
 *
 *  This contains code for determining the number of hares and
 *  Pumas in a grid. There will be water present on the grid, and
 *  in water squares, no important animals live.
 *
 *  \author Karl Delargy (kdelargy)
 *  \author Brian Flynn (bflynn)
 *  \author Killian Tattan (ktattan)
 *  \author Angelos Stylianides (astyliandes)
 *  \bug No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



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


/**
*setting the colour scales for the output ppm files
*/
double min_colour = 70.0;
double scale_pumas = 37.2;//(256.0 - min_colour)/crit_pumas_upper;
double scale_hares = 37.2;//(256.0 - min_colour)/crit_hares_upper;

/** \enum enum_area
    \brief Enum.

    An enum where land is set to have a value of 1 and water has a value of zero.
*/

enum enum_area{LAND, WATER};

/*! \var typedef struct cell
    \typedef struct cell .

    Details.
*/

/*! \struct cell
    \A struct.

    We are making our cells so that they contain 3 things, whether they are land or water
    , the density of pumas and the density of hares.
*/

typedef struct cell {
  enum enum_area area;
  double pumas;
  double hares;
} struct_cell;

/*! \var typedef struct matrix
    \typedef struct matrix .

    Details.
*/

/*! \struct matrix
    \A struct.

    A more detailed class description.
*/

typedef struct matrix {
  /*\{*/
  int x; /**< 1D */
  int y; /**< 2D */
  struct_cell **map; /**< 3D */
  /*\}*/
} struct_matrix;

/** \fn dynamic_alloc_map
 * \brief  brief description of what the fucntion does
 * \fn dynamic_alloc_map
 *
 *
 *
 *
 *  \param x dimesnions of matrix
 *  \param y dimesnions of matrix
 *  \return map.
 */

struct_cell **dynamic_alloc_map(int x, int y) {

  struct_cell **map;
  int i;

  map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
  if (map == NULL)
  {
    fprintf(stderr, "malloc faild\n");
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
      fprintf(stderr, "malloc faild\n");
      exit(EXIT_FAILURE);
    }
  }

  return map;

}

/** * \fn free_map
 *\brief  brief description of what the fucntion does
 *
 *
 *
 *
 *  \param gameland talk about what gameland does
 *  \return Void.
 */

void free_map(struct_matrix *gameLand) {
  int i;

  for (i = 0; i < gameLand->x; i++)
  {
    free(gameLand->map[i]);
  }
  //TODO check that
//  free(gameLand->map);

}

/** \fn init_map
 * \brief  brief description of what the fucntion does
 * \fn init_map
 *
 *
 * This one is quite big so I think we should explain this one
 * A little bit more
 *
 *  \param gameland talk about what gameland does
 *  \return Void.
 */

void init_map(FILE *fp, struct_matrix *gameLand) {
  char *line1 = NULL;
  char *line2 = NULL;
  char *token;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t read;
  int nx, ny, i, j;

  /**
  * \brief talk awhat happens next
  */

  //TODO add coments
  if ((read = getline(&line1, &len, fp)) != -1)
  {
    token = strtok_r(line1, " ", &saveptr);
    gameLand->y = atoi(token) + 2;
    token = strtok_r(NULL, " ", &saveptr);
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
            gameLand->map[i][j].area = LAND;
// We need to gaurantee that the initial densities are randomly distributed between 0.1 (our lower bound, below which the animal dies) and upper (at which they saturate).
// (upper-lower)*rand ensures a random number between 0 and upper_limit.
// we add the lower limit to make it a random number between the two limits
            gameLand->map[i][j].pumas = ((crit_pumas_upper - crit_pumas_lower) * rand()/RAND_MAX )  + crit_pumas_lower;
            gameLand->map[i][j].hares = ((crit_hares_upper - crit_hares_lower) * rand()/RAND_MAX )  + crit_hares_lower;
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
}


/** \fn landNeighboursCells
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param i talk about what i does
 *  \param j talk about what j does
 *  \return n.
 */

double landNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  double n = 0.0;

  if (gameLand->map[i-1][j].area == LAND)
		n+= 1.0;

	if (gameLand->map[i+1][j].area == LAND)
	 n+= 1.0;

	if (gameLand->map[i][j-1].area == LAND)
	 n+= 1.0;

	if (gameLand->map[i][j+1].area == LAND)
	 n+= 1.0;

	return n;
}

/** \fn haresNeighboursCells
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param i talk about what i does
 *  \param j talk about what j does
 *  \return Void.
 */

double haresNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].hares + \
          gameLand->map[i+1][j].hares + \
          gameLand->map[i][j-1].hares + \
          gameLand->map[i][j+1].hares);
}

/** \fn haresNewValue
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param i talk about what i does
 *  \param j talk about what j does
 *  \return newValue.
 */


double haresNewValue(struct_matrix *gameLand, int i, int j) {
  double oldValue = gameLand->map[i][j].hares;
  double newValue = 0.0;
  double neibarsHaresValues = haresNeighboursCells(gameLand, i, j);
  double neibarsLandCells = landNeighboursCells(gameLand, i, j);
  double randomWalk = 0.0;

  //randomWalk
  randomWalk = k * (neibarsHaresValues - (neibarsLandCells * oldValue));

  newValue = ((r*oldValue) - a * oldValue * gameLand->map[i][j].pumas);
  newValue += randomWalk;
  newValue *= dt;
  newValue += oldValue;

  return newValue;
}
/** \fn pumasNeighboursCells
 * \brief  brief description of what the fucntion does
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param i talk about what i does
 *  \param j talk about what j does
 */

double pumasNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].pumas + \
          gameLand->map[i+1][j].pumas + \
          gameLand->map[i][j-1].pumas + \
          gameLand->map[i][j+1].pumas);
}

/** \fn pumasNewValue
 * \brief  brief description of what the fucntion does
 * \fn pumasNewValue
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param i talk about what i does
 *  \param j talk about what j does
 *  \return newValue.
 */

double pumasNewValue(struct_matrix *gameLand, int i, int j) {
  double oldValue = gameLand->map[i][j].pumas;
  double newValue = 0.0;
  double neibarsPumasValues = pumasNeighboursCells(gameLand, i, j);
  double neibarsLandCells = landNeighboursCells(gameLand, i, j);
  double randomWalk = 0.0;

  //randomWalk
  randomWalk = l * (neibarsPumasValues - (neibarsLandCells * oldValue));

  newValue = (b*oldValue * gameLand->map[i][j].hares) - m * oldValue;
  newValue += randomWalk;
  newValue *= dt;
  newValue += oldValue;

  return newValue;
}

/** \fn mainLoop
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param newGameLand
 *  \return Void.
 */

void mainLoop(struct_matrix *gameLand, struct_matrix *newGameLand) {

  struct_matrix *tempPtr;
	int i, j;
	double n;

	for (i = 1; i < gameLand->x; i++) {
    for (j = 1; j < gameLand->y; j++) {
  		newGameLand->map[i][j].area = gameLand->map[i][j].area;
				if (gameLand->map[i][j].area == LAND) {
					newGameLand->map[i][j].hares = haresNewValue(gameLand, i ,j);
					newGameLand->map[i][j].pumas = pumasNewValue(gameLand, i ,j);

					// if statements to check critical densities
					if(gameLand->map[i][j].hares < crit_hares_lower){gameLand->map[i][j].hares = 0.0;}
					else if(gameLand->map[i][j].hares > crit_hares_upper){gameLand->map[i][j].hares = crit_hares_upper;}
					if(gameLand->map[i][j].pumas < crit_pumas_lower){gameLand->map[i][j].pumas = 0.0;}
					else if(gameLand->map[i][j].pumas > crit_hares_upper){gameLand->map[i][j].pumas = crit_pumas_upper;}
				}
			}
		}
}

/** \fn printPPM
 * \brief  brief description of what the fucntion does
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \return Void.
 */

void printPPM(struct_matrix *gameLand) {
  static int fileID = 0;
  int i, j, k, ki;
  FILE *fpHares, *fpPumas, *fpTogether;
  char haresFileName[30] = "haresOut/hares"; //{ '\0' };
  char pumasFileName[30] = "pumasOut/pumas";
  char togetherFileName[30] = "togetherOut/together";
  fileID++;

  sprintf(haresFileName, "%s%d.ppm", haresFileName, fileID);
  fpHares=fopen(haresFileName, "w");

  sprintf(pumasFileName, "%s%d.ppm", pumasFileName, fileID);
  fpPumas=fopen(pumasFileName, "w");

  sprintf(togetherFileName, "%s%d.ppm", togetherFileName, fileID);
  fpTogether=fopen(togetherFileName, "w");

  fprintf(fpHares, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2); // width = 400, height = 400
  fprintf(fpPumas, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);
  fprintf(fpTogether, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);

  for (i = 1; i < (gameLand->x)-1; i++)
  {
    for (j = 1; j < (gameLand->y)-1; j++) {
//      for (k = 0; k < 8; k++) {
        if (gameLand->map[i][j].area == LAND) {
//          for (ki = 0; ki < 8; ki++) {
            fprintf(fpHares, "%d %d %d\t", (int)((gameLand->map[i][j].hares * 100)), 0, 0);//scale_hares + min_colour)), 0, 0);
            fprintf(fpPumas, "%d %d %d\t", 0, 0, (int)((gameLand->map[i][j].pumas * 100)), 0, 0);//scale_pumas + min_colour)));
            fprintf(fpTogether, "%d %d %d\t", (int)((gameLand->map[i][j].hares * 100/*scale_hares + min_colour*/)), 0, (int)(256 - (gameLand->map[i][j].pumas * 100)));// scale_pumas + min_colour)));
//          }
        }
        else {
//          for (ki = 0; ki < 8; ki++) {
            fprintf(fpHares, "%d %d %d\t", 41, 73, 178);
            fprintf(fpPumas, "%d %d %d\t", 41, 73, 178);
            fprintf(fpTogether, "%d %d %d\t", 41, 73, 178);
//          }
        }
//      }
    }
    fprintf(fpHares, "\n");
    fprintf(fpPumas, "\n");
    fprintf(fpTogether, "\n");
  }
  fclose(fpHares);
  fclose(fpPumas);
  fclose(fpTogether);
}

/** \fn printMapToImg
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \param gameland talk about what gameland does
 *  \param i talk about what i does
 *  \param j talk about what j does
 *  \return newValue.
 */

void printMapToImg(struct_matrix *gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j, k, ki;
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d.ppm", fileName, fileID);

  fp=fopen(fileName, "w");

  fprintf(fp, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);

  for (i = 1; i < (gameLand->x - 1); i++) {
    for (j = 1; j < (gameLand->y - 1); j++) {
      if (gameLand->map[i][j].area == LAND) {
        fprintf(fp, "%d %d %d\t", 0, 0, 0);
      }
      else {
        fprintf(fp, "%d %d %d\t", 41, 73, 178);
      }
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

/** \fn printMap
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

void printMap(const struct_matrix *gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { '\0' };
  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");

   for (i = 1; i < (gameLand->x - 1); i++) {
    for (j = 1; j < (gameLand->y - 1); j++)
      fprintf(fp, "%d ", (gameLand->map[i][j].area == LAND) ? 1 : 0 );
    fprintf(fp, "\n");
  }

  fclose(fp);

}

/** \fn printHares
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 * \param gameLand
 *
 */

void printHares(struct_matrix *gameLand) { //}, char initFileName[30]) { //246, 15, 15
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { "haresValues/hares.txt" };

  fileID++;

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");

  for (i = 0; i < gameLand->x; i++)
  {
    for (j = 0; j < gameLand->y; j++) {
      fprintf(fp, "%.2f ", gameLand->map[i][j].hares);
//      fprintf(fp, "%c %c %c", gameLand.map[i][j].hares * 10, 10, 10);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

}

/** \fn printPumas
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

void printPumas(struct_matrix *gameLand) { //}, char initFileName[30]) { //246, 15, 15
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { "pumasValues/pumas.txt" };

  fileID++;

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");

  for (i = 0; i < gameLand->x; i++)
  {
    for (j = 0; j < gameLand->y; j++) {
      fprintf(fp, "%.2f ", gameLand->map[i][j].pumas);
//      fprintf(fp, "%c %c %c", gameLand.map[i][j].hares * 10, 10, 10);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

}

/** \fn Main
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

int main(int argc, char **argv) {

  char *inputFile = NULL;
  FILE *fp =NULL;
  struct_matrix *gameLand;
	struct_matrix *newGameLand;
  struct_cell **swap;

  int i, j;
  double t;
  srand(time(NULL));

  //Check if argv 2 exist
  if (argc != 2)
  {
    fprintf(stderr, "No input file\n");
    return -1;
  }

  //get input file name
  inputFile = strdup(argv[1]);
  if (inputFile == NULL)
  {
    fprintf(stderr, "\\> function srtdup fail to allocate space and return \
            NULL.\n");
    return -1;
  }

  //open the input file
  fp = fopen(inputFile, "r+");
  if (fp == NULL)
  {
    fprintf(stderr, "\\> function fopen fail to open the file: %s\n", \
            inputFile);
    return -1;
  }

  gameLand = (struct_matrix*) malloc(sizeof(struct_matrix));
  //initialize map
  init_map(fp, gameLand);

  newGameLand = (struct_matrix*) malloc(sizeof(struct_matrix));
  newGameLand->x = gameLand->x;
  newGameLand->y = gameLand->y;
  newGameLand->map = dynamic_alloc_map(newGameLand->x, newGameLand->y);

  //free file ponter
  fclose(fp);

  //free file name store array
  free(inputFile);

//  printMap(gameLand, "rawMap.txt");

//  printMapToImg(gameLand, "theMap");
//  printMap(gameLand, "theMap");

	for (t = 0.0;  t < 500.0; t += dt) {
		mainLoop(gameLand, newGameLand);
		printPPM(gameLand);
	  printHares(gameLand);
	  printPumas(gameLand);
		//swap the arrays
    swap = gameLand->map;
    gameLand->map = newGameLand->map;
    newGameLand->map = swap;
//    for (i = 0; i < gameLand->x; i++) {
//      memcpy(gameLand->map[i], newGameLand->map[i], sizeof(struct_cell) * gameLand->y);
//    }

//	  print_pumas(gameLand);
//	  printf("------------------------------------------------------------\n");
//	  printPPM(gameLand);
	//  printHares(gameLand);
	  //printPumas(gameLand);
	}

  //free programme matrix
  free_map(gameLand);
  free_map(newGameLand);
  //TODO chech that
//  free(gameLand);

  printf("Reach the end\n");
  return 0;
}
