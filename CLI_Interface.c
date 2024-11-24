#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to check if ADB is available
int TAP1_X, TAP1_Y, TAP2_X, TAP2_Y;

int get_coord_value(const char* line) {
    int value = 0;
    // Try to extract the value after the '=' sign
    if (sscanf(line, "%*[^=]=%d", &value) == 1) {
        return value;
    }
    return -1; // Return -1 if parsing fails
}

int check_config(){
    FILE *file = fopen("config.txt", "r");
    if(file == NULL) {
        printf("Config not found, please run setup.\n");
        return 1;
    }
    char line[30];
     while (fgets(line, sizeof(line), file)) {
        // Remove the newline character if it exists
        line[strcspn(line, "\n")] = '\0';

        // Check each line and assign values
        if (strstr(line, "TAP1_X=")) {
            TAP1_X = get_coord_value(line);
        } else if (strstr(line, "TAP1_Y=")) {
            TAP1_Y = get_coord_value(line);
        } else if (strstr(line, "TAP2_X=")) {
            TAP2_X = get_coord_value(line);
        } else if (strstr(line, "TAP2_Y=")) {
            TAP2_Y = get_coord_value(line);
        }
    }
    if (TAP1_X == -1 || TAP1_Y == -1 || TAP2_X == -1 || TAP2_Y == -1) {
        fprintf(stderr, "Error: One or more coordinates are missing or invalid in config.txt\n");
        return 1;
    }
    fclose(file);
    return 0;
}


int check_android_device() {
    
    if (system("command -v adb > /dev/null 2>&1") != 0) {
        printf("ADB is not installed. please run setup.\n");
        return 1;
    }

    FILE *fp = popen("adb devices | grep 'device$'", "r");
    
    if (fp == NULL) {  // Correct check for NULL (not 0)
        printf("Error opening pipe. Please ensure adb is installed and configured correctly.\n");
        return 1;
    }

    // Try reading one line from the output of the command
    char buffer[128];  // Buffer to store each line read from the command output
    // While no device is found (fgets returns NULL), keep prompting user for retry
    while (fgets(buffer, sizeof(buffer), fp) == NULL) {
        printf("No device found. Please ensure USB debugging is enabled and the device is connected.\n");
        printf("Press Enter to retry...\n");
        // Wait for the user to press Enter before retrying
        getchar(); 
        sleep(1);

        // Reopen the pipe to retry the command
        fclose(fp);  // Close the old file pointer
        fp = popen("adb devices | grep 'device$'", "r");  // Reopen the pipe

        if (fp == NULL) {
            printf("Error opening pipe. Please ensure adb is installed and configured correctly.\n");
            return 1;
        }
    }
    fclose(fp);  // Close the pipe
    printf("Device found: %s", buffer);
    return 0;
}

int main() {


    printf(" ___             ___  _           _                              ___              _                \n");
    printf("(  _`\\         /'___)( )_        ( )                            (  _`\\  _        (_ )              \n");
    printf("| (_(_)   _   | (__  | ,_) _ _   | |__     _     ___     __     | | ) |(_)   _ _  | |    __   _ __ \n");
    printf("`\\__ \\  /'_`\\ | ,__) | |  ( '_`\\ |  _ `\\ /'_`\\ /' _ `\\ /'__`\\   | | | )| | /'_` ) | |  /'__`\\( '__)\n");
    printf("( )_) |( (_) )| |    | |_ | (_) )| | | |( (_) )| ( ) |(  ___/   | |_) || |( (_| | | | (  ___/| |   \n");
    printf("`\\____)`\\___/'(_)    `\\__)| ,__/'(_) (_)`\\___/'(_) (_)`\\____)   (____/'(_)`\\__,_)(___)`\\____)(_)   \n");
    printf("                          | |                                                              \n");
    printf("                          (_)                                                              \n");

    printf("Please connect your Android phone, allow your device to send and receive files and enable USB Debugging in the developer settings before continuing.\n");
    // Wait for the user to press Enter (or any key)
    printf("\nPress Enter to continue...\n");
    getchar();  // This will wait until Enter is pressed


    // Check if ADB is available and a device is connected
    if(check_config() != 0){exit(1);}


    printf("\033[1mChecking if device is detected: \033[0m");

    if (check_android_device() != 0){exit(1);}
    
    const char *file_name = "./automate_number.sh";
    FILE *file = fopen(file_name, "r");
    if (!file){
        printf("automate_number.sh is missing from working directory!\n");
        exit(1);
    }
     // Create the command string with arguments
    char command[256];  // Ensure enough space for the command string
    snprintf(command, sizeof(command), "./automate_number.sh %d %d %d %d", TAP1_X, TAP1_Y, TAP2_X, TAP2_Y);

    printf("\033[1mProgram is ready, press enter to dial the number in the clipboard or CTRL + C to exit: \n\033[0m");
    while(1){
        printf("Awaiting Enter key: \n");
        getchar();
        int result = system(command);
    }
    return 0;
}
