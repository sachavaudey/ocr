#ifndef UI_MAIN_H
#define UI_MAIN_H

#ifndef GTK_H
#define GTK_H
#include <gtk/gtk.h>
#endif

#ifndef SDL_H
#define SDL_H
#include <SDL2/SDL.h>
#endif

#ifndef SDL_IMAGE_H
#define SDL_IMAGE_H
#include <SDL2/SDL_image.h>
#endif


//#include <glib.h>
//#include <glib/gprintf.h>
//#include <string.h>
//#include <stdlib.h>
// ca compile sans tout ca, a rajouter si pb ??

#include "processor.h"


gboolean update_gui_after_detection(gpointer data);
gpointer detection_thread_func(gpointer data);
void quit_button();
void image_button(GtkWidget* widget, gpointer data);
void load_button();
int main(int argc, char* argv[]);

#endif
