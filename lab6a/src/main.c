/*
 * -------------------------------------------------------------------------------------
 * Lab 6a: Motor Control with Joystick Speed and Button Direction
 * 
 * EFM8UB2 + L293D Motor Driver
 * - BTN0: Clockwise direction
 * - BTN1: Counter-clockwise direction
 * - Joystick Y-axis (P2.5): Speed control (center=stop, up=faster)
 * -------------------------------------------------------------------------------------
 */
#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include <stdint.h>

//-----------------------------------------------------------------------------
// Definitions & Constants
//-----------------------------------------------------------------------------
// ADC thresholds for joystick (10-bit ADC, 0-1023 range)
#define JOYSTICK_CENTER     512    // Center position
#define JOYSTICK_DEADZONE   50     // Deadzone around center

// Button Pins (directly connected, directly usable)
SI_SBIT(BTN0, SFR_P0, 2);    // Clockwise button
SI_SBIT(BTN1, SFR_P0, 3);    // Counter-clockwise button

// H-Bridge Driver Pins (Direction control)
SI_SBIT(MOTOR_IN1, SFR_P0, 4);
SI_SBIT(MOTOR_IN2, SFR_P0, 5);

// Motor direction states
#define DIR_STOPPED    0
#define DIR_CLOCKWISE  1
#define DIR_COUNTERCLOCKWISE 2

// Global Variables
uint8_t motor_direction = DIR_STOPPED;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void SiLabs_Startup(void);
void Set_Duty_Cycle(uint8_t dc_percent);
void Delay_ms(uint16_t ms);
uint16_t Read_ADC(void);

//-----------------------------------------------------------------------------
// SiLabs_Startup
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
    PCA0MD &= ~0x40; // Disable Watchdog
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

// Simple Delay
void Delay_ms(uint16_t ms)
{
    uint16_t i;
    volatile uint16_t j;
    for(i = 0; i < ms; i++)
    {
        for(j = 0; j < 4000; j++);
    }
}

// Read ADC value from joystick (P2.5 = ADC0.13)
uint16_t Read_ADC(void)
{
    uint16_t result;
    
    // Start conversion
    ADC0CN0 |= ADC0CN0_ADBUSY__SET;
    
    // Wait for conversion to complete
    while (!(ADC0CN0 & ADC0CN0_ADINT__SET));
    
    // Clear the interrupt flag
    ADC0CN0 &= ~ADC0CN0_ADINT__SET;
    
    // Read 10-bit result (right-justified)
    result = (ADC0H << 8) | ADC0L;
    
    return result;
}

// Set PWM Duty Cycle (8-bit PWM mode)
// 0% = motor off, 100% = full speed
void Set_Duty_Cycle (uint8_t dc_percent)
{
    uint8_t cmp_val;

    if (dc_percent > 100) dc_percent = 100;

    // For 8-bit PWM: duty cycle = (256 - PCA0CPH0) / 256
    // PCA0CPH0 = 256 - (duty * 256 / 100)
    // At 0% duty: PCA0CPH0 = 0xFF (output always low)
    // At 100% duty: PCA0CPH0 = 0x00 (output always high)
    
    if (dc_percent == 0)
    {
        cmp_val = 0xFF;  // 0% duty
    }
    else if (dc_percent >= 100)
    {
        cmp_val = 0x00;  // 100% duty
    }
    else
    {
        cmp_val = (uint8_t)(256 - ((uint16_t)dc_percent * 256 / 100));
    }

    PCA0CPH0 = cmp_val;
}

//-----------------------------------------------------------------------------
// Set Motor Direction
//-----------------------------------------------------------------------------
void Set_Motor_Direction(uint8_t dir)
{
    // Stop PWM briefly when changing direction
    if (motor_direction != dir && motor_direction != DIR_STOPPED)
    {
        Set_Duty_Cycle(0);
        Delay_ms(100);  // Brief pause for direction change
    }
    
    motor_direction = dir;
    
    switch(dir)
    {
        case DIR_CLOCKWISE:
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 0;
            break;
        case DIR_COUNTERCLOCKWISE:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 1;
            break;
        case DIR_STOPPED:
        default:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            break;
    }
}

//-----------------------------------------------------------------------------
// Main Loop
//-----------------------------------------------------------------------------
void main (void)
{
    uint16_t adc_value;
    uint8_t speed_percent;
    uint8_t btn0_prev = 1, btn1_prev = 1;

    enter_DefaultMode_from_RESET();

    // Enable global interrupts (for timer-based functions if needed)
    IE_EA = 1;

    // Initialize motor to stopped
    motor_direction = DIR_STOPPED;
    Set_Motor_Direction(DIR_STOPPED);
    Set_Duty_Cycle(0);

    while (1)
    {
        // ==========================================
        // BTN0: Set direction to Clockwise
        // ==========================================
        if (BTN0 == 0 && btn0_prev == 1)
        {
            Set_Motor_Direction(DIR_CLOCKWISE);
            Delay_ms(50);  // Debounce
        }
        btn0_prev = BTN0;

        // ==========================================
        // BTN1: Set direction to Counter-clockwise
        // ==========================================
        if (BTN1 == 0 && btn1_prev == 1)
        {
            Set_Motor_Direction(DIR_COUNTERCLOCKWISE);
            Delay_ms(50);  // Debounce
        }
        btn1_prev = BTN1;

        // ==========================================
        // Joystick: Read speed from Y-axis
        // ==========================================
        adc_value = Read_ADC();
        
        // Map joystick to speed:
        // Center (512) = 0% speed (stopped)
        // Push up (>512+deadzone) = increase speed proportionally
        // Push down (<512-deadzone) = also increase speed (optional: reverse)
        
        if (adc_value > (JOYSTICK_CENTER + JOYSTICK_DEADZONE))
        {
            // Joystick pushed UP - calculate speed 0-100%
            // Range: 562 to 1023 maps to 0-100%
            speed_percent = (uint8_t)(((uint32_t)(adc_value - JOYSTICK_CENTER - JOYSTICK_DEADZONE) * 100) 
                                      / (1023 - JOYSTICK_CENTER - JOYSTICK_DEADZONE));
        }
        else if (adc_value < (JOYSTICK_CENTER - JOYSTICK_DEADZONE))
        {
            // Joystick pushed DOWN - also map to speed (same direction, or stop)
            // Range: 0 to 462 maps to 100-0%
            speed_percent = (uint8_t)(((uint32_t)(JOYSTICK_CENTER - JOYSTICK_DEADZONE - adc_value) * 100) 
                                      / (JOYSTICK_CENTER - JOYSTICK_DEADZONE));
        }
        else
        {
            // Joystick in center deadzone - stop
            speed_percent = 0;
        }
        
        // Apply speed if a direction is selected
        if (motor_direction != DIR_STOPPED)
        {
            Set_Duty_Cycle(speed_percent);
        }
        else
        {
            Set_Duty_Cycle(0);  // No direction = no movement
        }

        Delay_ms(20);  // Main loop delay
    }
}
