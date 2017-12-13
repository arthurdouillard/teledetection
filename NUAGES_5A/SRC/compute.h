
#include <gtk/gtk.h>

#define LargeurImage 360

void grayscale(int nb_line, int nb_col, guchar* img_src, guchar* img_dst);
void ComputeImage(guchar *img_src, int nb_col, int nb_line, guchar *img_dst);
void kmeans(int nb_line, int nb_col, unsigned** pixels,
                  size_t nb_class, size_t vec_size, guchar* img);
double cartesian_distance(unsigned* vec1, unsigned* vec2, size_t vec_size);
void sort(unsigned* values, unsigned len);
unsigned mean(unsigned* values, unsigned len);
size_t get_highest_center(unsigned** centers, size_t nb_class, size_t vec_size);
void classify_per_centers(int nb_col, int nb_line,
                          size_t nb_class, size_t vec_size,
                          unsigned** pixels, unsigned** centers,
                          unsigned* nb_per_class, unsigned* nb_per_class2,
                          unsigned* classification);