#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include <stdint.h>
#include "tick.h"

// --- FREQUENCY SETTINGS ---
#define DELAY_MIN   500     // Maximum Frequency
#define DELAY_MAX   60000   // Minimum Frequency
#define DELAY_STEP  500     // 10% equivalent step (approx)

// Joystick Thresholds (from provided reference)
#define JOY_LEFT_MIN   580   // ~1.9V
#define JOY_LEFT_MAX   650   // ~2.08V
#define JOY_RIGHT_MIN  740   // ~2.4V
#define JOY_RIGHT_MAX  810   // ~2.6V

// Global variable for frequency timing
volatile uint16_t timer_delay = 30000;

// --- PIN DEFINITIONS ---
SI_SBIT(M1,   SFR_P0, 4); // Motor Phase 1
SI_SBIT(M2,   SFR_P0, 5); // Motor Phase 2
SI_SBIT(BTN0, SFR_P0, 2); // Physical Button Right
SI_SBIT(BTN1, SFR_P0, 3); // Physical Button Left

// --- 1. PCA INTERRUPT FOR FREQUENCY OUTPUT ---
// This handles the "Toggle" logic to create a precise frequency
SI_INTERRUPT (PCA0_ISR, PCA0_IRQn)
{
    uint16_t next_val;
    if (PCA0CN0_CCF0)
    {
        PCA0CN0_CCF0 = 0; // Clear flag

        // Calculate next toggle point based on current timer value + delay
        next_val = (PCA0CPH0 << 8) | PCA0CPL0;
        next_val += timer_delay;

        // Schedule the next toggle
        PCA0CPL0 = (next_val & 0x00FF);
        PCA0CPH0 = (next_val >> 8);
    }
}

void Init_Hardware_Manual(void)
{
    PCA0MD &= ~0x40; // Disable Watchdog

    // Port Configuration
    P0MDOUT |= 0x30; // P0.4, P0.5 Motor Outputs
    P1MDOUT |= 0x02; // P1.1 Frequency Output (Push-Pull)

    // Joystick on P2.5 (Analog)
    P2MDIN  &= ~0x20;
    P2MDOUT &= ~0x20;
    P2      |= 0x20;

    // ADC Setup
    ADC0CN0 |= 0x80;
    AMX0P = 0x0D;    // Select P2.5
    AMX0N = 0x1F;

    // PCA Setup for Frequency Output (Mode 0x4D)
    PCA0MD   |= 0x08; // System Clock
    PCA0CPM0  = 0x4D; // Toggle + Match + Interrupt Enable
    PCA0CN0  |= 0x40; // Enable PCA Timer

    // Crossbar: Route CEX0 to P1.1
    P0SKIP = 0xFF;
    P1SKIP = 0x01;
    XBR1   = 0x41;
}

void SILABS_STARTUP (void) { PCA0MD &= ~0x40; }

uint16_t Read_Joystick(void)
{
    ADC0CN0_ADBUSY = 1;
    while (!ADC0CN0_ADINT);
    ADC0CN0_ADINT = 0;
    return ADC0;
}

void main (void)
{
    uint16_t joy_val;

    enter_DefaultMode_from_RESET();
    Init_Hardware_Manual();

    // Enable Global and PCA Interrupts
    EIE1 |= 0x10;
    IE_EA = 1;

    M1 = 0; M2 = 0; // Start stopped

    while (1)
    {
        joy_val = Read_Joystick();

        // --- FREQUENCY CONTROL (JOYSTICK & BUTTONS) ---

        // INCREASE FREQUENCY (Right Joystick or BTN0)
        // Frequency UP = timer_delay DOWN
        if ((joy_val > JOY_RIGHT_MIN && joy_val < JOY_RIGHT_MAX) || (BTN0 == 0))
        {
            if (timer_delay > (DELAY_MIN + DELAY_STEP))
            {
                timer_delay -= DELAY_STEP;
            }
            Wait(200); // Step delay
        }

        // DECREASE FREQUENCY (Left Joystick or BTN1)
        // Frequency DOWN = timer_delay UP
        else if ((joy_val > JOY_LEFT_MIN && joy_val < JOY_LEFT_MAX) || (BTN1 == 0))
        {
            if (timer_delay < (DELAY_MAX - DELAY_STEP))
            {
                timer_delay += DELAY_STEP;
            }
            Wait(200); // Step delay
        }

        // --- MOTOR DIRECTION LOGIC ---
        // You can keep your BTN logic here if they are used for direction,
        // or use the 'Up'/'Down' voltage levels from your image for M1/M2.
    }
}
