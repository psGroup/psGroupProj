#ifndef DATA_STRUCTIARS_H
#define DATA_STRUCTIARS_H

#include <stdio.h>
#include "global_values.h"

/** \enum enum_area
 *   \brief  An enum where land is set to have a value of 1 and water has a value of zero.
*/

enum enum_area{LAND, WATER};

/*! \var typedef struct cell
    \typedef struct cell .

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

/*! \var typedef struct matrix
    \typedef struct matrix .

   Generating a matrix of where the elements are of the type of the struct we have defined
   i.e contain information about land/water, hare and puma densities
*/

typedef struct matrix {
  /*\{*/
  int x; /**< 1D */
  int y; /**< 2D */
  struct_cell **map; /**< 3D */
  /*\}*/
} struct_matrix;

/** \fn dynamic_alloc_map
 * 
 * dynamically allocating memory to a map 
 * where we will input values for land/water, 
 * hare and puma densities
 *
 *  \param x dimesnions of matrix (number of rows)
 *  \param y dimesnions of matrix (number of columns)
 *  \return map.
 */
struct_cell **dynamic_alloc_map(int x, int y);

/** * \fn free_map
 *\brief  deallocates memory assigned previously
 *
 *  \param gameland Gameland is our map of land with hare and puma information
 *  \return Void.
 */
void free_map(struct_matrix *gameLand);

/** \fn init_map
 * \brief  brief description of what the fucntion does
 ** \brief  Initialise map for from given matrix
 *
 * When we pass in a binary file indicating land/water,
 * this function turns it into a matrix of structs, each of 
 * which contains an element for land/water, a random density of hares,
 * and a random density of pumas. 
 * We also add halo data: an extra edge of all water, so that our functions all work correctly. 
 *
 *  \return Void.
 */

int init_map(FILE *fp, struct_matrix *gameLand, configurations configs);

#endif /* DATA_STRUCTIARS */
