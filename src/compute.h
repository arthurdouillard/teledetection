#ifndef COMPUTE_H
# define COMPUTE_H

# include <gtk/gtk.h>
# include "misc.h"


# define LargeurImage 360

void ComputeImage(guchar *img_src, int nb_col, int nb_line, guchar *img_dst);
double get_percent_clouds(guchar *img_src, guchar *img_dst, ImageFormat fmt);
int draw_clouds(guchar* img, ImageFormat fmt, unsigned* classification);

int kmeans(unsigned** pixels, ImageFormat fmt, guchar* img);
int has_changed(unsigned* a, unsigned* b);

void classify(unsigned* nb_per_class, unsigned* centers,
              unsigned* classification, unsigned** pixels,
              ImageFormat fmt);
double distance(unsigned* pixels, unsigned center);

void update_medians(unsigned* nb_per_class, unsigned* classification,
                    unsigned** medians, unsigned** pixels, ImageFormat fmt);
void update_centers(unsigned* nb_per_class, unsigned* centers,
                    unsigned** medians);

#endif /* COMPUTE_H */
