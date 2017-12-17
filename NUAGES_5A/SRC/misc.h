#ifndef MISC_H
# define MISC_H

# include <stddef.h>
# include <stdlib.h>
# include <gtk/gtk.h>

/*
 * All functions not directly related to the logic of the algorithm.
*/

# define NB_CLASS 8
# define VEC_SIZE 5
# define N_BEST 2

typedef struct
{
    int nb_line;
    int nb_col;
    int nb_chan;
} ImageFormat;


size_t get_pos(size_t x, size_t y, int nb_col);
void sort(unsigned* values, unsigned len);
void grayscale(guchar* img_src, guchar* img_dst, ImageFormat fmt);

unsigned* init_centers();
unsigned* init_classification(ImageFormat fmt);
unsigned** init_medians();
void init_pixels(unsigned** pixels, ImageFormat fmt, guchar* img);

void free_medians(unsigned** medians);

void copy_array(unsigned* src, unsigned* dst);

#endif /* MISC_H */