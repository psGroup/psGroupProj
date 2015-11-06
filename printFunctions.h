/** \file printFunctions.h
 *  \brief Header file for printing functions
 *
 *  \author B083194
 *  \author B084292
 *  \author B082906
 *  \author B088321
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H


#include "dataStructiars.h"


int increace(float one, float two);

/** \fn printPPM
 * \brief  Prints ppms files
 *
 *
 */

void printPPM(struct_matrix *gameLand/**<[in] Explain */, char *hares_directory/**<[in] Explain */,
					 char *pumas_directory/**<[in] Explain */, char *together_directory/**<[in] Explain */, configurations configs/**<[in] Explain */);

/** \fn printMapToImg
 * \brief  Prints entire map area
 *
 *
 */

void printMapToImg(struct_matrix *gameLand, char initFileName[30]);

/** \fn printLandAvg
 * \brief  Print average hares and pumas densities over land grid points
 *
 *
 */


void printLandAvg(FILE *fp, double t, double avgHaresLand, double avgPumasLand);

/** \fn printGridAvg
 * \brief  Print average hares and pumas densities over entire grid.
 *
 *
 *
 */

void printLandAvg(FILE *fp, double t, double avgHaresGrid, double avgPumasGrid);
#endif
