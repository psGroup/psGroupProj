/** @file doxygen.c 
   @brief Puma and hare density
 
   This contains code for determining the number of hares and
   Pumas in a grid. There will be water present on the grid, and
   in water squares, no important animals live.
 
   @author B083194
   @author B084292
   @author B082906
   @author B088321
   @bug No known bugs.
 */

/**
 libraries
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
 setting the colour scales for the output ppm files
*/
double min_colour = 70.0;
double scale_pumas = 37.2;//(256.0 - min_colour)/crit_pumas_upper;
double scale_hares = 37.2;//(256.0 - min_colour)/crit_hares_upper;

/** @enum enum_area
   @brief Enum.

    enumeration list: Water, Land. Each cell must be one of these
*/

enum enum_area{LAND, WATER};

/** @var typedef struct cell
    @typedef struct cell .
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

/** @var typedef struct matrix
    @typedef struct matrix .
    Details.
*/

/** @struct matrix
    @A struct.
    A more detailed class description.
*/

typedef struct matrix {
  /*\{*/
  int x; /**< 1D */
  int y; /**< 2D */
  struct_cell **map; /**< 3D */
  /*\}*/
} struct_matrix;

/** @fn dynamic_alloc_map
  @brief  brief description of what the fucntion does
 
 
 
 
 
  @param x dimesnions of matrix
  @param y dimesnions of matrix
  @return map.
 */

struct_cell **dynamic_alloc_map(int x, int y) {

  struct_cell **map; /**< **map is a pointer variable */
  int i; /**< To initialise the for loop */

  map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
  if (map == NULL)
  {
    fprintf(stderr, "malloc faild\n");
    exit(EXIT_FAILURE);
  }

  /**
  * @brief Give a brief description of what is going on here
  */

  for (i = 0; i < x; i++)
  {
    map[i] = (struct_cell*) malloc(sizeof(struct_cell) * y); /**< Explain what is happening */
    if (map[i] == NULL)
    {
      fprintf(stderr, "malloc faild\n"); /**< Why do you do this? */
      exit(EXIT_FAILURE);
    }
  }

  return map;

}

/** @fn free_map
  @brief  brief description of what the fucntion does
 
 
 
 
   @param gameland talk about what gameland does
   @return Void.
 */

void free_map(struct_matrix *gameLand) {
  int i; /**< To initialise the for loop */


  for (i = 0; i < gameLand->x; i++)
  {
    free(gameLand->map[i]); /**< Explain what free does and why  */
  }
  //TODO check that
//  free(gameLand->map);

}

/** @fn init_map
 * @brief  brief description of what the fucntion does
 * @fn init_map
 *
 *
 * This one is quite big so I think we should explain this one
 * A little bit more
 *
 *  @param gameland talk about what gameland does
 *  @return Void.
 */

