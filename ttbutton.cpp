#include <Arduino.h>
#include "ttbutton.h"

TTButton::TTButton(int pin) {
    pinMode(pin, INPUT_PULLUP);
    this->pin = pin;
    this->debouncing = false;
    this->debounce_start = millis();
}

void TTButton::update() {
    
    bool instant_state = false;
    const int DEBOUNCE_TIME = 50;

    if(!this->debouncing) {
        instant_state = !digitalRead(pin);
        if(instant_state != steady_state) {
            this->debouncing = true;
            this->debounce_start = millis();
        }
    }
    else if(debouncing && millis() - this->debounce_start > DEBOUNCE_TIME) {
        this->steady_state = !digitalRead(pin);
        this->debouncing = false;
    }
}

bool TTButton::isPressed() {
    return this->steady_state;
}

TTButtonGroup::TTButtonGroup(){
    this->button_count = 0;
    this->button_mask = 0;
    this->prev_mask = 0;
    this->prev_buttons_pressed = 0;
    this->ignore_release = false;
    this->mask_start_time = millis();
}

void TTButtonGroup::addButton(TTButton* button_ptr) {
    this->buttons[button_count] = button_ptr;
    this->button_count++;
}

void TTButtonGroup::update() {

    // Read the actual button pins and handle debouncing
    for(int i = 0; i < this->button_count; i++) {
        buttons[i]->update();
    }

    // Read the current steady state of the buttons
    this->button_mask = 0;
    for(int i = 0; i < this->button_count; i++) {
        this->button_mask |= buttons[i]->isPressed() << i;
    }

    // Handle any changes
    int buttons_pressed = 0;

    for(int i = 0; i < this->button_count; i++) {
        if(this->button_mask & 1 << i) {
            buttons_pressed++;
        }
    }

    // Button Pressed
    if(buttons_pressed > this->prev_buttons_pressed) {
        this->mask_start_time = millis();
        this->ignore_release = false;
    }
    // Button Released
    else if(buttons_pressed < this->prev_buttons_pressed && !this->ignore_release) {
        unsigned int press_time = millis() - this->mask_start_time;
        for(int i = 0; i < this->function_count; i++) {
            BTN_FUNC* bf = &(this->button_functions[i]);
            if(
                bf->start_mask == this->prev_mask && 
                (bf->any_button_release || bf->stop_mask == this->button_mask) &&
                press_time >= bf->min_press &&
                press_time < bf->max_press
            ) {
                bf->callback(press_time);
                // Once a function has been executed, don't execute any more
                // based on subsequent button releases
                this->ignore_release = true;
            }
        }
    }
    this->prev_mask = this->button_mask;
    this->prev_buttons_pressed = buttons_pressed;
}

void TTButtonGroup::addFunction(
        int start_button_count, const int* start_buttons,
        int stop_button,
        unsigned int min_press, unsigned int max_press,
        ButtonCallback callback
    )
{
    uint8_t start_mask = 0;
    uint8_t stop_mask = 0;

    for(int i = 0; i < start_button_count; i++) {
        start_mask |= 1 << start_buttons[i];
    }

    BTN_FUNC* this_func = &(this->button_functions[function_count]);

    if(stop_button < -1 || stop_button > 31) {
        Serial.println("Stop button value " + String(stop_button) + " is out of bounds");
    }
    else if(stop_button == -1) {
        this_func->any_button_release = true;
        stop_mask = 0;
    }
    else {
        this_func->any_button_release = false;
        for(int i = 0; i < start_button_count - 1; i++) {
            stop_mask = start_mask & ~(1 << stop_button);
        }
    }
    
    this_func->start_mask = start_mask;
    this_func->stop_mask = stop_mask;
    this_func->callback = callback;
    this_func->min_press = min_press;
    this_func->max_press = max_press;
    this->function_count++;
}

