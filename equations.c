#include "equations.h"

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

double haresNewValue(struct_matrix *gameLand, int i, int j, configurations configs) {
  double oldValue = gameLand->map[i][j].hares;
  double newValue = 0.0;
  double neibarsHaresValues = haresNeighboursCells(gameLand, i, j);
  double neibarsLandCells = landNeighboursCells(gameLand, i, j);
  double randomWalk = 0.0;

  //randomWalk
  randomWalk = configs.k * (neibarsHaresValues - (neibarsLandCells * oldValue));

  newValue = ((configs.r*oldValue) - configs.a * oldValue * gameLand->map[i][j].pumas);
  newValue += randomWalk;
  newValue *= configs.dt;
  newValue += oldValue;

  return newValue;
}

double pumasNeighboursCells(const struct_matrix *gameLand, int i, int j) {
  return (gameLand->map[i-1][j].pumas + \
          gameLand->map[i+1][j].pumas + \
          gameLand->map[i][j-1].pumas + \
          gameLand->map[i][j+1].pumas);
}

double pumasNewValue(struct_matrix *gameLand, int i, int j, configurations configs) {
  double oldValue = gameLand->map[i][j].pumas;
  double newValue = 0.0;
  double neibarsPumasValues = pumasNeighboursCells(gameLand, i, j);
  double neibarsLandCells = landNeighboursCells(gameLand, i, j);
  double randomWalk = 0.0;

  //randomWalk
  randomWalk = configs.l * (neibarsPumasValues - (neibarsLandCells * oldValue));

  newValue = (configs.b*oldValue * gameLand->map[i][j].hares) - configs.m * oldValue;
  newValue += randomWalk;
  newValue *= configs.dt;
  newValue += oldValue;

  return newValue;
}

void mainLoop(struct_matrix *gameLand, struct_matrix *newGameLand, double *totalHares, double *totalPumas, configurations configs) {

	int i, j;

	for (i = 1; i < gameLand->x; i++) {
    for (j = 1; j < gameLand->y; j++) {
  		newGameLand->map[i][j].area = gameLand->map[i][j].area;
				if (gameLand->map[i][j].area == LAND) {
					newGameLand->map[i][j].hares = haresNewValue(gameLand, i ,j, configs);
					newGameLand->map[i][j].pumas = pumasNewValue(gameLand, i ,j, configs);

          // calculate average over grid
          (*totalHares) += newGameLand->map[i][j].hares;
          (*totalPumas) += newGameLand->map[i][j].pumas;

					// if statements to check critical densities
					if (gameLand->map[i][j].hares < configs.crit_hares_lower) {
						gameLand->map[i][j].hares = 0.0;
					}
					else if (gameLand->map[i][j].hares > configs.crit_hares_upper) {
						gameLand->map[i][j].hares = configs.crit_hares_upper;
					}
					if (gameLand->map[i][j].pumas < configs.crit_pumas_lower) {
						gameLand->map[i][j].pumas = 0.0;
					}
					else if(gameLand->map[i][j].pumas > configs.crit_hares_upper) {
						gameLand->map[i][j].pumas = configs.crit_pumas_upper;
					}
				}
			}
		}
}
