#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double dt = 0.4;
double r = 0.08;
double a = 0.04;
double b = 0.02;
double m = 0.06;
double k = 0.2;
double l = 0.2;
double T = 5.0;
double crit_hares_lower = 0.1;
double crit_hares_upper = 5.0;
double crit_pumas_lower = 0.1;
double crit_pumas_upper = 5.0;

enum enum_area{LAND, WATER};

typedef struct cell {
  enum enum_area area;
  double pumas;
  double hares;
} struct_cell;

typedef struct matrix {
  int x;
  int y;
  struct_cell **map;
} struct_matrix;

struct_cell **dynamic_alloc_map(int x, int y) {

  struct_cell **map;
  int i;

  map = (struct_cell**) malloc(sizeof(struct_cell*) * x);
  if (map == NULL)
  {
    fprintf(stderr, "malloc faild\n");
    exit(EXIT_FAILURE);
  }

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

void free_map(struct_matrix *gameLand) {
  int i;

  for (i = 0; i < gameLand->x; i++)
  {
    free(gameLand->map[i]);
  }
  //TODO check that
//  free(gameLand->map);

}

void init_map(FILE *fp, struct_matrix *gameLand) {
  char *line1 = NULL;
  char *line2 = NULL;
  char *token;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t read;
  int nx, ny, i, j;


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
            gameLand->map[i][j].pumas = 3.0;
            gameLand->map[i][j].hares = 3.0;
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

double haresNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].hares + \
          gameLand->map[i+1][j].hares + \
          gameLand->map[i][j-1].hares + \
          gameLand->map[i][j+1].hares);
}

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

double pumasNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].pumas + \
          gameLand->map[i+1][j].pumas + \
          gameLand->map[i][j-1].pumas + \
          gameLand->map[i][j+1].pumas);
}

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
				}
			}
		}
}

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

  fprintf(fpHares, "P6\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2); // width = 400, height = 400
  fprintf(fpPumas, "P6\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);
  fprintf(fpTogether, "P6\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);

  for (i = 1; i < (gameLand->x)-1; i++)
  {
    for (j = 1; j < (gameLand->y)-1; j++) {
//      for (k = 0; k < 8; k++) {
        if (gameLand->map[i][j].area == LAND) {
//          for (ki = 0; ki < 8; ki++) {
            fprintf(fpHares, "%c%c%c", (int)((gameLand->map[i][j].hares * 100)), 10, 10);
            fprintf(fpPumas, "%c%c%c", 10, (int)((gameLand->map[i][j].pumas * 100)), 10);
            fprintf(fpTogether, "%c%c%c", (int)((gameLand->map[i][j].hares * 100)), (int)(255 - (gameLand->map[i][j].pumas * 100)), 10);
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

void printHares(struct_matrix *gameLand) { //}, char initFileName[30]) { //246, 15, 15
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { "haresValues/whatEver.txt" };

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

int main(int argc, char **argv) {

  char *inputFile = NULL;
  FILE *fp =NULL;
  struct_matrix *gameLand;
	struct_matrix *newGameLand;
  struct_cell **swap;

  int i, j;
  double t;

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
		//swap the arrays
    swap = gameLand->map;
    gameLand->map = newGameLand->map;
    newGameLand->map = swap;
//    for (i = 0; i < gameLand->x; i++) {
//      memcpy(gameLand->map[i], newGameLand->map[i], sizeof(struct_cell) * gameLand->y);
//    }

//	  print_pumas(gameLand);
//	  printf("------------------------------------------------------------\n");
	  printPPM(gameLand);
	  printHares(gameLand);
	}

  //free programme matrix
  free_map(gameLand);
  free_map(newGameLand);
  //TODO chech that
//  free(gameLand);

  printf("Reach the end\n");
  return 0;
}
