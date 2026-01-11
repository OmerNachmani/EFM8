#include <SI_EFM8UB2_Register_Enums.h> // SFR declarations for EFM8UB2
#include "InitDevice.h"                 // Device initialization prototypes
sbit OUTPIN = P2^6;                     // LED output pin connected to P2.6
sbit TF0    = TCON^5;                   // Timer0 overflow flag (TF0 bit in TCON)
sbit TR0    = TCON^4;                   // Timer0 run control bit (TR0 bit in TCON)
//-----------------------------------------------------------------------------
// SiLabs_Startup
//-----------------------------------------------------------------------------
// This function is called immediately after reset, before main().
// It is kept empty here because all initialization is done in
// enter_DefaultMode_from_RESET().
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
    // No special startup code required
}
//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
// Configure the device, start Timer0, and toggle OUTPIN (P2.6) every time
// Timer0 overflows (TF0 = 1). The program runs in an infinite loop.
//-----------------------------------------------------------------------------
int main (void)
{
    // Initialize device (clock, ports, Timer0 mode, etc.)
    enter_DefaultMode_from_RESET();
    // Load Timer0 high and low bytes with initial value
    // These values define the delay until the next overflow
    TH0 = 0xB5;
    TL0 = 0xB5;
    // Start Timer0 (set run control bit)
    TR0 = 1;
    // Main loop - poll Timer0 overflow flag
    while (1)
    {
        // Check if Timer0 overflow occurred
        if (TF0)
        {
            // Toggle LED output on P2.6
            OUTPIN = ~OUTPIN;

            // Clear Timer0 overflow flag so we can detect the next overflow
            TF0 = 0;
        }
    }
}