void init_map(FILE *fp, struct_matrix *gameLand) {
  char *line1 = NULL; /**< Pointer */
  char *line2 = NULL; /**< Pointer */
  char *token; /**< Pointer */
  char *saveptr = NULL; /**< Pointer */
  size_t len = 0; /**< explain this */
  ssize_t read;  /**< explain this */
  int nx, ny, i, j;  /**< explain these */

  /**
  * @brief talk what happens next
  */

  //TODO add coments
  if ((read = getline(&line1, &len, fp)) != -1)
  {
    token = strtok_r(line1, " ", &saveptr); /**< explain this */
    gameLand->y = atoi(token) + 2; /**< explain this */
    token = strtok_r(NULL, " ", &saveptr); /**< explain this */
    gameLand->x = atoi(token) + 2; /**< explain this */
  }
  free(line1); /**< fixing memory leakages */

  gameLand->map = dynamic_alloc_map(gameLand->x, gameLand->y); /**< explain this */

  printf("Rows:\t%d\n", gameLand->x);   /**< why these print statements */
  printf("Columns:\t%d\n", gameLand->y);  /**< why these print statements */

  /**
  * @brief talk awhat happens next
  */

  for (i = 0; i < gameLand->x; i++) /**< explain what this for loop does */
  {
    gameLand->map[i][0].area = \
    gameLand->map[i][gameLand->y - 1].area = WATER;
    gameLand->map[i][0].hares = \
    gameLand->map[i][gameLand->y - 1].hares = \
    gameLand->map[i][0].pumas = \
    gameLand->map[i][gameLand->y - 1].pumas = 0.0;
  }

  for (j = 0; j < gameLand->y; j++)  /**< explain what this for loop does */
  {
    gameLand->map[0][j].area = \
    gameLand->map[gameLand->x - 1][j].area = WATER;
    gameLand->map[0][j].hares = \
    gameLand->map[gameLand->x - 1][j].hares = \
    gameLand->map[0][j].pumas = \
    gameLand->map[gameLand->x - 1][j].pumas = 0.0;
  }

  i = j = 1;  /**< initialising */
  while((read = getline(&line2, &len, fp)) != -1)  /**< what this while/for  loop does */
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
//(upper-lower)*rand ensures a random number between 0 and upper_limit.
 //we add the lower limit to make it a random number between the two limits


 /**
 * Need to explain this next bit of code
 */
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


/** @fn landNeighboursCells
 * @brief  this counts the numbers of neighbouring cells which are land
 *
 *
 * We check the squares above, below and either side (NOT DIAGONALS) to use in our
 * formula
 *
 *
 *  @param gameland talk about what gameland does
 *  @param i This is the cell we are currently measuring (rows)
 *  @param j This is the cell we are currently measuring (column)
 *  @return n.
 */

double landNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  double n = 0.0;

  if (gameLand->map[i-1][j].area == LAND) /**< checking if neighbouring cell is land */
		n+= 1.0;

	if (gameLand->map[i+1][j].area == LAND) /**< checking if neighbouring cell is land */
	 n+= 1.0;

	if (gameLand->map[i][j-1].area == LAND) /**< checking if neighbouring cell is land */
	 n+= 1.0;

	if (gameLand->map[i][j+1].area == LAND) /**< checking if neighbouring cell is land */
	 n+= 1.0;

	return n;
}

/** @fn haresNeighboursCells
 * @brief  finds the density of hares in the neighboring cells
 *
 *
 * For our formula we need to know the density of hares surrounding the square we are on
 * this function works that out for us.
 *
 *
 *  @param gameland talk about what gameland does
 *  @param i This is the cell we are currently measuring (rows)
 *  @param j This is the cell we are currently measuring (column)
 *  @return sumOfNeighbouringDensities
 */

double haresNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].hares + \
          gameLand->map[i+1][j].hares + \
          gameLand->map[i][j-1].hares + \
          gameLand->map[i][j+1].hares);
}

/** @fn haresNewValue
 * @brief  This finds the new value for the density of hares on a square
 *
 *
 *
 *
 *
 *  @param gameland talk about what gameland does
 *  @param i This is the cell we are currently measuring (rows)
 *  @param j This is the cell we are currently measuring (column)
 *  @return newValue.
 */


double haresNewValue(struct_matrix *gameLand, int i, int j) {
  double oldValue = gameLand->map[i][j].hares; /**< old value */
  double newValue = 0.0; /**< initialises new value */
  double neibarsHaresValues = haresNeighboursCells(gameLand, i, j); /**< description here */
  double neibarsLandCells = landNeighboursCells(gameLand, i, j); /**< description here */
  double randomWalk = 0.0; /**< description here */

  /**
  * Describe briefly why and what a random walk is
  */

  randomWalk = k * (neibarsHaresValues - (neibarsLandCells * oldValue));

  newValue = ((r*oldValue) - a * oldValue * gameLand->map[i][j].pumas);
  newValue += randomWalk;
  newValue *= dt;
  newValue += oldValue;

  return newValue; /**< function returns the newvalue of a cell */
}
/** @fn pumasNeighboursCells
 * @brief  Works out the density of neighbouring puma squares
 *
 *
 *
 *
 *  @param gameland talk about what gameland does
 *  @param i This is the cell we are currently measuring (rows)
 *  @param j This is the cell we are currently measuring (column)
 * @return sumOfNeighbouringDensities
 */

double pumasNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].pumas + \
          gameLand->map[i+1][j].pumas + \
          gameLand->map[i][j-1].pumas + \
          gameLand->map[i][j+1].pumas);
}

/** @fn pumasNewValue
 * @brief  Calculates the new value for the density of pumas in a cell
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @param i This is the cell we are currently measuring (rows)
 *  @param j This is the cell we are currently measuring (column)
 *  @return newValue.
 */

double pumasNewValue(struct_matrix *gameLand, int i, int j) {
  double oldValue = gameLand->map[i][j].pumas;
  double newValue = 0.0;
  double neibarsPumasValues = pumasNeighboursCells(gameLand, i, j);
  double neibarsLandCells = landNeighboursCells(gameLand, i, j);
  double randomWalk = 0.0;

  /**
  * Explain random walk is
  */

  randomWalk = l * (neibarsPumasValues - (neibarsLandCells * oldValue));

  newValue = (b*oldValue * gameLand->map[i][j].hares) - m * oldValue;
  newValue += randomWalk;
  newValue *= dt;
  newValue += oldValue;

  return newValue;
}

/** @fn mainLoop
 * @brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @param newGameLand
 *  @return Void.
 */

void mainLoop(struct_matrix *gameLand, struct_matrix *newGameLand) {

  struct_matrix *tempPtr; /**< pointer */
	int i, j; /**< for the for loops*/
	double n;

  /**
  * Explanation of the next for loop
  */

	for (i = 1; i < gameLand->x; i++) {
    for (j = 1; j < gameLand->y; j++) {
  		newGameLand->map[i][j].area = gameLand->map[i][j].area;
				if (gameLand->map[i][j].area == LAND) {
					newGameLand->map[i][j].hares = haresNewValue(gameLand, i ,j);
					newGameLand->map[i][j].pumas = pumasNewValue(gameLand, i ,j);

					/**
          * if statements to check critical densities
          */

					if(gameLand->map[i][j].hares < crit_hares_lower){gameLand->map[i][j].hares = 0.0;}
					else if(gameLand->map[i][j].hares > crit_hares_upper){gameLand->map[i][j].hares = crit_hares_upper;}
					if(gameLand->map[i][j].pumas < crit_pumas_lower){gameLand->map[i][j].pumas = 0.0;}
					else if(gameLand->map[i][j].pumas > crit_hares_upper){gameLand->map[i][j].pumas = crit_pumas_upper;}
				}
			}
		}
}

/** @fn printPPM
 * @brief  prints our densities to a 'heat' map, or coulded map
 *
 * To visually represent the changing densities, we wanted to print our
 * results onto a so called 'heat map'. This aids our analysis
 *
 *
 *  @param gameland talk about what gameland does
 *  @return Void.
 */

