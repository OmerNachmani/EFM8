#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include "tick.h"

// Define Control Pins
SI_SBIT(M_IN1, SFR_P0, 4);  // Direction Pin 1
SI_SBIT(M_IN2, SFR_P0, 5);  // Direction Pin 2
SI_SBIT(BTN0, SFR_P0, 2);   // PB0: Spin Right
SI_SBIT(BTN1, SFR_P0, 3);   // PB1: Spin Left

void main(void) {
    uint8_t joystick_val;

    enter_DefaultMode_from_RESET();
    IE_EA = 1;

    // Initial State: Stopped
    M_IN1 = 0;
    M_IN2 = 0;

    while (1) {
        // 1. Direction Control (Buttons)
        if (BTN0 == 0) { // Right
            M_IN1 = 1; M_IN2 = 0;
        }
        else if (BTN1 == 0) { // Left
            M_IN1 = 0; M_IN2 = 1;
        }

        // 2. Speed Control (Joystick ADC)
        // Your InitDevice.c sets ADC0 to trigger on Timer 0
        if (ADC0CN0_ADINT) {        // Check if conversion is complete
            ADC0CN0_ADINT = 0;      // Clear flag
            joystick_val = ADC0H;   // Get 8-bit MSB of joystick

            // PCA0CPH0 controls the PWM duty cycle on P0.1 [cite: 152, 158]
            // Note: In 8-bit PWM, 0 is full speed, 255 is stop.
            PCA0CPH0 = joystick_val;
        }
    }
}
