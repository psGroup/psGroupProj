/** \file printFunctions.c
 *  \brief Functions that print all outsputs
 *
 *
 *  \author B083194
 *  \author B084292
 *  \author B082906
 *  \author B088321
 *  \date 06/11/12
 *  \bug No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "printFunctions.h"

/** \fn printPPM
 * \brief  prints PPM files after grid is updated
 *
 *
 */

void printPPM(struct_matrix *gameLand, char *hares_directory, 
					char *pumas_directory, char *together_directory, configurations configs) {
  
   static int fileID = 0;
  int i, j;
   
  double scale_pumas = (256.0 - configs.min_colour)/configs.crit_pumas_upper;
  double scale_hares = (256.0 - configs.min_colour)/configs.crit_hares_upper;
 
  FILE *fpHares, *fpPumas, *fpTogether;
  char haresFileName[30] = "haresPPM/hares"; //{ '\0' };
  char pumasFileName[30] = "pumasPPM/pumas";
  char togetherFileName[30] = "togetherPPM/together";
  fileID++;

  sprintf(haresFileName, "%s/%s%d.ppm", hares_directory, "hares", fileID);
  fpHares=fopen(haresFileName, "w");	
  
	if(fpHares==NULL){
		printf("Hares directory is null \n");
	}

  sprintf(pumasFileName, "%s/%s%d.ppm", pumas_directory, "pumas", fileID);
  fpPumas=fopen(pumasFileName, "w");
	if(fpPumas ==NULL){
		printf("Pumas directory is null \n");
	}

  sprintf(togetherFileName, "%s/%s%d.ppm", together_directory, "hares", fileID);
  fpTogether=fopen(togetherFileName, "w");	
 	if(fpTogether==NULL){
		printf("Together directory is null \n");
	}

  fprintf(fpHares, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2); 
  fprintf(fpPumas, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);
  fprintf(fpTogether, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);


  int pumas_ppm;
  int hares_ppm;

  for (i = 1; i < (gameLand->x)-1; i++)
  {
    for (j = 1; j < (gameLand->y)-1; j++) {
        if (gameLand->map[i][j].area == LAND) {
        	/**
        	 * Scaling factor to output ppms
        	 */
            pumas_ppm = (int)(((gameLand->map[i][j].pumas * scale_pumas)) + configs.min_colour - 20);
            hares_ppm = (int)(((gameLand->map[i][j].hares * scale_hares)) + configs.min_colour - 20);
            //printf("hares_ppm %d", hares_ppm);

            fprintf(fpHares, "%d %d %d\t", pumas_ppm, 0, 0);
            fprintf(fpPumas, "%d %d %d\t", 0, hares_ppm, 0);
            fprintf(fpTogether, "%d %d %d\t", pumas_ppm, hares_ppm, 0);

        }
        else {
            fprintf(fpHares, "%d %d %d\t", 41, 73, 178);
            fprintf(fpPumas, "%d %d %d\t", 41, 73, 178);
            fprintf(fpTogether, "%d %d %d\t", 41, 73, 178);
        }
    }
    fprintf(fpHares, "\n");
    fprintf(fpPumas, "\n");
    fprintf(fpTogether, "\n");
  }
  fclose(fpHares);
  fclose(fpPumas);
  fclose(fpTogether);
}



void printMapToImg(struct_matrix *gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { '\0' };

  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d.ppm", fileName, fileID);

  fp=fopen(fileName, "w");
	if(fp == NULL) {
		fprintf(stderr, "\\> fopen failed within printMapToImg function\n");
		exit(EXIT_FAILURE);
	}
  fprintf(fp, "P3\n%d %d\n255\n", gameLand->y - 2, gameLand->x - 2);

  for (i = 1; i < (gameLand->x - 1); i++) {
    for (j = 1; j < (gameLand->y - 1); j++) {
      if (gameLand->map[i][j].area == LAND) {
        fprintf(fp, "%d %d %d\t", 0, 0, 0);
      }
      else {
        fprintf(fp, "%d %d %d\t", 41, 73, 178);
      }
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}

/** \fn printMap
 * \brief Print the entire map
 *
 */

void printMap(const struct_matrix *gameLand, char initFileName[30]) {
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { '\0' };
  fileID++;

  strcpy(fileName, initFileName);

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");
	if(fp == NULL) {
		fprintf(stderr, "\\> fopen failed within printMap function\n");
		exit(EXIT_FAILURE);
	}

   for (i = 1; i < (gameLand->x - 1); i++) {
    for (j = 1; j < (gameLand->y - 1); j++)
      fprintf(fp, "%d ", (gameLand->map[i][j].area == LAND) ? 1 : 0 );
    fprintf(fp, "\n");
  }

  fclose(fp);

}

/** \fn printHares
 * \brief  Print hares density
 *
 */

void printHares(struct_matrix *gameLand) {
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { "haresDensity/hares.txt" };

  fileID++;

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");
	if(fp == NULL) {
			fprintf(stderr, "\\> fopen failed within printHares function\n");
			exit(EXIT_FAILURE);
		}
  for (i = 0; i < gameLand->x; i++)
  {
    for (j = 0; j < gameLand->y; j++) {
      fprintf(fp, "%.2f ", gameLand->map[i][j].hares);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

}

/** \fn printPumas
 * \brief  Print pumas density
 *
 */

void printPumas(struct_matrix *gameLand) {
  static int fileID = 0;
  int i, j;
  FILE *fp;
  char fileName[30] = { "pumasDensity/pumas.txt" };

  fileID++;

  sprintf(fileName, "%s%d", fileName, fileID);

  fp=fopen(fileName, "w");
	if(fp == NULL) {
		fprintf(stderr, "\\> fopen failed within printPumas function\n");
		exit(EXIT_FAILURE);
	}

  for (i = 0; i < gameLand->x; i++)
  {
    for (j = 0; j < gameLand->y; j++) {
      fprintf(fp, "%.2f ", gameLand->map[i][j].pumas);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

}

/** \fn printLandAvg
 * \brief  Print the average densities of hares and pumas over land grid points
 *
 *
 */

void printLandAvg(FILE *fp, double t, double avgHaresLand, double avgPumasLand) {
  fprintf(fp, "%8.2f%24.2f%24.2f\n", t, avgHaresLand, avgPumasLand);

}

/** \fn printGridAvg
 * \brief  Print the average densities of hares and pumas over the entire grid
 *
 */

void printGridAvg(FILE *fp, double t, double avgHaresGrid, double avgPumasGrid) {
  fprintf(fp, "%8.2f%24.2f%24.2f\n", t, avgHaresGrid, avgPumasGrid);

}