void printPPM(struct_matrix *gameLand) {
  static int fileID = 0;
  int i, j, k, ki /**<  Explain these  */
  FILE *fpHares, *fpPumas, *fpTogether;
  char haresFileName[30] = "haresOut/hares"; //{ '\0' };
  char pumasFileName[30] = "pumasOut/pumas";
  char togetherFileName[30] = "togetherOut/together";
  fileID++;

  /**
  *Explain what this next bit of code does
  */

  sprintf(haresFileName, "%s%d.ppm", haresFileName, fileID);
  fpHares=fopen(haresFileName, "w");

  sprintf(pumasFileName, "%s%d.ppm", pumasFileName, fileID);
  fpPumas=fopen(pumasFileName, "w");

  sprintf(togetherFileName, "%s%d.ppm", togetherFileName, fileID);
  fpTogether=fopen(togetherFileName, "w");

  /**
  * What is being printed?
  */

  fprintf(fpHares, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2); // width = 400, height = 400
  fprintf(fpPumas, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);
  fprintf(fpTogether, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);

  /**
  * Explain what happens in the next for loop
  */

  for (i = 1; i < (gameLand->x)-1; i++)
  {
    for (j = 1; j < (gameLand->y)-1; j++) {
/**
*      for (k = 0; k < 8; k++) {
*/
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

/** @fn printMapToImg
* @brief  prints our densities to a 'heat' map, or coloured map
*
* To visually represent the changing densities, we wanted to print our
* results onto a so called 'heat map'. This aids our analysis
*
 *
 *  @param gameland talk about what gameland does
 *  @param i This is the cell we are currently measuring (rows)
 *  @param j This is the cell we are currently measuring (column)
 *  @return newValue.
 */

void printMapToImg(struct_matrix *gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j, k, ki;
  FILE *fp; /**<  Explain this   */
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d.ppm", fileName, fileID);

  fp=fopen(fileName, "w");

  fprintf(fp, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);

  /**
  * Explain this for loop
  */

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

/** @fn printMap
 * @brief  prints our densities to a 'heat' map, or coloured map
 *
 * To visually represent the changing densities, we wanted to print our
 * results onto a so called 'heat map'. This aids our analysis
 *
 *
 * @param *gameLand
 * @param intfilename[]
 * @return Void.
 */

void printMap(const struct_matrix *gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j;
  FILE *fp; /**<  What does this do?  */
  char fileName[30] = { '\0' }; /**<  fileName is NULL */
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

/** @fn printHares
 * @brief  prints the hares to the heat map
 *
 *
 * A more detailed description could go here
 *
 *
 * @param *gameLand is a pointer
 *
 */

void printHares(struct_matrix *gameLand) { //}, char initFileName[30]) { //246, 15, 15
  static int fileID = 0; /**<  Explain this */
  int i, j;
  FILE *fp;
  char fileName[30] = { "haresValues/hares.txt" }; /**<  For output */

  fileID++;

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");

/**
* Explain this for loop
*/

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

/** @fn printPumas
 * @brief  prints the pumas to the heat map
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

void printPumas(struct_matrix *gameLand) { //}, char initFileName[30]) { //246, 15, 15
  static int fileID = 0; /**<  What is this?  */
  int i, j;
  FILE *fp;
  char fileName[30] = { "pumasValues/pumas.txt" };

  fileID++;

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");

  /**
  *Explain this loop
  */

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

/** @fn Main
 * @brief  This utilises all our other functions to give us the desired output
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
  struct_matrix *gameLand; /**<  pointer  */
	struct_matrix *newGameLand; /**<  pointer  */
  struct_cell **swap; /**<  pointer  */

  int i, j;
  double t; /**<  What is t for   */
  srand(time(NULL)); /**<  What is going on here?  */

  //Check if argv 2 exist
  if (argc != 2)
  {
    fprintf(stderr, "No input file\n");
    return -1;
  }

  /**
  *  Explain this shizzle
  */


  //get input file name
  inputFile = strdup(argv[1]);
  if (inputFile == NULL)
  {
    fprintf(stderr, "\\> function srtdup fail to allocate space and return \
            NULL.\n");
    return -1;
  }

  /**
  *  Explain this shizzle
  */

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
  free(inputFile); /**<  Frees inputfile  */

//  printMap(gameLand, "rawMap.txt");

//  printMapToImg(gameLand, "theMap");
//  printMap(gameLand, "theMap");

/**
* @brief calls on all the functions to be carried out at each  incremented time step (0.4)
*/

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

  /**
  free programme matrix
  */
  free_map(gameLand); /**<  Freeing memory gameLand  */
  free_map(newGameLand); /**<  Freeing memory newGameLand */
  //TODO check that
//  free(gameLand);

  printf("Reach the end\n");
  return 0;
}
