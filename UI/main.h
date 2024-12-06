#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <string.h>
#include <stdlib.h>
#include "processor.h"

gboolean update_gui_after_detection(gpointer data);
gpointer detection_thread_func(gpointer data);
void quit_button(GtkWidget* widget, gpointer data);
void image_button(GtkWidget* widget, gpointer data);
void load_button(GtkWidget* widget, gpointer data);
int main(int argc, char* argv[]);

#endif