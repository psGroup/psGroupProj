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
