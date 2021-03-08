#include "ttbutton.h"

// Start by creating button objects
// Note that each button is given an internal index in the button group as it is created
TTButton left_button   = TTButton(5);  // Button connected to pin 5, given group index 0
TTButton middle_button = TTButton(6);  // Button connected to pin 6, given group index 1
TTButton right_button  = TTButton(7);  // Button connected to pin 7, given group index 2

// Create a button group to which we can later assign the buttons
TTButtonGroup button_group = TTButtonGroup();

// Crate functions to assign to the button group
void handleLeftPress(unsigned int press_time) {
    Serial.println("Left button pressed for " + String(press_time) + " ms");
}

void handleRightPress(unsigned int press_time) {
    Serial.println("Right button pressed for " + String(press_time) + " ms");
}

void handleLRPressLeftRelease(unsigned int press_time) {
    Serial.println("Left and right buttons pressed");
    Serial.println("Left button released first after " + String(press_time) + " ms");
}

void handleLRPressRightRelease(unsigned int press_time) {
    Serial.println("Left and right buttons pressed");
    Serial.println("Right button released first after " + String(press_time) + " ms");
}

void handleAllPressAnyReleaseShort(unsigned int press_time) {
    Serial.println("All buttons pressed");
    Serial.println("One button released after " + String(press_time) + " ms");
}

void handleAllPressAnyReleaseLong(unsigned int press_time) {
    Serial.println("All buttons pressed for a long time");
    Serial.println("One button released after " + String(press_time) + " ms");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Ready!");

    // Add the buttons to the button group
    // The ButtonGroup takes pointers to the Button objects. If you haven't used pointers before, 
    // don't worry about it, just don't forget to put a "&" in front of each button name when assigning it. 
    button_group.addButton(&left_button);
    button_group.addButton(&middle_button);
    button_group.addButton(&right_button);
    /*
        addFunction(
            start_button_count -- How many buttons are pressed to start to timer
            start_buttons -- Which buttons are pressed
            stop_button -- Which button, when released, stops the timer. 
                -1 if function should execute when any button is realesed.
            min_press_time -- Buttons must be pressed at least this many milliseconds...
            max_press_time -- ...but less than this many milliseconds to activate the assigned callback
        )

        MAX_PRESS_TIME is defined as the maximum value of an unsigned int.
    */
    
    const int left_button_set[]  = {0};         // Left button pressed
    const int right_button_set[] = {2};         // Right button pressed
    const int lr_button_set[]    = {0, 2};      // Left & right buttons pressed
    const int all_button_set[]   = {0, 1, 2};   // All buttons pressed

    // Call handleLeftPress() after left button is released
    button_group.addFunction(1, left_button_set, -1, 0, MAX_PRESS_TIME, handleLeftPress);

    // Call handlerightPress() after right button is released
    button_group.addFunction(1, right_button_set, -1, 0, MAX_PRESS_TIME, handleRightPress);

    // Call handleLRPressLeftRelease() after left & right buttons are pressed and the left one is released
    button_group.addFunction(2, lr_button_set, 0, 0, MAX_PRESS_TIME, handleLRPressLeftRelease);

    // Call handleLRPressRightRelease() after left & right buttons are pressed and the right one is released
    button_group.addFunction(2, lr_button_set, 2, 0, MAX_PRESS_TIME, handleLRPressRightRelease);

    // Call handleAllPressAnyReleaseShort() after all buttons are pressed and any of them are released in less than 1 second
    button_group.addFunction(3, all_button_set, -1, 0, 1000, handleAllPressAnyReleaseShort);

    // Call handleAllPressAnyReleaseLong() after all buttons are pressed and any of them are released in more than 1 second
    button_group.addFunction(3, all_button_set, -1, 1000, MAX_PRESS_TIME, handleAllPressAnyReleaseLong);
}

void loop() {
    // Constantly poll buttons. Debouncing is handled under the hood. 
    // When a transition occurs that matches one to which a function has been assigned, that function is called
    button_group.update();
}