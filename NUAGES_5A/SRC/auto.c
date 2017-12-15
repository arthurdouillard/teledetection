#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <dirent.h>
#include "compute.h"


int check_ext(char* name)
{
    char* point;
    if((point = strrchr(name, '.')) != NULL )
        return strcmp(point, ".bmp") == 0;

    return 0;
}

void process_img(char* name)
{
    if (!check_ext(name))
        return;

    printf("%s\n", name);

    GError* error = NULL;
    GdkPixbuf* pix = gdk_pixbuf_new_from_file(name, &error);
    if (pix == NULL)
    {
        g_printerr("Error loading file: #%d %s\n", error->code, error->message);
        g_error_free(error);
        exit(1);
    }

    GdkPixbuf* pix_copy = gdk_pixbuf_copy(pix);

    guchar* img_src = gdk_pixbuf_get_pixels(pix);
    guchar* img_dst = gdk_pixbuf_get_pixels(pix_copy);

    int nb_col = gdk_pixbuf_get_width(pix);
    int nb_line = gdk_pixbuf_get_height(pix);

    double percent = get_percent_clouds(img_src, nb_line, nb_col, img_dst);
    printf("Percentage of clouds:\t%.2f\%\n", percent);
}

int main(void)
{
  DIR *d = opendir(".");
  struct dirent *dir;
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
      process_img(dir->d_name);

    closedir(d);
  }

  return(0);
}