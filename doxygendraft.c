/** @file doxygen.c
 *  @brief Puma and hare density
 *
 *  This contains code for determining the number of hares and
 *  Pumas in a grid. There will be water present on the grid, and
 *  in water squares, no important animals live.
 *
 *  @author Karl J. Delargy (kdelargy)
 *  @author Brian Flynn (bflynn)
 *  @author Killian Tattan (ktattan)
 *  @author Angelos Stylianides (astyliandes)
 *  @bug No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//! Variables
/*!
The first 8 variables were given to us before we started
The variables starting with 'crit' were levels we deemed a critical point where above
or below these we send the value straight to zero
*/


double dt = 0.4;/**< change in time*/
double r = 0.08; /**< birthrate of hares */
double a = 0.04; /**< the predation rate of pumas */
double b = 0.02; /**< birth rate of hares per hare eaten */
double m = 0.06; /**< the puma morality rate */
double k = 0.2; /**< the diffusion rates for hares */
double l = 0.2;/**< diffusion rate for pumas */
double T = 5.0; /**< Time step */
double crit_hares_lower = 0.1;
double crit_pumas_lower = 0.1;

/** @enum enum_area
    @brief Enum.

    An enum where land is set to have a value of 1 and water has a value of zero.
*/


enum enum_area{LAND, WATER};

/*! \var typedef struct cell
    \typedef struct cell .

    Details.
*/

/*! \struct cell
    \A struct.

    We are making our cells so that they contain 4 things, whether they are land or water
    , the density of pumas and the density of hares.
*/


typedef struct cell {
  /*@{*/
  enum enum_area area; /**< 1D */
  double pumas; /**< 2D */
  double hares; /**< 3D */
  /*@}*/
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
  /*@{*/
  int x; /**< 1D */
  int y; /**< 2D */
  struct_cell **map; /**< 3D */
  /*@}*/
} struct_matrix;

/** @brief  brief description of what the fucntion does
 *
 *
 *
 *
 *
 *  @param x dimesnions of matrix
 *  @param y dimesnions of matrix
 *  @return map.
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

/** @brief  brief description of what the fucntion does
 *
 *
 *
 *
 *
 *  @param gameland talk about what gameland does
 *  @return Void.
 */

void free_map(struct_matrix *gameLand) {
  int i;
  for (i = 0; i < gameLand->x; i++)
  {
    free(gameLand->map[i]);
  }
  free(gameLand->map);
}

/** @brief  brief description of what the fucntion does
 *
 *
 *
 * This one is quite big so I think we should explain this one
 * A little bit more
 *
 *  @param gameland talk about what gameland does
 *  @return Void.
 */

void init_map(FILE *fp, struct_matrix *gameLand) {
  char *line1 = NULL; /**< describe variables here */
  char *line2 = NULL; /**< describe variables here */
  char *token; /**< describe variables here */
  char *saveptr = NULL; /**< describe variables here */
  size_t len = 0; /**< describe variables here */
  ssize_t read; /**< describe variables here */
  int nx, ny, i, j; /**< describe variables here */

/**
* @brief talk awhat happens next
*/

  if ((read = getline(&line1, &len, fp)) != -1)
  {
    token = strtok_r(line1, " ", &saveptr);
    nx = atoi(token);
    token = strtok_r(NULL, " ", &saveptr);
    ny = atoi(token);
  }
  free(line1);

  (*gameLand).x = nx;
  (*gameLand).y = ny;
  gameLand->map = dynamic_alloc_map(nx, ny);

  /**
  * @brief talk awhat happens next
  */

  i = j = 0;
  while((read = getline(&line2, &len, fp)) != -1)
  {
    j = 0;
    for (token = strtok_r(line2, " ", &saveptr); \
         token != NULL; \
         token = strtok_r(NULL, " ", &saveptr))
    {
      if (j < ny) {
        if (atoi(token) == 1)
        {
            (gameLand->map)[i][j].area = LAND;
            (gameLand->map)[i][j].pumas = 3.0;
            (gameLand->map)[i][j].hares = 3.0;
            j++;
        }
        else
        {
            (gameLand->map)[i][j].area = WATER;
            (gameLand->map)[i][j].pumas = 0.0;
            (gameLand->map)[i][j].hares = 0.0;
            j++;
        }
      }
    }
    i++;

  }

  free(line2);
}

/** @brief  brief description of what the fucntion does
 *
 *
 *
 *
 *
 *  @param gameland talk about what gameland does
 *  @param gameland talk about what initFileName[30] does
 *  @return Void.
 */

