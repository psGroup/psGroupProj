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
  free(gameLand->map);
}

void init_map(FILE *fp, struct_matrix *gameLand) {
  char *line1 = NULL;
  char *line2 = NULL;
  char *token;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t read;
  int nx, ny, i, j;


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
    //if (i >
  }

  free(line2);
}
void printMap(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0;
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

void printMapToImg(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j, k, ki;
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d.ppm\0", fileName, fileID);

  fp=fopen(fileName, "w");

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

void printPPM(struct_matrix gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j, k, ki;
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d.ppm\0", fileName, fileID);

  fp=fopen(fileName, "w");

  fprintf(fp, "P6\n%d %d 255\n", gameLand.x, gameLand.y); // width = 400, height = 400

  for (i = 0; i < gameLand.x; i++)
  {
    for (j = 0; j < gameLand.y; j++) {
//      for (k = 0; k < 8; k++) {
        if (gameLand.map[i][j].area == LAND) {
//          for (ki = 0; ki < 8; ki++) {
            fprintf(fp, "%c%c%c", (int)(255 - (gameLand.map[i][j].pumas * 100)), 10, 10);
//          }
        }
        else {
//          for (ki = 0; ki < 8; ki++) { 
            fprintf(fp, "%c%c%c", 41, 73, 178);
//          }
        }
//      }
    }
  }
  fclose(fp);
  
}

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

struct_matrix mainLoop(struct_matrix gameLand) {

	struct_matrix newGameLand;
	int i, j;
	double n;
	newGameLand.x = gameLand.x;
	newGameLand.y = gameLand.y;

	newGameLand.map = dynamic_alloc_map(newGameLand.x, newGameLand.y);

		for (i = 0; i < gameLand.x; i++) {
			for (j = 0; j < gameLand.y; j++) {
//				printf("-->%d - %d <--\n", i, j);
				newGameLand.map[i][j].area = gameLand.map[i][j].area;
				if (newGameLand.map[i][j].area == LAND) {
					n = neighboursCells(newGameLand, i, j);
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
