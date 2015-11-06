/** \file global_values.h
 *  \brief Explain
 *
 *   explain
 *
*  \author B083194
 *  \author B084292
 *  \author B082906
 *  \author B088321
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#ifndef GLOBAL_VALUES_H
#define GLOBAL_VALUES_H

#include <stdio.h>
/** 
 * Declaring all parameters used throughout
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
