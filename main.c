#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum enum_area{LAND, WATER};

typedef struct cell {
  enum enum_area area;
  int pumas;
  int hares;
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
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < x; i++)
  {
    map[i] = (struct_cell*) malloc(sizeof(struct_cell) * y);
    printf("A-->%u<--\n", map[i]);
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
    printf("FF-->%u<--\n", gameLand->map[i]);
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
  printf("\\> nx: %d\tny: %d\n", nx, ny);

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
      printf("'%s'", token);
      if (atoi(token) == 1)
      {
        if (j < ny)
          (gameLand->map)[i][j++].area = LAND;
      }
      else
      {
        if (j < ny)
          (gameLand->map)[i][j++].area = WATER;
      }
//      printf("%d - %d
    }
    i++;
    if (i >
  }

  free(line2);
  printf("is here\n");
}

int main(int argc, char **argv) {

  char *inputFile = NULL;
  FILE *fp =NULL;
  struct_matrix gameLand;
  int i, j;
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
  printf("\\> input file -->%s<--\n", inputFile);

  fp = fopen(inputFile, "r+");
  if (fp == NULL)
  {
    fprintf(stderr, "\\> function fopen fail to open the file: %s\n", \
            inputFile);
    return -1;
  }

  init_map(fp, &gameLand);

  fclose(fp);

//print
  for (i = 0; i < gameLand.x; i++) {
    for(j = 0; j < gameLand.y; j++) {
      printf("%d ", gameLand.map[i][j].area == WATER ? 0 : 1);
    }
    printf("\n");
  }

  free_map(&gameLand);

  free(inputFile);

  printf("Reach the end\n");
  return 0;
}
