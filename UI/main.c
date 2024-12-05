#include "main.h"

#define BUTTON_COUNT 7

const char* buttonLabels[BUTTON_COUNT] = 
{
    "Pretreatment",
    "Contrast Boost",
    "Rotation",
    "Automatic Rotation",
    "Detection",
    "Solver", 
    "AUX"
};

const char* imagePaths[BUTTON_COUNT] = 
{
    "../data/post_PRT.png",
    "contrastboost.png",
    "rotation.png", 
    "automaticrotation.png",
    "../data/post_DET.png",
    "solver.png",
    "image.png"
};

GtkWidget* imageWidget;
GtkWidget* searchEntry;

void quit_button(GtkWidget* widget, gpointer data) 
{
    gtk_main_quit();
}

void image_button(GtkWidget* widget, gpointer data) 
{
    const char* buttonLabel = (const char*)data;

    if (strcmp(buttonLabel, "Pretreatment") == 0) 
    {
        GtkWidget* dialog = gtk_dialog_new_with_buttons("Select Treatment Level",
                                                        GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                        GTK_DIALOG_MODAL,
                                                        "Light", 1,
                                                        "Medium", 2,
                                                        "Heavy", 3,
                                                        "Cancel", 0,
                                                        NULL);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        int treatmentLevel = 0;

        if (response == 1) treatmentLevel = 1;  
        else if (response == 2) treatmentLevel = 2;  
        else if (response == 3) treatmentLevel = 3;  

        gtk_widget_destroy(dialog);

        if (treatmentLevel > 0) 
        {
            // Change the image to "../data/images/l11.png"
            gtk_image_set_from_file(GTK_IMAGE(imageWidget), "../data/images/l11.png");

            SDL_Surface* backgroundImage = SDL_LoadBMP("../data/images/l11.png"); // Replace
            if (backgroundImage) 
            {
                //run_pretreatment(backgroundImage, treatmentLevel); // to fix bc ne reconnait pas
            }
        }
    } 
    else if (strcmp(buttonLabel, "Rotation") == 0) 
    {
        // Handle rotation button: Call run_rotation with parameter 4
        SDL_Surface* backgroundImage = SDL_LoadBMP("background_image.bmp"); // Replace
        if (backgroundImage) 
        {
            //run_rotation(backgroundImage, 4); // to fix bc ne reconnait pas
        }
    } 
    else if (strcmp(buttonLabel, "AUX") == 0) 
    {
        
        const char* filePath = "../neuron_network/other/word";  //wrong path?  does not work ftm

        // Check if the file exists
        if (g_file_test(filePath, G_FILE_TEST_EXISTS)) 
        {
            char command[512]; 

            // ca ca ne marche pas
            snprintf(command, sizeof(command), "xdg-open \"%s\"", filePath);

    
            int result = system(command);
            if (result == 0)
            {
                g_print("Opened AUX file: %s\n", filePath);
            }
            else
            {
                g_print("Failed to open AUX file: %s\n", filePath);
            }
        }
        else
        {
            g_print("File does not exist: %s\n", filePath);
        }
    } 
    else 
    {
    
        gtk_image_set_from_file(GTK_IMAGE(imageWidget), buttonLabel);
        g_print("Button clicked: %s\n", buttonLabel);
    }
}

void load_button(GtkWidget* widget, gpointer data) 
{
    const char* filename = gtk_entry_get_text(GTK_ENTRY(searchEntry));
    if(strlen(filename) > 4 && 
    strcmp(filename + strlen(filename) - 4, ".png") == 0)
    {
        if (g_file_test(filename, G_FILE_TEST_EXISTS))
        {
            gtk_image_set_from_file(GTK_IMAGE(imageWidget), filename);
            g_print("Loaded image: %s\n", filename);
        }
        else
        {
            g_print("File does not exist: %s\n", filename);
        }
    }
    else
    {
        g_print("Invalid file type: %s\n", filename);
    }
}

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Interface");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(mainBox), 10);
    gtk_container_add(GTK_CONTAINER(window), mainBox);

    GtkWidget* topBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(mainBox), topBox, TRUE, TRUE, 0);

    GtkWidget* buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(buttonBox, FALSE);
    gtk_widget_set_vexpand(buttonBox, TRUE);
    gtk_box_pack_start(GTK_BOX(topBox), buttonBox, FALSE, TRUE, 0);

    for (int i = 0; i < BUTTON_COUNT; i++) 
    {
        GtkWidget* button = gtk_button_new_with_label(buttonLabels[i]);
        g_signal_connect(button, "clicked", G_CALLBACK(image_button), (gpointer)buttonLabels[i]);
        gtk_box_pack_start(GTK_BOX(buttonBox), button, TRUE, TRUE, 0);
    }


    GtkWidget* quitButton = gtk_button_new_with_label("Quit");
    g_signal_connect(quitButton, "clicked", 
                    G_CALLBACK(quit_button), NULL);
    gtk_box_pack_end(GTK_BOX(buttonBox), quitButton, FALSE, FALSE, 0);

    imageWidget = gtk_image_new();
    if (argc > 1)
    {
        gtk_image_set_from_file(GTK_IMAGE(imageWidget), argv[1]);
    }
    else
    {
        gtk_image_set_from_file(GTK_IMAGE(imageWidget), 
                                "resultPretreatment.png");
    }
    gtk_box_pack_start(GTK_BOX(topBox), imageWidget, TRUE, TRUE, 0);

    GtkWidget* searchBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(mainBox), searchBox, FALSE, FALSE, 0);

    searchEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(searchEntry), 
                                   "Enter image file (e.g., image.png)");
    gtk_box_pack_start(GTK_BOX(searchBox), searchEntry, TRUE, TRUE, 0);

    GtkWidget* loadButton = gtk_button_new_with_label("Load");
    g_signal_connect(loadButton, "clicked",
                                G_CALLBACK(load_button), NULL);
    gtk_box_pack_start(GTK_BOX(searchBox), loadButton, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
} 