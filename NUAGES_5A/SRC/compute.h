
#include <gtk/gtk.h>

#define LargeurImage 360

void grayscale(int nb_line, int nb_col, guchar* img_src, guchar* img_dst);
void ComputeImage(guchar *img_src, int nb_col, int nb_line, guchar *img_dst);
int** kmeans(int nb_line, int nb_col, unsigned** pixels);
double cartesian_distance(unsigned* vec1, unsigned* vec2, size_t vec_size);
void sort(unsigned* values, unsigned len);