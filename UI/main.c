#include "main.h"
#include "processor.h"

#define BUTTON_COUNT 7


typedef struct {
    GtkWidget *imageWidget;
    GtkWidget *loadingDialog;
} DetectionData;

GtkImage* displayedimage;
GtkWidget* window;
GtkWidget* mainBox;

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
    "data/post_PRT.png",
    "contrastboost.png",
    "rotation.png", 
    "automaticrotation.png",
    "../data/post_DET.png",
    "solver.png",
    "image.png"
};

GtkWidget* imageWidget;
GtkWidget* searchEntry;

gboolean update_gui_after_detection(gpointer data)
{
    DetectionData *detectionData = (DetectionData*) data;
    gtk_widget_destroy(detectionData->loadingDialog);
    displayedimage = "data/post_DET.png";
    gtk_image_set_from_file(GTK_IMAGE(detectionData->imageWidget), displayedimage);
    g_print("Detection process completed successfully!\n");
    g_free(detectionData);

    return FALSE;
}

gpointer detection_thread_func(gpointer data)
{
    DetectionData *detectionData = (DetectionData*) data;
    run_detection();
    g_idle_add(update_gui_after_detection, detectionData);

    return NULL;
}


void quit_button(GtkWidget* widget, gpointer data) 
{
    gtk_main_quit();
}

void image_button(GtkWidget* widget, gpointer data) 
{
    const char* filename = gtk_entry_get_text(GTK_ENTRY(searchEntry));
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
            SDL_Surface* backgroundImage = IMG_Load(filename);
            if (backgroundImage) 
            {
                printf(".png to SDL surface loaded successfully\n");

                run_pretreatment(backgroundImage, treatmentLevel, 0); 

                const char* outputPath = "data/post_PRT.png";
                if (backgroundImage) 
                {
                    printf("SDL surface saved to: %s\n", outputPath);

        
                    displayedimage = outputPath;
                    gtk_image_set_from_file(GTK_IMAGE(imageWidget), displayedimage);
                    g_print("Displayed image updated to: %s\n", displayedimage);
                } 
                else 
                {
                    g_print("Failed to save SDL surface: %s\n", SDL_GetError());
                }

                SDL_FreeSurface(backgroundImage);
            } 
            else 
            {
                g_print("Failed to load SDL surface from: %s\n", filename);
            }
        }
    }
    




    // TODO ############################################
    else if (strcmp(buttonLabel, "Rotation") == 0) 
    {
        GtkWidget *dialog = gtk_dialog_new_with_buttons(
            "Enter Rotation Angle",
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_MODAL,
            "Rotate", GTK_RESPONSE_ACCEPT,
            "Cancel", GTK_RESPONSE_CANCEL,
            NULL);

        GtkWidget *contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter angle in degrees");
        gtk_box_pack_start(GTK_BOX(contentArea), entry, TRUE, TRUE, 0);

        gtk_widget_show_all(dialog);

        gint response = gtk_dialog_run(GTK_DIALOG(dialog));

        if (response == GTK_RESPONSE_ACCEPT) 
        {
            const char *angleText = gtk_entry_get_text(GTK_ENTRY(entry));
            if (angleText != NULL && strlen(angleText) > 0)
            {
                int angle = atoi(angleText);
                printf("Rotating image by %d degrees\n", angle);
                SDL_Surface* backgroundImage = IMG_Load(displayedimage);
                if (backgroundImage) 
                {
                    printf("ifbackgroundimage ok \n");
                    run_pretreatment(&backgroundImage, 4, angle);
                    int res = backgroundImage == NULL;
                    printf("IS NULL : %d\n", res);
                    printf("run pretreatment ok \n");
                    const char* outputPath = "data/post_PRT.png";

                    if (IMG_SavePNG(backgroundImage, outputPath) != NULL) 
                    {
                        printf("SDL surface saved to: %s\n", outputPath);
                        displayedimage = outputPath;
                        gtk_image_set_from_file(GTK_IMAGE(imageWidget), displayedimage);
                        g_print("Displayed image updated to: %s\n", displayedimage);
                    } 
                    else 
                    {
                        g_print("Failed to save SDL surface: %s\n", SDL_GetError());
                    }

                    SDL_FreeSurface(backgroundImage);
                } 
                else 
                {
                    g_print("Failed to load image for rotation: %s\n", displayedimage);
                }
            } 
            else 
            {
                g_print("Invalid input or no angle provided. Rotation canceled.\n");
            }
        }

        gtk_widget_destroy(dialog);
    }
    //#########################################################

    else if (strcmp(buttonLabel, "Automatic Rotation") == 0) 
    {
        SDL_Surface* backgroundImage = IMG_Load(displayedimage);
        if (backgroundImage) 
        {
            run_pretreatment(backgroundImage, 4, 0);
            const char* outputPath = "data/post_PRT.png";
            
            if (backgroundImage) 
            {
                printf("SDL surface saved to: %s\n", outputPath);

                displayedimage = outputPath;
                gtk_image_set_from_file(GTK_IMAGE(imageWidget), displayedimage);
                g_print("Displayed image updated to: ");
            } 
            else 
            {
                g_print("Failed to save SDL surface: %s\n", SDL_GetError());
            }

            SDL_FreeSurface(backgroundImage);
        } 
        else 
        {
            g_print("Failed to load image for automatic rotation");
        }
    }














    
    else if (strcmp(buttonLabel, "Detection") == 0) 
    {
        SDL_Surface* processedImage = IMG_Load("data/post_PRT.png");
        if (processedImage) 
        {
            GtkWidget* loadingDialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                            GTK_DIALOG_MODAL,
                                                            GTK_MESSAGE_INFO,
                                                            GTK_BUTTONS_NONE,
                                                            "Please wait a few minutes...");
            gtk_widget_show_now(loadingDialog);
            DetectionData *detectionData = g_new(DetectionData, 1);
            detectionData->imageWidget = imageWidget;
            detectionData->loadingDialog = loadingDialog;
            GThread *thread = g_thread_new("detection_thread", detection_thread_func, detectionData);

            SDL_FreeSurface(processedImage);
        }
        else
        {
            g_print("Failed to load image: %s\n", "data/post_PRT.png");
        }
    }
    
    else if (strcmp(buttonLabel, "AUX") == 0) 
    {
        
        const char* filePath = "neuron_network/other/grid";  //wrong path?  does not work ftm

        
        if (g_file_test(filePath, G_FILE_TEST_EXISTS)) 
        {
            char command[512]; 

            snprintf(command, sizeof(command), "vim %s", filePath);

    
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

    else if (strcmp(buttonLabel, "Solver") == 0) 
    {
            g_print("re");
            run_solver(2);


        
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
    gtk_window_set_title(GTK_WINDOW(window), "ANSN Studio OCR");
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