void printMap(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0; /**< describe variables here */
  int i, j;
  FILE *fp;
  char fileName[30] = { '\0' };
  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d \0", fileName, fileID);

  fp=fopen(fileName, "w");

  for (i = 0; i < gameLand.x; i++)
  {
    for (j = 0; j < gameLand.y; j++) {
      fprintf(fp, "%d ", (gameLand.map[i][j].area == LAND) ? 1 : 0 );
    }
    fprintf(fp, "\n");
  }

  fclose(fp);

}


/** @brief  brief description of what the fucntion does
 *
 *
 * This one could do with a big description too
 * Angelos youre going to have to explain this one to me a bit
 *
 *  @param gameland talk about what gameland does
 *  @param gameland talk about what initFileName[30] does
 *  @return Void.
 */


void printMapToImg(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0;  /**< describe variables here */
  int i, j, k, ki;  /**< describe variables here */
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d.ppm\0", fileName, fileID);

  fp=fopen(fileName, "w");  /**< describe variables here */

  fprintf(fp, "P6\n%d %d 255\n", gameLand.x*8, gameLand.y*8);

  for (i = 0; i < gameLand.x; i++) {
    for (k = 0; k < 8; k++) {
      for (j = 0; j < gameLand.y; j++) {
        if (gameLand.map[i][j].area == LAND) {
          for (ki = 0; ki < 8; ki++) fprintf(fp, "%c%c%c", 97, 64, 27);
        }
        else {
          for (ki = 0; ki < 8; ki++) fprintf(fp, "%c%c%c", 41, 73, 178); //19, 19, 228);
        }
      }
    }
  }
  fclose(fp);
}


/** @brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @param gameland talk about what initFileName[30] does
 *  @return Void.
 */

