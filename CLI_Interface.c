#include <stdio.h>
#include <stdlib.h>

#define TAP1_X 558
#define TAP1_Y 500
#define TAP2_X 540
#define TAP2_Y 2200

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

    // If a device is found, you will reach here
    // printf("Device found: %s\n", buffer);
    
    fclose(fp);  // Close the pipe
    return 0;
}

int main() {
    // Check if ADB is available and a device is connected
    if (check_android_device() != 0) {
        return 1;
    }
    
    while(1){

    }

    // Call the existing Bash script to handle clipboard and Android interaction
    int result = system("./your_bash_script.sh"); // Replace with the path to your original Bash script
    if (result != 0) {
        printf("Error executing Bash script.\n");
        return 1;
    }

    printf("Done\n");
    return 0;
}