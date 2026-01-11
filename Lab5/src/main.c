#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include "tick.h"
#include <stdint.h>

// Direction Pin Definitions (Slide 12)
SI_SBIT(M1, SFR_P0, 4);   // EFM8 P0.4 -> L293D Input 1
SI_SBIT(M2, SFR_P0, 5);   // EFM8 P0.5 -> L293D Input 2

// Button Definitions
SI_SBIT(BTN0, SFR_P0, 2); // PB0: Spin Direction A
SI_SBIT(BTN1, SFR_P0, 3); // PB1: Spin Direction B

// PWM Speed Constants (8-bit PWM)
#define SPEED_MAX   0     // 100% Duty Cycle (PCA0CPH0 = 0)
#define SPEED_MIN   255   // 0% Duty Cycle (PCA0CPH0 = 255)
#define SPEED_STEP  25

void Set_Motor_Speed(uint8_t duty) {
    PCA0CPH0 = duty;      // Adjusts the PWM duty cycle on P0.1
}

void main(void) {
    uint8_t current_speed = 128; // Start at 50% speed

    enter_DefaultMode_from_RESET();
    IE_EA = 1;

    // Configure PCA0 for 8-bit PWM mode on P0.1
    // PCA0CPM0: PWM16=0, ECOM=1, CAPP=0, CAPN=0, MAT=0, TOG=0, PWM=1, ECCF=0
    PCA0CPM0 = 0x42;

    // Initial State: Stopped
    M1 = 0;
    M2 = 0;
    Set_Motor_Speed(current_speed);

    while (1) {
        // --- Direction Control (Buttons) ---
        if (BTN0 == 0) {       // PB0 pressed: Spin Right
            M1 = 1;
            M2 = 0;
            Wait(100);
        }
        if (BTN1 == 0) {       // PB1 pressed: Spin Left
            M1 = 0;
            M2 = 1;
            Wait(100);
        }

        // --- Speed Control (Joystick/Logic) ---
        // Note: You mentioned Joystick right/left for speed.
        // If the joystick uses ADC, you would read the ADC value here.
        // For now, we will use your existing button logic logic to simulate speed steps:

        /* Example logic if using digital inputs for Joystick:
        if (JOY_RIGHT == 0 && current_speed > SPEED_MAX) {
            current_speed -= SPEED_STEP;
            Set_Motor_Speed(current_speed);
            Wait(200);
        }
        */

        Set_Motor_Speed(current_speed);
    }
}
