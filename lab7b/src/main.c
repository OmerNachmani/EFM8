#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"

// Hardware Pin Definitions
SI_SBIT(PB0, SFR_P0, 2); // Left input
SI_SBIT(PB1, SFR_P0, 3); // Right input

// PCA Capture/Compare values for Servo positioning
// Clock source: 4MHz.
// Target pulse widths: 0.6ms (Min), 1.5ms (Mid), 2.4ms (Max)
#define TICKS_AT_0_DEG    63136
#define TICKS_AT_90_DEG   59536
#define TICKS_AT_180_DEG  55936

// ----------------------------------------------------------------------------
// Function Declarations
// ----------------------------------------------------------------------------
void SiLabs_Startup(void);
void Update_PCA_Comparator(uint16_t compare_value);
void Simple_Wait(uint16_t duration);

// ----------------------------------------------------------------------------
// Startup Routine
// ----------------------------------------------------------------------------
void SiLabs_Startup(void)
{
    // Stop the Watchdog Timer to prevent system reset
    PCA0MD &= ~0x40;
}

// ----------------------------------------------------------------------------
// Utility Functions
// ----------------------------------------------------------------------------

// Loads a 16-bit value into the PCA0 Capture/Compare registers
void Update_PCA_Comparator(uint16_t compare_value)
{
    PCA0CPL0 = (compare_value & 0x00FF); // Load lower byte
    PCA0CPH0 = (compare_value >> 8);     // Load upper byte
}

// Basic software delay loop
void Simple_Wait(uint16_t duration)
{
    uint16_t cycle_a;
    volatile uint16_t cycle_b;

    for(cycle_a = 0; cycle_a < duration; cycle_a++)
    {
        // Internal loop tuned for approximate timing
        for(cycle_b = 0; cycle_b < 4000; cycle_b++);
    }
}

// ----------------------------------------------------------------------------
// Main Application
// ----------------------------------------------------------------------------
void main(void)
{
    enter_DefaultMode_from_RESET();

    // -- Hardware Configuration --
    XBR1 |= 0x40;      // Activate Crossbar to route signals
    PCA0CPM0 = 0xC2;   // Configure Channel 0 for 16-bit Pulse Width Modulation
    IE_EA = 1;         // Enable global interrupt handling

    // -- Initial State --
    // Set servo to neutral position immediately
    Update_PCA_Comparator(TICKS_AT_90_DEG);
    Simple_Wait(500);

    while (1)
    {
        // 1. Dual Press Detection
        // If both inputs are active, force return to neutral
        if ((PB0 == 0) && (PB1 == 0))
        {
            Update_PCA_Comparator(TICKS_AT_90_DEG);
            Simple_Wait(50);
        }
        // 2. Left Input Detected
        else if (PB0 == 0)
        {
            Update_PCA_Comparator(TICKS_AT_0_DEG);
        }
        // 3. Right Input Detected
        else if (PB1 == 0)
        {
            Update_PCA_Comparator(TICKS_AT_180_DEG);
        }

        // Stabilization delay between polling
        Simple_Wait(20);
    }
}
