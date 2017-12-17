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
  ImageFormat fmt =
  {
    .nb_line = nb_line,
    .nb_col = nb_col,
    .nb_chan = 3
  };

  get_percent_clouds(img_src, img_dst, fmt);
}

double get_percent_clouds(guchar *img_src, guchar *img_dst, ImageFormat fmt)
{
  grayscale(img_src, img_dst, fmt);
  int nb_pixels = (fmt.nb_line - 2) * (fmt.nb_col - 2);
  unsigned** pixels = malloc(sizeof(unsigned*) * nb_pixels);
  init_pixels(pixels, fmt, img_dst);

  int nb_clouds = kmeans(pixels, fmt, img_dst);
  double percent = ((double)nb_clouds) / nb_pixels * 100;

  for (size_t x = 1; x < fmt.nb_col-1; x++)
    for (size_t y = 1; y < fmt.nb_line-1; y++)
      free(pixels[get_pos(x-1, y-1, fmt.nb_col-2)]);
  free(pixels);

  return percent;
}


int kmeans(unsigned** pixels, ImageFormat fmt, guchar* img)
{
  unsigned* centers = init_centers();
  unsigned* history_centers = malloc(sizeof(unsigned) * NB_CLASS);

  unsigned** medians = init_medians();
  unsigned* classification = init_classification(fmt);

  unsigned* nb_per_class = malloc(sizeof(unsigned) * NB_CLASS);
  unsigned* nb_per_class2 = malloc(sizeof(unsigned) * NB_CLASS);


  int loop_counter = 0;
  do // Loop of the K-means algorithm
  {
    copy_array(centers, history_centers); // Save previous mass centers

    // 1. Associate pixels to mass centers
    classify(nb_per_class, centers, classification, pixels, fmt);

    // 2. Update mass centers
    copy_array(nb_per_class, nb_per_class2);
    update_medians(nb_per_class, classification, medians, pixels, fmt);
    update_centers(nb_per_class2, centers, medians);


  } while (loop_counter++ > 100 || has_changed(centers, history_centers));

  classify(nb_per_class2, centers, classification, pixels, fmt);

  int nb_clouds = draw_clouds(img, fmt, classification);

  free(nb_per_class);
  free(nb_per_class2);
  free(centers);
  free(history_centers);
  free(classification);
  free_medians(medians);

  return nb_clouds;
}


int draw_clouds(guchar* img, ImageFormat fmt, unsigned* classification)
{
  int nb_clouds = 0;
  for (size_t x = 1; x < fmt.nb_col-1; x++)
  {
    for (size_t y = 1; y < fmt.nb_line-1; y++)
    {
      size_t pos = get_pos(x-1, y-1, fmt.nb_col-2);
      size_t class = classification[pos];

      if (class >= NB_CLASS - N_BEST)
      {
        nb_clouds++;
        size_t pos_img = get_pos(x, y, fmt.nb_col) * 3;
        img[pos_img] = 255;
        img[pos_img+1] = 0;
        img[pos_img+2] = 0;
      }
    }
  }

  return nb_clouds;
}

void update_centers(unsigned* nb_per_class, unsigned* centers,
                    unsigned** medians)
{
  for (size_t i = 0; i < NB_CLASS; i++)
  {
    sort(medians[i], nb_per_class[i]);

    if (nb_per_class[i])
      centers[i] = medians[i][nb_per_class[i] / 2];
  }
}

void update_medians(unsigned* nb_per_class, unsigned* classification,
                    unsigned** medians, unsigned** pixels, ImageFormat fmt)
{
  for (size_t i = 0; i < NB_CLASS; i++)
  {
    if (medians[i])
      free(medians[i]);

    medians[i] = malloc(sizeof(unsigned) * nb_per_class[i] * VEC_SIZE);
  }

  for (size_t x = 1; x < fmt.nb_col-1; x++)
  {
    for (size_t y = 1; y < fmt.nb_line-1; y++)
    {
      size_t pos = get_pos(x-1, y-1, fmt.nb_col-2);
      size_t class = classification[pos];

      for (size_t i = 0; i < VEC_SIZE; i++)
        medians[class][(nb_per_class[class] - 1) * VEC_SIZE + i] = pixels[pos][i];

      nb_per_class[class]--;
    }
  }
}


void classify(unsigned* nb_per_class, unsigned* centers,
              unsigned* classification, unsigned** pixels,
              ImageFormat fmt)
{
  for (size_t i = 0; i < NB_CLASS; i++)
    nb_per_class[i] = 0;

  for (size_t x = 1; x < fmt.nb_col-1; x++)
  {
    for (size_t y = 1; y < fmt.nb_line-1; y++)
    {
      size_t pos = get_pos(x-1, y-1, fmt.nb_col-2);

      unsigned best_class = 0;
      double dist = DBL_MAX;

      for (size_t i = 0; i < NB_CLASS; i++)
      {
        double new_dist = distance(pixels[pos], centers[i]);
        if (new_dist < dist)
        {
          best_class = i;
          dist = new_dist;
        }
      }

      classification[pos] = best_class;
      nb_per_class[best_class]++;
    }
  }
}

double distance(unsigned* pixels, unsigned center)
{
  double dist = 0.0;
  int b = center;

  for (size_t i = 0; i < VEC_SIZE; i++)
  {
    int a = pixels[i];
    dist += abs(a - b);
  }

  return dist;
}

int has_changed(unsigned* a, unsigned* b)
{
  for (size_t i = 0; i < NB_CLASS; i++)
    if (a[i] != b[i])
      return 1;

  return 0;
}




