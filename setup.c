#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("Unknown Linux distribution.\n");
    return 0;  // Unknown distribution
}

int ask_user_to_proceed(const char* action) {
    char choice;
    printf("%s (y/n): \n", action);
    // Get user input
    while (1) {
        scanf(" %c", &choice);  // Notice the space before %c to consume any leftover whitespace
        if (choice == 'y' || choice == 'Y') {
            return 1; // User agreed
        } else if (choice == 'n' || choice == 'N') {
            return 0; // User declined
        } else {
            printf("Invalid input. Please enter 'y' or 'n': ");
        }
    }
}

int check_adb() {
    // Check if adb is installed
    if (system("command -v adb > /dev/null 2>&1") != 0) {
        printf("ADB is not installed.\n");
        if (ask_user_to_proceed("Would you like the script to install ADB?") == 0){
            printf("Please install ADB manually.\n");
            return 1;
        }
        #if defined(__linux__)
            // Linux-based systems
            printf("Attempting to install ADB on Linux...\n");
            int distro = check_linux_distribution();  // Check Linux distro
            switch (distro) {
                case 1:  // Ubuntu
                case 2:  // Debian
                    if (system("sudo apt update && sudo apt install -y android-tools-adb") != 0) {
                        printf("Failed to install ADB on Debian/Ubuntu. Please install it manually.\n");
                        return 1;
                    }
                    printf("ADB installed successfully.\n");
                    break;
                case 3:  // Fedora
                    if (system("sudo dnf install -y android-tools") != 0) {
                        printf("Failed to install ADB on Fedora. Please install it manually.\n");
                        return 1;
                    }
                    printf("ADB installed successfully.\n");
                    break;
                case 4:  // CentOS
                    if (system("sudo yum install -y android-tools") != 0) {
                        printf("Failed to install ADB on CentOS. Please install it manually.\n");
                        return 1;
                    }
                    printf("ADB installed successfully.\n");
                    break;
                default:
                    printf("Unknown Linux distribution. Please install ADB manually.\n");
                    return 1;
            }

        #elif defined(__APPLE__)
            // macOS (using brew for installation)
            printf("Attempting to install ADB on macOS...\n");
            if (system("brew install android-platform-tools") != 0) {
                printf("Failed to install ADB on macOS. Please install it manually.\n");
                return 1;
            }
            printf("ADB installed successfully.\n");

        #elif defined(_WIN32)
            // Windows-based systems (ADB installation is manual or via third-party tools)
            printf("Please install ADB manually from https://developer.android.com/studio\n");
            return 1;

        #else
            // Unknown OS
            printf("Unsupported operating system. Please install ADB manually.\n");
            return 1;
        #endif
    } else {
        printf("ADB is already installed.\n");
    }
    return 0;
}
int main(){
    printf(" ___             ___  _           _                              ___              _                \n");
    printf("(  _`\\         /'___)( )_        ( )                            (  _`\\  _        (_ )              \n");
    printf("| (_(_)   _   | (__  | ,_) _ _   | |__     _     ___     __     | | ) |(_)   _ _  | |    __   _ __ \n");
    printf("`\\__ \\  /'_`\\ | ,__) | |  ( '_`\\ |  _ `\\ /'_`\\ /' _ `\\ /'__`\\   | | | )| | /'_` ) | |  /'__`\\( '__)\n");
    printf("( )_) |( (_) )| |    | |_ | (_) )| | | |( (_) )| ( ) |(  ___/   | |_) || |( (_| | | | (  ___/| |   \n");
    printf("`\\____)`\\___/'(_)    `\\__)| ,__/'(_) (_)`\\___/'(_) (_)`\\____)   (____/'(_)`\\__,_)(___)`\\____)(_)   \n");
    printf("                          | |                                                              \n");
    printf("                          (_)                                                              \n");
    printf("Welcome to the setup:\n");
    printf("Checking if ADB is installed:\n");
    if(check_adb() != 0){
        exit(1);
    };
    

}