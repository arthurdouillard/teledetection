#include "misc.h"

void sort(unsigned* values, unsigned len)
{
  if (len < 2)
    return;

  unsigned pivot = values[len / 2];

  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (values[i] < pivot)
      i++;
    while (values[j] > pivot)
      j--;

    if (i >= j)
      break;

    int temp = values[i];
    values[i] = values[j];
    values[j] = temp;
  }

  sort(values, i);
  sort(values + i, len - i);
}


void copy_array(unsigned* src, unsigned* dst)
{
    for (size_t i = 0; i < NB_CLASS; i++)
        dst[i] = src[i];
}

// ---------------- FREE ------------------
void free_medians(unsigned** medians)
{
    for (size_t i = 0; i < NB_CLASS; i++)
        free(medians[i]);
    free(medians);
}

// ---------------- INIT ------------------


unsigned** init_medians()
{
    unsigned** medians = malloc(sizeof(unsigned*) * NB_CLASS);
    for (size_t i = 0; i < NB_CLASS; i++)
        medians[i] = NULL;

    return medians;
}

/*
 * Init the classification, each pixel pos has an associated class.
 * By default all pixels belongs to the class 0.
*/
unsigned* init_classification(ImageFormat fmt)
{
    int nb_pixels = (fmt.nb_line - 2) * (fmt.nb_col - 2);
    unsigned* classification = calloc(nb_pixels, sizeof(unsigned));

    return classification;
}

unsigned* init_centers()
{
  unsigned* centers = malloc(sizeof(unsigned) * NB_CLASS);
  unsigned range = 255 / (NB_CLASS - 1);

  for (size_t i = 0; i < NB_CLASS; i++)
    centers[i] = range * i;

  return centers;
}

/*
 * Init the groups of VEC_SIZE pixels including the current pixels and its
 * neighbours. Sort them to lose postion information.
*/
void init_pixels(unsigned** pixels, ImageFormat fmt, guchar* img)
{
  unsigned nb_col = fmt.nb_col;
  unsigned nb_line = fmt.nb_line;

  for (size_t x = 1; x < nb_col-1; x++)
  {
    for (size_t y = 1; y < nb_line-1; y++)
    {
      size_t pos = get_pos(x-1, y-1, nb_col-2);

      pixels[pos] = malloc(sizeof(unsigned) * VEC_SIZE);

      pixels[pos][0] = img[get_pos(x, y, nb_col) * fmt.nb_chan];     // Current pixel
      pixels[pos][1] = img[get_pos(x, y-1, nb_col) * fmt.nb_chan];   // Up
      pixels[pos][2] = img[get_pos(x+1, y, nb_col) * fmt.nb_chan];   // Right
      pixels[pos][3] = img[get_pos(x, y+1, nb_col) * fmt.nb_chan];   // Down
      pixels[pos][4] = img[get_pos(x-1, y, nb_col) * fmt.nb_chan];   // Left

      sort(pixels[pos], VEC_SIZE); // To supress position information
    }
  }
}

size_t get_pos(size_t x, size_t y, int nb_col)
{
  return x + y * nb_col;
}

void grayscale(guchar* img_src, guchar* img_dst, ImageFormat fmt)
{
  for (size_t x = 0; x < fmt.nb_col; x++)
  {
      for (size_t y = 0; y < fmt.nb_line; y++)
      {
          size_t pos = get_pos(x, y, fmt.nb_col) * fmt.nb_chan;

          int v = 0;
          for (size_t chan = 0; chan < fmt.nb_chan; chan++)
            v += img_src[pos+chan];
          v /= fmt.nb_chan;

          for (size_t chan = 0; chan < fmt.nb_chan; chan++)
            img_dst[pos+chan] = v;
      }
  }
}
