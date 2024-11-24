#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int check_linux_distribution(){
    char buffer[128];
    FILE *fp;

    // Try to read the Linux distribution from /etc/os-release
    fp = fopen("/etc/os-release", "r");
    if (fp == NULL) {
        printf("Unable to detect the Linux distribution.\n");
        return -1;  // Error opening file
    }

    // Check for common distributions by reading the content of /etc/os-release
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "ID=ubuntu") != NULL) {
            printf("Ubuntu detected.\n");
            fclose(fp);
            return 1; // Ubuntu
        }
        if (strstr(buffer, "ID=debian") != NULL) {
            printf("Debian detected.\n");
            fclose(fp);
            return 2; // Debian
        }
        if (strstr(buffer, "ID=fedora") != NULL) {
            printf("Fedora detected.\n");
            fclose(fp);
            return 3; // Fedora
        }
        if (strstr(buffer, "ID=centos") != NULL) {
            printf("CentOS detected.\n");
            fclose(fp);
            return 4; // CentOS
        }
    }

    fclose(fp);
    return 0;  // Unknown distribution
}

int install_package(const char* package_name, int distro) {
    char command[256];
    printf("Attempting to install %s...\n", package_name);
    switch (distro) {
        case 1:  // Ubuntu
        case 2:  // Debian
            snprintf(command, sizeof(command), "sudo apt update && sudo apt install -y %s", package_name);
            break;
        case 3:  // Fedora
            snprintf(command, sizeof(command), "sudo dnf install -y %s", package_name);
            break;
        case 4:  // CentOS
            snprintf(command, sizeof(command), "sudo yum install -y %s", package_name);
            break;
        default:
            printf("Unknown Linux distribution. Please install %s manually.\n", package_name);
            return 1;
    }

    // Run the installation command
    if (system(command) != 0) {
        printf("Failed to install %s. Please install it manually.\n", package_name);
        return 1;
    }

    printf("%s installed successfully.\n", package_name);
    return 0;
}

int ask_user_to_proceed(const char* action) {
    char choice;
    printf("%s (y/n): \n", action);
    // Get user input
    while (1) {
        scanf(" %c", &choice);  // Notice the space before %c to consume any leftover whitespace
        if (choice == 'y' || choice == 'Y') {
            return 0; // User agreed
        } else if (choice == 'n' || choice == 'N') {
            return 1; // User declined
        } else {
            printf("Invalid input. Please enter 'y' or 'n': ");
        }
    }
}

int check_adb() {
    if (system("command -v adb > /dev/null 2>&1") != 0) {
        printf("ADB is not installed.\n");
        if (ask_user_to_proceed("Would you like the script to install ADB?") == 1){
            printf("Please install ADB manually.\n");
            return 1;
        }
            int distro = check_linux_distribution();  // Check Linux distro
            return install_package("android-tools-adb", distro);
    } else {
        printf("ADB is already installed.\n");
    }
    return 0;
}

int check_xclip() {
    if (system("command -v xclip > /dev/null 2>&1") != 0) {
        printf("xclip is not installed.\n");
        if (ask_user_to_proceed("Would you like the script to install xclip?") == 1) {
            printf("Please install xclip manually.\n");
            return 1;
        }
        int distro = check_linux_distribution();  // Check Linux distro
        return install_package("xclip", distro);  // Install xclip package
    } else {
        printf("xclip is already installed.\n");
    }
    return 0;
}

int check_device_connection(){
    FILE *fp = popen("adb devices | grep 'device$'", "r");
    
    if (fp == NULL) { 
        printf("Please ensure adb is configured correctly.\n");
        printf("Failed to run command: adb devices\n");
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
    printf("Device found: %s", buffer);
    return 0;
}   

void getCoordinates(int *x, int *y) {
    // Read the two integers (x and y) from the user input
    if (scanf("%d %d", x, y) != 2) {
        printf("Invalid input. Please enter two integers.\n");
    }
}

void saveToFile(int tap1_x, int tap1_y, int tap2_x, int tap2_y) {
    // Open the file config.txt for writing
    FILE *file = fopen("config.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write the coordinates in the desired format
    fprintf(file, "TAP1_X=%d\n", tap1_x);
    fprintf(file, "TAP1_Y=%d\n", tap1_y);
    fprintf(file, "TAP2_X=%d\n", tap2_x);
    fprintf(file, "TAP2_Y=%d\n", tap2_y);

    // Close the file
    fclose(file);
    printf("Coordinates saved to config.txt\n");
}

int update_tap_coords(){
    int tap1_x, tap1_y, tap2_x, tap2_y;

    // Get the first set of coordinates
    printf("Enter the coordinates for the text box as x y: ");
    getCoordinates(&tap1_x, &tap1_y);

    // Get the second set of coordinates
    printf("Enter the coordinates for the dialer x y: ");
    getCoordinates(&tap2_x, &tap2_y);

    // Save the coordinates to the file
    saveToFile(tap1_x, tap1_y, tap2_x, tap2_y);

    return 0;
    /*
    int result;
    result = system("./Android_x_y.sh");
    if (result == -1) {
        printf("Failed to update config\n");
        exit(1);
    }
    printf("Updated config successfully\n");
    return 0;
    */
}

int check_os(){
#if defined(__linux__)
    // Nothing happens if the system is Linux
    // The program continues silently (no output, no exit)
    return 0;
#else
    printf("The operating system is not currently supported.\n");
    return 1;
#endif
}

int main(){
    //Variable declarations1

    printf(" ___             ___  _           _                              ___              _                \n");
    printf("(  _`\\         /'___)( )_        ( )                            (  _`\\  _        (_ )              \n");
    printf("| (_(_)   _   | (__  | ,_) _ _   | |__     _     ___     __     | | ) |(_)   _ _  | |    __   _ __ \n");
    printf("`\\__ \\  /'_`\\ | ,__) | |  ( '_`\\ |  _ `\\ /'_`\\ /' _ `\\ /'__`\\   | | | )| | /'_` ) | |  /'__`\\( '__)\n");
    printf("( )_) |( (_) )| |    | |_ | (_) )| | | |( (_) )| ( ) |(  ___/   | |_) || |( (_| | | | (  ___/| |   \n");
    printf("`\\____)`\\___/'(_)    `\\__)| ,__/'(_) (_)`\\___/'(_) (_)`\\____)   (____/'(_)`\\__,_)(___)`\\____)(_)   \n");
    printf("                          | |                                                              \n");
    printf("                          (_)                                                              \n");
    printf("Welcome to the setup:\n");

    if(check_os() != 0){exit(1);}

    printf("Please connect your Android phone, allow your device to send and receive files and enable USB Debugging in the developer settings before continuing.\n");
    // Wait for the user to press Enter (or any key)
    printf("\nPress Enter to continue...\n");
    getchar();  // This will wait until Enter is pressed

    printf("\033[1mChecking if ADB is installed: \033[0m");
    if(check_adb() != 0){exit(1);}

    printf("\033[1mChecking if device is detected: \033[0m");
    if(check_device_connection() != 0){exit(1);}

    FILE *file = fopen("config.txt", "r");
    printf("\033[1mSetting up configuration file: \033[0m");
    if(file == NULL) {
        printf("Config not found, creating one.\n");
        update_tap_coords();
    } else {
        if(ask_user_to_proceed("Config file found, would you like to reconfigure it?") == 0){update_tap_coords();}
    }
    printf("\033[1mSetup finished successfully, Exiting...\033[0m\n");
    sleep(2);
    exit(0);
}