#!/bin/bash

export DISPLAY=:0
# Define the coordinates (x, y) for the taps
TAP1_X=$1
TAP1_Y=$2

TAP2_X=$3
TAP2_Y=$4

# Function to clean the clipboard (remove all spaces and trailing whitespace)
copyAndClean_clipboard() {
    # Read the current clipboard contents
    clipboard=$(xclip -selection clipboard -o)
    
    if [[ -z "$clipboard" ]]; then
        echo "Clipboard is empty. Please copy text before running the script."
        exit 1
    fi
    
    # Check if the clipboard contains only numbers (possibly with spaces)
    if [[ "$clipboard" =~ ^[0-9[:space:]+]+$ ]]; then  # Allow the + sign
        # Remove all spaces and trailing whitespace
        cleaned_clipboard=$(echo "$clipboard" | tr -d '[:space:]' | sed 's/[[:space:]]*$//')
    else
        echo "Error: Clipboard contains invalid content (non-numeric or invalid format)."
        exit 1
    fi
}

# Function to check if the input field is empty
# Function to clear the input field (select all and delete)
clear_input_field() {
    # Step 1: Select all text (Ctrl + A)
    adb shell input keyevent 57  # Ctrl key
    adb shell input keyevent 29  # A key to select all text
    adb shell input keyevent 57  # Release Ctrl key
    sleep 0.1  # Small delay to ensure text is selected

    # Step 2: Simulate backspace to delete the text (keycode 67)
    for i in {1..5}; do  # Loop to send backspace multiple times (adjust if necessary)
        adb shell input keyevent 67  # Backspace key event
    done
}


# Call this function to clear the input field
clear_input_field

# Clean the clipboard before proceeding
copyAndClean_clipboard

# Function to simulate a tap on the screen
tap() {
  local x=$1
  local y=$2
  adb shell input tap $x $y
}

# Function to simulate typing text into an input field on the Android device
type_text() {
  local text=$1
  # Convert any special characters like spaces to %20 for correct input
  local safe_text=$(echo "$text" | sed 's/ /%20/g')
  adb shell input text "$safe_text"
}

# Function to simulate pressing the ESCAPE key
press_escape() {
  adb shell input keyevent 111  # ESCAPE key event (keycode 111)
}

# Main execution

# Tap the first area to focus the input field
#echo "Tapping at ($TAP1_X, $TAP1_Y)..."
#echo "Tapping number area"
tap $TAP1_X $TAP1_Y
sleep 0.1  # Shorter wait time after tap

# Type the clipboard content into the focused input field on Android
#echo "Typing clipboard content into Android..."
#echo "Typing..."
type_text "$cleaned_clipboard"

# Press the ESCAPE key
#echo "Pressing ESCAPE..."
press_escape
sleep 0.2  # Shorter wait time for escape key press

# Tap the second area
last_four_digits="${cleaned_clipboard: -4}"
echo "Calling... $last_four_digits"
tap $TAP2_X $TAP2_Y

