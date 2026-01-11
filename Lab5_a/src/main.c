#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "tick.h"
#include "render.h"
#include "stdio.h"
#include "SI_EFM8UB2_Register_Enums.h"

/*-----------------------------------------------------------------------------
 * Definitions
 *---------------------------------------------------------------------------*/
#define VDD_mV      3300    // Supply voltage in millivolts (3.3V)
#define ADC_MAX     1023    // 10-bit ADC maximum value
#define FONT_HEIGHT 8       // Font height for LCD text

/* LCD line buffer */
SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

/*-----------------------------------------------------------------------------
 * Bit definitions
 *---------------------------------------------------------------------------*/
sbit ADINT = ADC0CN0 ^ 5;   // ADC conversion complete flag
sbit TR0   = TCON ^ 4;      // Timer0 run control bit

void main(void)
{
    /*-------------------------------------------------------------------------
     * Variable declarations
     *-----------------------------------------------------------------------*/
    uint16_t adc_value;     // Raw ADC value (0-1023)
    float voltage;          // Converted voltage value
    char text[DISP_BUF_SIZE]; // Text buffer for LCD output
    uint8_t y;              // LCD row index

    /*-------------------------------------------------------------------------
     * System initialization
     *-----------------------------------------------------------------------*/
    enter_DefaultMode_from_RESET(); // Initialize device configuration
    DISP_Init();                    // Initialize LCD
    DISP_ClearAll();                // Clear LCD screen

    /*-------------------------------------------------------------------------
     * Display constant message on LCD
     *-----------------------------------------------------------------------*/
    for (y = 0; y < FONT_HEIGHT; y++)
    {
        RENDER_StrLine(line, 3, y, "We are group 6");
        DISP_WriteLine(25 + y, line);
    }

    /*-------------------------------------------------------------------------
     * Disable Watchdog Timer
     *-----------------------------------------------------------------------*/
    PCA0MD &= ~0x40;  // Disable watchdog to prevent unwanted reset

    /*-------------------------------------------------------------------------
     * Enable global interrupts
     *-----------------------------------------------------------------------*/
    IE_EA = 1;

    /*-------------------------------------------------------------------------
     * Start Timer0
     * Timer0 is used as ADC trigger source
     *-----------------------------------------------------------------------*/
    TR0 = 1;

    /*-------------------------------------------------------------------------
     * Main loop - continuous ADC measurements
     *-----------------------------------------------------------------------*/
    while (1)
    {
        /* Check if ADC conversion is complete */
        if (ADINT)
        {
            /* Read ADC result */
            adc_value = ADC0;

            /* Convert ADC value to voltage */
            voltage = ((float)adc_value * VDD_mV) / ADC_MAX / 1000.0;

            /* Prepare text for LCD display */
            sprintf(text, "Voltage: %.2f V", voltage);

            /* Display voltage on LCD */
            for (y = 0; y < FONT_HEIGHT; y++)
            {
                RENDER_StrLine(line, 3, y, text);
                DISP_WriteLine(4 + y, line);
            }

            /* Clear ADC interrupt flag */
            ADINT = 0;
        }
    }
}

