#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "processor.h"

void quit_button(GtkWidget* widget, gpointer data);
void image_button(GtkWidget* widget, gpointer data);
void load_button(GtkWidget* widget, gpointer data);
int main(int argc, char* argv[]);

#endif