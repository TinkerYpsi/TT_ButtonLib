# TT_ButtonLib
An Arduino library for easily handling multiple button presses!

This library allows you to easily set up multiple buttons and assign functions to various combinations of presses and hold times. The library also automagically
handles button debouncing for you. For example, if you had three buttons, you could assign a function to execute when any of the three buttons is pressed and
immediately released, a different function when each of them are pressed and released after 3 seconds, and another when two buttons are pressed and one or the
other of them is released. This allows you, with great ease, to dramatically increase the functionality you can get from just a few buttons.

Below is an example of how to set up a single button group, but you can use multiple button groups; just make sure to not assign the same button to multiple groups.


```cpp
#include "ttbutton.h"

// Start by creating button objects
// Note that each button is given an internal index in the button group as it is created
TTButton left_button   = TTButton(5);  // Button connected to pin 5, given group index 0
TTButton middle_button = TTButton(6);  // Button connected to pin 6, given group index 1
TTButton right_button  = TTButton(7);  // Button connected to pin 7, given group index 2

// Create a button group to which we can later assign the buttons
TTButtonGroup button_group = TTButtonGroup();

// Create the functions you want to assign to the group
void left(int press_time) {
  Serial.println("Left button press");
}

void right(int press_time) {
  Serial.println("Right button long press");
}

void all(int press_time) {
  Serial.println("All buttons pressed, middle released");
}

void setup() {
  Serial.begin(9600);
  
  // Add the buttons to the button group
  // The ButtonGroup takes pointers to the Button objects. If you haven't used pointers before, 
  // don't worry about it, just don't forget to put a "&" in front of each button name when assigning it. 
  button_group.addButton(&left_button);
  button_group.addButton(&middle_button);
  button_group.addButton(&right_button);
    
  // Using the button group indicies above, choose sets of buttons from the group to which you want to assign functions 
  const int left_button_set[]  = {0};        // Left button pressed
  const int right_button_set[] = {2};        // Right button pressed
  const int all_button_set[]   = {0, 1, 2};  // All buttons pressed

  // Assign functions to button group
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
  
  // Calls left() after any length press of left button
  button_group.addFunction(1, left_button_set, -1, 0, MAX_PRESS_TIME, myFunc_0);
  
  // Calls right() when right button is released after being held at least 3 seconds
  button_group.addFunction(1, right_button_set, -1, 3000, MAX_PRESS_TIME, myFunc_1);
  
  // Calls all() after all buttons are held for at least 1 second and the middle button is released
  button_group.addFunction(3, all_button_set, 1, 0, MAX_PRESS_TIME, all);
}

void loop() {
  // Constantly poll buttons. Debouncing is handled under the hood. 
  // When a transition occurs that matches one to which a function has been assigned, that function is called
  button_group.update();
}
```

## Limitations
- Button pins are automatically set as `INPUT_PULLUP`, so the button lead not connected to its assigned GPIO pin must be attached to GND.
- Each group may have a maximum of 8 buttons and 10 button functions assigned to it. If you absolutely need more, you can modify the library definitions, 
but these were selected to minimize the library's memory consumption.
- Functions are only executed upon the release of a button. 
- The library is based on polling, so a button group's `update()` function must be called frequently for good responsiveness. If you have long-running functions,
you may want to add a call to update the buttons within them or design them to be reentrant, allowing for reasonably frequent returns to the main loop where
you can perform the button update.
- Button functions must be of the form `void myCallback(int press_time)`

## Order of Operations
**Q:** What happens if I assign a function to occur after button A and button B are held and button B is released, but also assign another function to occur 
when button A is released.

**A:** If you press button A and release it, the function assigned to that single button will be called. If you press both buttons and release button B, then button A,
only the function assigned to the double press / button B release will be called. The function assigned to button A will not be called when it is released after button
B, only when both buttons have been released and it is pressed and released again.

tl;dr -- If multiple buttons are pressed, only one function will be called as buttons are sequentially released.
