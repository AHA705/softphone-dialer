#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define EVENT_FILE_1 "touch_events_1.txt" // File for first tap events
#define EVENT_FILE_2 "touch_events_2.txt" // File for second tap events

// Flag to control when to stop capturing events
volatile int capturing_events = 1;

// Function to execute adb command and save output to a file concurrently
void* capture_events(void* arg) {
    FILE *fp = (FILE*) arg;
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    // Run the adb command and save output to the file
    FILE *adb_fp = popen("adb shell getevent -l", "r");
    if (adb_fp == NULL) {
        perror("popen");
        exit(1);
    }

    char buffer[1024];
    while (capturing_events && fgets(buffer, sizeof(buffer), adb_fp) != NULL) {
        fprintf(fp, "%s", buffer); // Save the event to the file
    }

    fclose(adb_fp);
    fclose(fp);

    return NULL;
}

// Function to prompt user to tap Area 1
void prompt_first_tap(int *tap1_x, int *tap1_y) {
    char user_input[10];
    printf("Please tap the first area on the screen.\n");
    printf("When you're ready, press Enter to continue...\n");

    // Wait for user to press Enter to continue
    getchar(); // Wait for Enter key

    printf("Waiting for the first tap event...\n");

    // Capture events concurrently in a separate thread
    FILE *fp1 = fopen(EVENT_FILE_1, "w");
    if (fp1 == NULL) {
        perror("fopen");
        exit(1);
    }
    pthread_t capture_thread_1;
    pthread_create(&capture_thread_1, NULL, capture_events, fp1);

    // Wait for user to indicate that they tapped
    printf("Please indicate when you have tapped the first area by pressing Enter...\n");
    fgets(user_input, sizeof(user_input), stdin);
    
    // Stop capturing events
    capturing_events = 0;
    
    // Wait for the capture thread to finish
    pthread_cancel(capture_thread_1);
    pthread_join(capture_thread_1, NULL); // Ensure capture thread finishes before proceeding

    // Now parse the file for touch event data
    FILE *fp = fopen(EVENT_FILE_1, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "EV_ABS") != NULL) {
            if (strstr(buffer, "ABS_MT_POSITION_X") != NULL) {
                sscanf(buffer, "%*s %*s %*s %x", tap1_x); // Extract x-coordinate
            }
            if (strstr(buffer, "ABS_MT_POSITION_Y") != NULL) {
                sscanf(buffer, "%*s %*s %*s %x", tap1_y); // Extract y-coordinate
            }
        }
    }

    printf("First tap detected at: X = %d, Y = %d\n", *tap1_x, *tap1_y);

    fclose(fp);
}

// Function to prompt user to tap Area 2
void prompt_second_tap(int *tap2_x, int *tap2_y) {
    char user_input[10];
    printf("Please tap the second area on the screen.\n");
    printf("When you're ready, press Enter to continue...\n");

    // Wait for user to press Enter to continue
    getchar(); // Wait for Enter key

    printf("Waiting for the second tap event...\n");

    // Capture events concurrently in a separate thread
    FILE *fp2 = fopen(EVENT_FILE_2, "w");
    if (fp2 == NULL) {
        perror("fopen");
        exit(1);
    }
    pthread_t capture_thread_2;
    pthread_create(&capture_thread_2, NULL, capture_events, fp2);

    // Wait for user to indicate that they tapped
    printf("Please indicate when you have tapped the second area by pressing Enter...\n");
    fgets(user_input, sizeof(user_input), stdin);

    // Stop capturing events
    capturing_events = 0;

    // Wait for the capture thread to finish
    pthread_cancel(capture_thread_2);
    pthread_join(capture_thread_2, NULL); // Ensure capture thread finishes before proceeding

    // Now parse the file for touch event data
    FILE *fp = fopen(EVENT_FILE_2, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char buffer[1024];
    int found_first_x = 0, found_first_y = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "EV_ABS") != NULL) {
            if (strstr(buffer, "ABS_MT_POSITION_X") != NULL) {
                sscanf(buffer, "%*s %*s %*s %x", tap2_x); // Extract x-coordinate
                found_first_x = 1;
            }
            if (strstr(buffer, "ABS_MT_POSITION_Y") != NULL) {
                sscanf(buffer, "%*s %*s %*s %x", tap2_y); // Extract y-coordinate
                found_first_y = 1;
            }
        }
    }

    if (found_first_x && found_first_y) {
        printf("Second tap detected at: X = %d, Y = %d\n", *tap2_x, *tap2_y);
    } else {
        printf("Second tap detection failed or no touch events found.\n");
    }

    fclose(fp);
}

int main() {
    int tap1_x = -1, tap1_y = -1, tap2_x = -1, tap2_y = -1;

    // Prompt user to tap the first area (Area 1)
    prompt_first_tap(&tap1_x, &tap1_y);

    // Prompt user to tap the second area (Area 2)
    prompt_second_tap(&tap2_x, &tap2_y);

    // Check if both taps were successfully detected
    if (tap1_x != -1 && tap1_y != -1 && tap2_x != -1 && tap2_y != -1) {
        printf("Both taps detected.\n");
        printf("Tap 1: X = %d, Y = %d\n", tap1_x, tap1_y);
        printf("Tap 2: X = %d, Y = %d\n", tap2_x, tap2_y);
    } else {
        printf("Error: One or both taps were not detected properly.\n");
        return 1;
    }

    return 0;
}
