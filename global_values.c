/** \file global_values.c
 *  \brief Setting up all parameters used in equations and print statements
 *
 *
 *  \author B083194
 *  \author B084292
 *  \author B082906
 *  \author B088321
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#include "global_values.h"
#include <string.h>
#include <stdlib.h>
configurations parse_configs(FILE *fp) {
  configurations config;
  char *line = NULL;
  char *token;
  char *saveptr = NULL;
  size_t len = 0;
  ssize_t read;
  
  while((read = getline(&line, &len, fp)) != -1)
  {
    
    token = strtok_r(line, " ", &saveptr);
    if (!strcmp(token, "dt")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.dt = atof(token);
    }
    
    else if (!strcmp(token, "r"))
    {
      token = strtok_r(NULL, " ", &saveptr);
      config.r = atof(token);
    }
    else if (!strcmp(token, "a")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.a = atof(token);
    }
    else if (!strcmp(token, "b")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.b = atof(token);
    }
    else if (!strcmp(token, "m")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.m = atof(token);
    }
    else if (!strcmp(token, "k")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.k = atof(token);
    }
    else if (!strcmp(token, "l")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.l = atof(token);
    }
    else if (!strcmp(token, "T")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.T = atof(token);
    }
    else if (!strcmp(token, "crit_hares_lower")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.crit_hares_lower = atof(token);
    }
    else if (!strcmp(token, "crit_hares_upper")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.crit_hares_upper = atof(token);
    }
    else if (!strcmp(token, "crit_pumas_lower")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.crit_pumas_lower = atof(token);
    }
    else if (!strcmp(token, "crit_pumas_upper")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.crit_pumas_lower = atof(token);
    }
    else if (!strcmp(token, "min_colour")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.min_colour = atof(token);
    }
    else if (!strcmp(token, "land_number")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.land_number = atof(token);
    }
    else if (!strcmp(token, "grid_number")) {
      token = strtok_r(NULL, " ", &saveptr);
      config.grid_number = atof(token);
    }
  }
  
  return config;
  
}


void print_configs(configurations config) {
  printf("---------Configurations----------\n");
  printf("dt\t%f\n", config.dt);
  printf("r\t%f\n", config.r);
  printf("a\t%f\n", config.a);
  printf("b\t%f\n", config.b);
  printf("m\t%f\n", config.m);
  printf("k\t%f\n", config.k);
  printf("l\t%f\n", config.l);
  printf("T\t%f\n", config.T);
  printf("config.crit_hares_lower\t%f\n", config.crit_hares_lower);
  printf("config.crit_hares_upper\t%f\n", config.crit_hares_upper);
  printf("config.crit_pumas_lower\t%f\n", config.crit_hares_lower);
  printf("config.crit_pumas_upper\t%f\n", config.crit_hares_upper);
  printf("config.min_colour\t%f\n", config.crit_hares_lower);
  printf("config.land_number\t%f\n", config.land_number);
  printf("config.grid_number\t%f\n", config.grid_number);
}
