#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <gtk/gtk.h>

#include "compute.h"

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto:


  But:

  Entrees:
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes � la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes � la suite des autres)


  Sortie:

  Rem:

  Voir aussi:

  ---------------------------------------*/
void ComputeImage(guchar *img_src, int nb_line, int nb_col, guchar *img_dst)
{
  grayscale(nb_line, nb_col, img_src, img_dst);

  int nb_pixels = (nb_line - 2) * (nb_col - 2);
  unsigned** pixels = malloc(sizeof(unsigned*) * nb_pixels);

  // Copy pixels neighbours
  for (size_t x = 1; x < nb_col-1; x++)
    for (size_t y = 1; y < nb_line-1; y++)
    {
      size_t pos = x + y * nb_col;

      pixels[pos] = malloc(sizeof(unsigned) * 5);

      pixels[pos][0] = img_dst[x + y * nb_col];       // Current pixel
      pixels[pos][1] = img_dst[x + (y-1) * nb_col];   // Up
      pixels[pos][2] = img_dst[(x+1) + y * nb_col];   // Right
      pixels[pos][3] = img_dst[(x + (y+1) * nb_col)]; // Down
      pixels[pos][4] = img_dst[(x-1) + y * nb_col];   // Left
    }

  unsigned** centroids = kmeans(nb_line, nb_col, pixels);

  // Free pixels neighbours
  for (size_t x = 1; x < nb_col-1; x++)
    for (size_t y = 1; y < nb_line-1; y++)
      free(pixels[x + y * nb_col]);
  free(pixels);
}

int** kmeans(int nb_line, int nb_col, unsigned** pixels)
{
  srand(time(NULL));

  size_t nb_class = 4;
  size_t vec_size = 5;
  unsigned min_val = 0;
  unsigned max_val = 255;
  int nb_pixels = (nb_line - 2) * (nb_col - 2);
  unsigned* classification = calloc(nb_pixels, sizeof(unsigned));
  unsigned** centroids = malloc(sizeof(unsigned*) * nb_class);

  // Initialize centroids
  for (size_t class_i = 0; class_i < nb_class; class_i++)
  {
    centroids[class_i] = malloc(sizeof(unsigned) * vec_size);
    for (size_t vec_i = 0; vec_i < vec_size; vec_i++)
      centroids[class_i][vec_i] = (rand() % (max_val + 1 - min_val)) + min_val;
  }

  while (1)
  {
    // Classify pixels per centroids
    for (size_t x = 1; x < nb_col-1; x++)
      for (size_t y = 1; y < nb_line-1; y++)
      {
        size_t pos = x + y * nb_col;

        size_t best_class = 0;
        double min_dist = DBL_MAX;
        for (size_t class_i = 0; class_i < nb_class; class_i++)
        {
          double dist = cartesian_distance(pixels[pos],
                                           centroids[class_i],
                                           vec_size);
          if (dist < min_dist)
          {
            min_dist = dist;
            best_class = class_i;

            if (!min_dist) // Cannot be closer to centroid
              break;
          }
        }

        classification[pos] = best_class;
      }

  }
}

double cartesian_distance(unsigned* vec1, unsigned* vec2, size_t vec_size)
{
  unsigned long c = 0;
  for (size_t vec_i = 0; vec_i < vec_size; vec_i++)
    c += pow(vec1[vec_i] - vec2[vec_i], 2);

  return sqrt(c);
}


void grayscale(int nb_line, int nb_col, guchar* img_src, guchar* img_dst)
{
  int nb_pixels = nb_line * nb_col;
  int nb_channels = 3;

  for(int i = 0; i < nb_pixels * nb_channels; i += nb_channels)
  {
    int pixel = (unsigned char)((*(img_src + i) + *(img_src + i+1) + *(img_src + i+2)) / 3);

    for(int channel_i = 0; channel_i < nb_channels; channel_i++)
      *(img_dst + i+channel_i) = pixel;
  }
}