void printHares(struct_matrix gameLand, char initFileName[30]) { //246, 15, 15
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d\0", fileName, fileID);

  fp=fopen(fileName, "w");

  for (i = 0; i < gameLand.x; i++)
  {
    for (j = 0; j < gameLand.y; j++) {
      fprintf(fp, "%.2f ", gameLand.map[i][j].hares);
//      fprintf(fp, "%c %c %c", gameLand.map[i][j].hares * 10, 10, 10);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

}


/** @brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @param gameland talk about what initFileName[30] does
 *  @return Void.
 */


void printPumas(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d\0", fileName, fileID);

  fp=fopen(fileName, "w");

  for (i = 0; i < gameLand.x; i++)
  {
    for (j = 0; j < gameLand.y; j++) {
      fprintf(fp, "%.2f ", gameLand.map[i][j].pumas);
//      fprintf(fp, "%c %c %c", 255 - (gameLand.map[i][j].hares * 10), 10, 10);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

}


/** @brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @param gameland talk about what initFileName[30] does
 *  @return Void.
 */


void printPPM(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j, k, ki;
  FILE *fpHares, *fpPumas, *fpTogether;
  char haresFileName[30] = "haresOut/output"; //{ '\0' };  /**< describe variables here */
  char pumasFileName[30] = "pumasOut/hares";               /**< describe variables here */
  char togetherFileName[30] = "togetherOut/together";      /**< describe variables here */
  fileID++;

  sprintf(haresFileName, "%s%d.ppm\0", haresFileName, fileID);
  fpHares=fopen(haresFileName, "w");

  sprintf(pumasFileName, "%s%d.ppm\0", pumasFileName, fileID);
  fpPumas=fopen(pumasFileName, "w");

  sprintf(togetherFileName, "%s%d.ppm\0", togetherFileName, fileID);
  fpTogether=fopen(togetherFileName, "w");

  fprintf(fpHares, "P6\n%d %d 255\n", gameLand.x, gameLand.y); // width = 400, height = 400
  fprintf(fpPumas, "P6\n%d %d 255\n", gameLand.x, gameLand.y);
  fprintf(fpTogether, "P6\n%d %d 255\n", gameLand.x, gameLand.y);

  for (i = 0; i < gameLand.x; i++)
  {
    for (j = 0; j < gameLand.y; j++) {
//      for (k = 0; k < 8; k++) {
        if (gameLand.map[i][j].area == LAND) {
//          for (ki = 0; ki < 8; ki++) {
            fprintf(fpHares, "%c%c%c", (int)((gameLand.map[i][j].hares * 100)), 10, 10);
            fprintf(fpPumas, "%c%c%c", 10, (int)((gameLand.map[i][j].pumas * 100)), 10);
            fprintf(fpTogether, "%c%c%c", (int)((gameLand.map[i][j].hares * 100)), (int)(255 - (gameLand.map[i][j].pumas * 100)), 10);
//          }
        }
        else {
//          for (ki = 0; ki < 8; ki++) {
            fprintf(fpHares, "%c%c%c", 41, 73, 178);
            fprintf(fpPumas, "%c%c%c", 41, 73, 178);
            fprintf(fpTogether, "%c%c%c", 41, 73, 178);
//          }
        }
//      }
    }
  }
  fclose(fpHares);
  fclose(fpPumas);
  fclose(fpTogether);
}

/** @brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @param i talk about what i does
 *  @param j talk about what j does
 *  @return n.
 */

double neighboursCells(struct_matrix gameLand, int i, int j) {
	double n = 0.0;
	//TODO check boundarys

	if (gameLand.map[i-1][j].area == LAND)
		n+= 1.0;

	if (gameLand.map[i+1][j].area == LAND)
	 n+= 1.0;

	if (gameLand.map[i][j-1].area == LAND)
	 n+= 1.0;

	if (gameLand.map[i][j+1].area == LAND)
	 n+= 1.0;

	return n;
}

/** @brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  @param gameland talk about what gameland does
 *  @return newGameLand.
 */


struct_matrix mainLoop(struct_matrix gameLand) {

	struct_matrix newGameLand;
	int i, j;
	double n;
	newGameLand.x = gameLand.x; /**< describe variables here */
	newGameLand.y = gameLand.y; /**< describe variables here */

	newGameLand.map = dynamic_alloc_map(newGameLand.x, newGameLand.y); /**< describe variables here */

		for (i = 0; i < gameLand.x; i++) {
			for (j = 0; j < gameLand.y; j++) {
//				printf("-->%d - %d <--\n", i, j);
				newGameLand.map[i][j].area = gameLand.map[i][j].area;
				if (newGameLand.map[i][j].area == LAND) {
					n = neighboursCells(newGameLand, i, j);

            /**
            *Give a description of the equations etc, they are fairly complicated so this
            *can be done tomorroe
            */


	newGameLand.map[i][j].hares = gameLand.map[i][j].hares + dt * (r * gameLand.map[i][j].hares - \
														 a * gameLand.map[i][j].hares * gameLand.map[i][j].pumas + k * \
														 (gameLand.map[i-1][j].hares + gameLand.map[i+1][j].hares + gameLand.map[i][j-1].hares \
														 + gameLand.map[i][j-1].hares + gameLand.map[i][j+1].hares - n * gameLand.map[i][j].hares));

	newGameLand.map[i][j].pumas = gameLand.map[i][j].pumas + dt * ( b * gameLand.map[i][j].hares * \
														 gameLand.map[i][j].pumas - m * gameLand.map[i][j].pumas + l * \
														 (gameLand.map[i-1][j].pumas + gameLand.map[i+1][j].pumas + \
														 gameLand.map[i][j-1].pumas + gameLand.map[i][j].pumas - n*gameLand.map[i][j].pumas));
//				   printf("%.2f - %.2f\n", newGameLand.map[i][j].hares, newGameLand.map[i][j].pumas);
				}
			}
		}

	return newGameLand;

}


/** @brief  brief description of what the main fucntion does
 *
 *
 * A more detailed description could go here, not sure if
 * the arguments go in here for the main function, its prob better to leave them out
 *
 *
 *  @return 0.
 */


int main(int argc, char **argv) {

  char *inputFile = NULL;
  FILE *fp =NULL;
  struct_matrix gameLand;
  int i, j;
  double t;
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
  //printf("\\> input file -->%s<--\n", inputFile);

  fp = fopen(inputFile, "r+");
  if (fp == NULL)
  {
    fprintf(stderr, "\\> function fopen fail to open the file: %s\n", \
            inputFile);
    return -1;
  }

  init_map(fp, &gameLand);

  fclose(fp);

	for (t = 0.0;  t < 500.0; t += dt) {
		gameLand = mainLoop(gameLand);
		//printf("is here\n");
//	  print_pumas(gameLand);
//	  printf("------------------------------------------------------------\n");
//  	printHares(gameLand, "output_folder/hares_output.txt");
//	  printPumas(gameLand, "output_folder/pumas_output.txt");
	  printPPM(gameLand, "output_folder/output");

	}

//print
//  printMap(gameLand, "JustAName");

//  printMapToImg(gameLand, "angelos");
  free_map(&gameLand);

  free(inputFile);

  printf("Reach the end\n");
  return 0;
}
