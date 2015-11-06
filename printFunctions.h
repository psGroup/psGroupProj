/** \file printFunctions.h
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

#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H


#include "dataStructiars.h"

/** \fn increace
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

int increace(float one/**<[in] Explain */, float two/**<[in] Explain */);

/** \fn printPPM
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

void printPPM(struct_matrix *gameLand/**<[in] Explain */, char *hares_directory/**<[in] Explain */,
					 char *pumas_directory/**<[in] Explain */, char *together_directory/**<[in] Explain */, configurations configs/**<[in] Explain */);

/** \fn printMapToImg
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

void printMapToImg(struct_matrix *gameLand, char initFileName[30]);

/** \fn printLandAvg
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */


void printLandAvg(FILE *fp, double t, double avgHaresLand, double avgPumasLand);

/** \fn printGridAvg
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *
 *
 */

void printLandAvg(FILE *fp, double t, double avgHaresGrid, double avgPumasGrid);
#endif
