/** \file equations.h
 *  \brief Explain
 *
 *
 *  \author B083194
 *  \author B084292
 *  \author B
 *  \author B
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "dataStructiars.h"
#include "global_values.h"
/** 
 * \fn landNeighboursCells
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \return n.
 */

double landNeighboursCells(const struct_matrix *gameLand/**< [in] docs for input parameter v. */, int i/**< [in] docs for input parameter v. */, int j/**< [in] docs for input parameter v. */);

/** \fn haresNeighboursCells
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *
 *  \return Void.
 */

double haresNeighboursCells(const struct_matrix *gameLand, int i, int j);

/** \fn haresNewValue
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *  \return newValue.
 */

double haresNewValue(struct_matrix *gameLand, int i, int j, \
		configurations configs);

/** \fn pumasNeighboursCells
 * \brief  brief description of what the fucntion does
 *
 * A more detailed description could go here
 *
 *
 */

double pumasNeighboursCells(const struct_matrix *gameLand, int i, int j);
/** \fn pumasNewValue
 * \brief  brief description of what the fucntion does
 * \fn pumasNewValue
 *
 * A more detailed description could go here
 *
 *
 *  \return newValue.
 */

double pumasNewValue(struct_matrix *gameLand, int i, int j, \
                            configurations configs);

/** \fn mainLoop
 * \brief  brief description of what the fucntion does
 *
 *
 * A more detailed description could go here
 *
 *  \return Void.
 */

void mainLoop(struct_matrix *gameLand/**< [in] docs for input parameter v. */, struct_matrix *newGameLand/**< [in] docs for input parameter v. */,  double *totalHares/**< [in] docs for input parameter v. */, double *totalPumas/**< [in] docs for input parameter v. */, \
                            configurations configs);

#endif
