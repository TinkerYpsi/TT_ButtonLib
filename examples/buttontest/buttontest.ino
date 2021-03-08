#include "ttbutton.h"


TTButton l_button = TTButton(5);
TTButton m_button = TTButton(6);
TTButton r_button = TTButton(7);

TTButtonGroup button_group = TTButtonGroup();

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
    Serial.println("Left button released first after " + String(press_time) + " ms");
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
    Serial.print('Ready!');

    button_group.addButton(&l_button);
    button_group.addButton(&m_button);
    button_group.addButton(&r_button);
    /*
        addFunction(
            start_button_count -- How many buttons are pressed to start to timer
            start_buttons -- Which buttons are pressed
            stop_button -- Which button, when released, stops the timer. 
                -1 if function should execute when any button is realesed.
            min_press_time -- Buttons must be pressed at least this many milliseconds...
            max_press_time -- ...but less than this many milliseconds to activate the assigned callback
        )
    */
    
    const int left_button_set[] = {0};
    const int right_button_set[] = {2};
    const int lr_button_set[] = {0, 2};
    const int all_button_set[] = {0, 1, 2};

    button_group.addFunction(1, left_button_set, -1, 0, MAX_PRESS_TIME, handleLeftPress);
    button_group.addFunction(1, right_button_set, -1, 0, MAX_PRESS_TIME, handleRightPress);
    button_group.addFunction(2, lr_button_set, 0, 0, MAX_PRESS_TIME, handleLRPressLeftRelease);
    button_group.addFunction(2, lr_button_set, 2, 0, MAX_PRESS_TIME, handleLRPressRightRelease);
    button_group.addFunction(3, all_button_set, -1, 0, 1000, handleAllPressAnyReleaseShort);
    button_group.addFunction(3, all_button_set, -1, 1000, MAX_PRESS_TIME, handleAllPressAnyReleaseLong);
}

void loop() {
    button_group.update();
}