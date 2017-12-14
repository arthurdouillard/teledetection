#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
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

  size_t nb_class = 2;
  size_t vec_size = 5;

  // Copy pixels neighbours
  for (size_t x = 1; x < nb_col-1; x++)
    for (size_t y = 1; y < nb_line-1; y++)
    {
      size_t pos = (x-1) + (y-1) * (nb_col-2);

      pixels[pos] = malloc(sizeof(unsigned) * 5);

      pixels[pos][0] = img_dst[x + y * nb_col];       // Current pixel
      pixels[pos][1] = img_dst[x + (y-1) * nb_col];   // Up
      pixels[pos][2] = img_dst[(x+1) + y * nb_col];   // Right
      pixels[pos][3] = img_dst[x + (y+1) * nb_col]; // Down
      pixels[pos][4] = img_dst[(x-1) + y * nb_col];   // Left

      sort(pixels[pos], vec_size);
    }

  kmeans(nb_line, nb_col, pixels, nb_class, vec_size, img_dst);

  for (size_t x = 1; x < nb_col-1; x++)
    for (size_t y = 1; y < nb_line-1; y++)
      free(pixels[(x-1) + (y-1) * (nb_col-2)]);
  free(pixels);
}

size_t get_highest_center(unsigned** centers, size_t nb_class, size_t vec_size)
{
  double largest_norm = DBL_MIN;
  size_t best_center_idx = 0;

  for (size_t i = 0; i < nb_class; i++)
  {
    double norm = 0;
    for (size_t j = 0; j < vec_size; j++)
      norm += pow(centers[i][j], 2); // No need to compute sqrt here

    if (norm > largest_norm)
    {
      norm = largest_norm;
      best_center_idx = i;
    }
  }

  return best_center_idx;
}

void kmeans(int nb_line, int nb_col, unsigned** pixels,
                  size_t nb_class, size_t vec_size, guchar* img)
{
  srand(time(NULL));

  unsigned min_val = 0;
  unsigned max_val = 255;
  int nb_pixels = (nb_line - 2) * (nb_col - 2);
  unsigned* classification = calloc(nb_pixels, sizeof(unsigned));
  unsigned* nb_per_class = calloc(nb_class, sizeof(unsigned));
  unsigned* nb_per_class2 = calloc(nb_class, sizeof(unsigned));
  unsigned** centers = malloc(sizeof(unsigned*) * nb_class);

  unsigned** medians = malloc(sizeof(unsigned*) * nb_class);
  for (size_t class_i = 0; class_i < nb_class; class_i++)
    medians[class_i] = NULL;

  // Initialize centroids
  for (size_t class_i = 0; class_i < nb_class; class_i++)
  {
    centers[class_i] = malloc(sizeof(unsigned) * vec_size);
    for (size_t vec_i = 0; vec_i < vec_size; vec_i++)
      centers[class_i][vec_i] = (rand() % (max_val + 1 - min_val)) + min_val;
  }

  int c = 0;
  while (1)
  {
    printf("Loop %d\n", c);
    for (size_t i = 0; i < nb_class; i++)
    {
      printf("Center %d\t", i);
      for (size_t j = 0; j < vec_size; j++)
        printf(" %d", centers[i][j]);
      printf("\n");
    }
    printf("\n");

    if (c > 1)
      break;
    c++;
    // -----------------------------------
    // 1. Classify pixels per mass centers
    classify_per_centers(nb_col, nb_line, nb_class, vec_size,
                         pixels, centers, nb_per_class, nb_per_class2, classification);

    // -----------------------------------
    // 2. Compute new mass centers
    for (size_t class_i = 0; class_i < nb_class; class_i++)
    {
      if (!c)
        free(medians[class_i]);
      medians[class_i] = calloc(nb_per_class[class_i], sizeof(unsigned));
    }
    // Aglomerate pixels belonging to each class
    for (size_t x = 1; x < nb_col-1; x++)
      for (size_t y = 1; y < nb_line-1; y++)
      {
        size_t pos = (x-1) + (y-1) * (nb_col-2);
        size_t class = classification[pos];

        medians[class][nb_per_class[class]-1] = mean(pixels[pos], vec_size);
        nb_per_class[class]--;
      }

    // Sort each group of pixels
    for (size_t class_i = 0; class_i < nb_class; class_i++)
    {
      sort(medians[class_i], nb_per_class2[class_i]);

      unsigned new_value;
      if (nb_per_class2[class_i] % 2 != 0) // Odd
        new_value = medians[class_i][nb_per_class2[class_i] / 2];
      else // Even
        new_value = (medians[class_i][nb_per_class2[class_i] / 2] +\
                     medians[class_i][nb_per_class2[class_i] / 2 + 1]) / 2;

      for (size_t vec_i = 0; vec_i < vec_size; vec_i++)
          centers[class_i][vec_i] = new_value;
    }

    for (size_t i = 0; i < nb_class; i++)
    {
      nb_per_class[i] = 0;
      nb_per_class2[i] = 0;
    }
  }



  size_t cloud_idx = get_highest_center(centers, nb_class, vec_size);
  printf("Cloud %d\n", cloud_idx);
  for (size_t x = 0; x < nb_col; x++)
  {
    for (size_t y = 0; y < nb_line; y++)
    {
      size_t pos_img = (x*3) + y * (nb_col * 3);
      if (!x || !y || x == nb_col-1 || y == nb_line-1)
      {
        img[pos_img] = 0;
        img[pos_img+1] = 0;
        img[pos_img+2] = 0;
        continue;
      }

      size_t pos_pixels = (x-1) + (y-1) * (nb_col-2);
      unsigned val = 0;
      if (classification[pos_pixels] == cloud_idx)
        val = 255;
      else
        val = 0;

      img[pos_img] = val;
      img[pos_img+1] = val;
      img[pos_img+2] = val;
    }
  }

  free(nb_per_class);
  free(nb_per_class2);
  free(classification);
  for (size_t class_i = 0; class_i < nb_class; class_i++)
    free(medians[class_i]);
  free(medians);
}

void classify_per_centers(int nb_col, int nb_line,
                          size_t nb_class, size_t vec_size,
                          unsigned** pixels, unsigned** centers,
                          unsigned* nb_per_class, unsigned* nb_per_class2,
                          unsigned* classification)
{
  for (size_t x = 1; x < nb_col-1; x++)
    for (size_t y = 1; y < nb_line-1; y++)
    {
      size_t pos = (x-1) + (y-1) * (nb_col-2);

      size_t best_class = 0;
      double min_dist = DBL_MAX;
      for (size_t class_i = 0; class_i < nb_class; class_i++)
      {
        double dist = cartesian_distance(pixels[pos],
                                          centers[class_i],
                                          vec_size);
        if (dist < min_dist)
        {
          min_dist = dist;
          best_class = class_i;

          if (!min_dist) // Cannot be closer to a mass center
            break;
        }
      }

      printf("%d %d\n", pos,  best_class);
      nb_per_class[best_class]++;
      nb_per_class2[best_class]++;
      classification[pos] = best_class;
    }
}

unsigned mean(unsigned* values, unsigned len)
{
  unsigned c = 0;
  for (size_t i = 0; i < len; i++)
    c += values[i];

  return c / len;
}

void sort(unsigned* values, unsigned len)
{
  if (len < 2)
    return;

  int pivot = values[len / 2];

  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (values[i] < pivot)
      i++;
    while (values[j] > pivot)
      j--;

    if (i >= j)
      break;

    int temp = values[i];
    values[i]     = values[j];
    values[j]     = temp;
  }

  sort(values, i);
  sort(values + i, len - i);
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
