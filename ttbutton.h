#pragma once
#ifndef C03A6E4F_7800_485E_B4C0_4199219C7BAB
#define C03A6E4F_7800_485E_B4C0_4199219C7BAB

#include "Arduino.h"

typedef void(*ButtonCallback)(int);
#define MAX_PRESS_TIME UINT16_MAX

class TTButton {

    public:

    TTButton(int pin);

    void update();
    bool isPressed();

    private:
   
    int pin;
    bool debouncing;
    unsigned long debounce_start;
    bool steady_state;
    

};


class TTButtonGroup {

    public:

    typedef struct _btn_func {
        uint8_t start_mask;
        uint8_t stop_mask;
        ButtonCallback callback;
        uint16_t min_press;
        uint16_t max_press;
        bool any_button_release;
    } BTN_FUNC;

    

    TTButtonGroup();
    void addButton(TTButton* button_ptr);
    void update();
    void getState();
    void addFunction(
        int start_button_count, const int* start_buttons,
        int stop_button, 
        unsigned int min_press, unsigned int max_press,
        ButtonCallback callback
    );

    private:
    unsigned long int mask_start_time;
    uint8_t prev_mask;
    int prev_buttons_pressed;
    int button_count;
    int function_count;
    bool ignore_release;
    uint8_t button_mask;
    BTN_FUNC button_functions[10];
    TTButton* buttons[8];
};

#endif
