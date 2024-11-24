#!/bin/bash

# Function to prompt user to tap Area 1
prompt_first_tap() {
    echo "Please tap the first area on the screen."
    echo "Waiting for the first tap event..."

    # Use timeout to make sure the loop does not hang indefinitely
    timeout 1 adb shell getevent -l | while read -r line; do
        if echo "$line" | grep -q "EV_ABS"; then
            if echo "$line" | grep -q "ABS_MT_POSITION_X"; then
                x_hex=$(echo "$line" | awk '{print $NF}')
                TAP1_X=$(printf "%d\n" "0x$x_hex")
            fi
            if echo "$line" | grep -q "ABS_MT_POSITION_Y"; then
                y_hex=$(echo "$line" | awk '{print $NF}')
                TAP1_Y=$(printf "%d\n" "0x$y_hex")
            fi
            if [ -n "$TAP1_X" ] && [ -n "$TAP1_Y" ]; then
                echo "First tap detected at: X = $TAP1_X, Y = $TAP1_Y"
                break
            fi
        fi
    done
}

# Function to prompt user to tap Area 2
prompt_second_tap() {
    echo "Please tap the second area on the screen."
    echo "Once tapped, press Enter to capture the coordinates or type 'exit' to quit."

    # Wait for user confirmation after tapping
    read -p "Press Enter to continue once you're ready to tap the second area, or type 'exit' to quit: " user_input

    if [ "$user_input" == "exit" ]; then
        echo "Exiting script."
        exit 0
    fi

    echo "Waiting for the second tap event..."

    # Use timeout to make sure the loop does not hang indefinitely
    timeout 1 adb shell getevent -l | while read -r line; do
        if echo "$line" | grep -q "EV_ABS"; then
            if echo "$line" | grep -q "ABS_MT_POSITION_X"; then
                x_hex=$(echo "$line" | awk '{print $NF}')
                TAP2_X=$(printf "%d\n" "0x$x_hex")
            fi
            if echo "$line" | grep -q "ABS_MT_POSITION_Y"; then
                y_hex=$(echo "$line" | awk '{print $NF}')
                TAP2_Y=$(printf "%d\n" "0x$y_hex")
            fi
            if [ -n "$TAP2_X" ] && [ -n "$TAP2_Y" ]; then
                echo "Second tap detected at: X = $TAP2_X, Y = $TAP2_Y"
                break
            fi
        fi
    done
}

# Start execution
clear

# Initialize variables to avoid using undefined variables
TAP1_X=""
TAP1_Y=""
TAP2_X=""
TAP2_Y=""

# Prompt user to tap the first area (Area 1)
prompt_first_tap

# Prompt user to tap the second area (Area 2)
prompt_second_tap

# Check if both taps were successfully detected
if [ -n "$TAP1_X" ] && [ -n "$TAP1_Y" ] && [ -n "$TAP2_X" ] && [ -n "$TAP2_Y" ]; then
    echo "Both taps detected."
    echo "Tap 1: X = $TAP1_X, Y = $TAP1_Y"
    echo "Tap 2: X = $TAP2_X, Y = $TAP2_Y"
else
    echo "Error: One or both taps were not detected properly."
    exit 1
fi
