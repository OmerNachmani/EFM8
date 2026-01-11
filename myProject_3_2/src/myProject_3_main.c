#include "SI_EFM8UB2_Register_Enums.h"
#include "InitDevice.h"

//
// Bit-addressable definitions
// ----------------------------------------------------
sbit OUT0 = P2^6;    // Output for Timer0 (P2.6)
sbit OUT1 = P2^7;    // Output for Timer1 (P2.7)

// Timer0 bits in TCON
sbit TF0 = TCON^5;   // Timer0 overflow flag
sbit TR0 = TCON^4;   // Timer0 run control

// Timer1 bits in TCON
sbit TF1 = TCON^7;   // Timer1 overflow flag
sbit TR1 = TCON^6;   // Timer1 run control

// ----------------------------------------------------
// Startup (empty)
// ----------------------------------------------------
void SiLabs_Startup (void)
{
    // No startup code required
}

// ----------------------------------------------------
// MAIN
// ----------------------------------------------------
void main(void)
{
    // Initialize hardware from .hwconf
    enter_DefaultMode_from_RESET();

    // Clear outputs
    OUT0 = 0;   // P2.6 start low
    OUT1 = 0;   // P2.7 start low

    // ------------------------------------------------
    // Load Timer1 reload values (calculated)
    // ------------------------------------------------
    TH1 = 0xFD;     // High byte
    TL1 = 0x12;     // Low byte

    // Enable interrupts
    IE_EA = 1;      // Global interrupt enable
    IE_ET0 = 1;     // Enable Timer0 interrupt
    IE_ET1 = 1;     // Enable Timer1 interrupt

    // Start timers
    TR0 = 1;        // Start Timer0
    TR1 = 1;        // Start Timer1

    while (1)
    {
        // Work done in interrupts
    }
}

// ----------------------------------------------------
// TIMER0 ISR - toggle P2.6
// ----------------------------------------------------
SI_INTERRUPT(TIMER0_ISR, TIMER0_IRQn)
{
    TF0 = 0;        // Clear Timer0 overflow flag
    P2 ^= 0x40;     // Toggle P2.6
}

// ----------------------------------------------------
// TIMER1 ISR - toggle P2.7
// ----------------------------------------------------
SI_INTERRUPT(TIMER1_ISR, TIMER1_IRQn)
{
    TF1 = 0;        // Clear Timer1 overflow flag
    P2 ^= 0x80;     // Toggle P2.7
}
