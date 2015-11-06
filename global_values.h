/** \file global_values.h
 *  \brief Explain
 *
 *   explain
 *
 *  \author B083194
 *  \author B084292
 *  \author B
 *  \author B
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#ifndef GLOBAL_VALUES_H
#define GLOBAL_VALUES_H

#include <stdio.h>
/*
extern double dt;
extern double r;
extern double a;
extern double b;
extern double m;
extern double k;
extern double l;
extern double T;
extern double crit_hares_lower;
extern double crit_hares_upper;
extern double crit_pumas_lower;
extern double crit_pumas_upper;
*/

/**
*setting the colour scales for the output ppm files
*/
/*
extern double min_colour;
extern double scale_pumas;
extern double scale_hares;
*/
typedef struct configurations_struct {
  double dt;
  double r;
  double a;
  double b;
  double m;
  double k;
  double l;
  double T;
  double crit_hares_lower;
  double crit_hares_upper;
  double crit_pumas_lower;
  double crit_pumas_upper;
  double min_colour;
  double land_number;
  double grid_number;
}configurations;

configurations parse_configs(FILE *fp);

void print_configs(configurations config);

int isOnlyWiteSpace(char *str, size_t len);
#endif /* GLOBAL_VALUES */
