#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

// Function to check if ADB is available
int check_android_device() {
    // Check if adb is installed
    if (system("command -v adb > /dev/null 2>&1") != 0) {
        printf("ADB is not installed. Please install ADB.\n");
        return 1;
    }

    FILE *fp = popen("adb devices | grep 'device$'", "r");
    
    if (fp == NULL) {  // Correct check for NULL (not 0)
        printf("Error opening pipe. Please ensure adb is installed and configured correctly.\n");
        return 1;
    }

    // Try reading one line from the output of the command
    char buffer[128];  // Buffer to store each line read from the command output
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        // If no device is found (empty output from the command), print a message
        printf("No device found. Please ensure USB debugging is enabled and the device is connected.\n");
        fclose(fp);
        return 1;
    }

    fclose(fp);  // Close the pipe
    return 0;
}

// Callback function for the button click
void on_button_clicked(GtkWidget *widget, gpointer data) {
    // Check if a device is connected and print the result to the terminal
    if (check_android_device() != 0) {
        // Print to the terminal that no device was found
        printf("No device found. Please ensure USB debugging is enabled and the device is connected.\n");
    } else {
        // Print to the terminal that the device is found
        printf("Device found.\n");
    }
}

int main(int argc, char *argv[]) {
    
 // Declare variables to store the values read from the file
    int TAP1_X, TAP1_Y, TAP2_X, TAP2_Y;

    // Open the configuration file for reading
    FILE *file = fopen("config.txt", "r");
    if (file == NULL) {
        perror("Error finding config.txt, please run setup");
        return 1;
    }

    // Read each line from the file
    char line[256];  // Buffer for each line
    while (fscanf(file, "TAP1_X %d", &TAP1_X) == 1) {
        // Successfully read TAP1_X
    }
    while (fscanf(file, "TAP1_Y %d", &TAP1_Y) == 1) {
        // Successfully read TAP1_Y
    }
    while (fscanf(file, "TAP2_X %d", &TAP2_X) == 1) {
        // Successfully read TAP2_X
    }
    while (fscanf(file, "TAP2_Y %d", &TAP2_Y) == 1) {
        // Successfully read TAP2_Y
    }

    // Close the file
    fclose(file);
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Softphone-Dialer");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Create a button
    GtkWidget *button = gtk_button_new_with_label("Dial number:");

    // Set up a container (box) and add the button to the window
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Connect the button's "clicked" signal to the callback function
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Connect the "destroy" signal to close the window when the user exits
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
