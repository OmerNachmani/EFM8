#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "tick.h"
#include "render.h"
#include "stdio.h"
#include "SI_EFM8UB2_Register_Enums.h"

/* --- Hardware Constants --- */
#define REFERENCE_VOLTAGE   3300    // 3.3V represented in mV
#define ADC_RESOLUTION      1023    // Maximum value for a 10-bit ADC
#define ROW_SPACING         8       // Height offset for text rendering

/* Memory allocation for the display buffer */
SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);

/* --- Peripheral Bit Mapping --- */
sbit ADC_COMPLETE_FLAG = ADC0CN0 ^ 5;   // Hardware flag for conversion end
sbit START_TIMER_0     = TCON ^ 4;      // Control bit to run/stop Timer0

void main(void)
{
    /* --- Local Variable Setup --- */
    uint16_t raw_reading;               // To store the integer result from ADC
    float calculated_volts;             // To store the final floating-point value
    char display_string[DISP_BUF_SIZE]; // Array to hold the formatted text
    uint8_t i;                          // Loop counter for screen rendering

    /* --- Peripheral Startup Sequence --- */
    enter_DefaultMode_from_RESET();     // Load hardware configuration
    DISP_Init();                        // Wake up the LCD controller
    DISP_ClearAll();                    // Wipe the screen memory

    /* --- Static Text Placement --- */
    // Renders the group identifier once at the start
    for (i = 0; i < ROW_SPACING; i++)
    {
        RENDER_StrLine(line, 3, i, "We are group 6");
        DISP_WriteLine(25 + i, line);
    }

    /* --- Safety & Interrupt Config --- */
    PCA0MD &= ~0x40;                    // Watchdog must be off to prevent resets
    IE_EA = 1;                          // Global interrupt enable

    /* --- Begin Sampling --- */
    START_TIMER_0 = 1;                  // Kick off the ADC trigger source

    /* --- Infinite Monitoring Loop --- */
    while (1)
    {
        /* Check if the hardware has finished a new sample */
        if (ADC_COMPLETE_FLAG)
        {
            /* Grab the 10-bit value from the data register */
            raw_reading = ADC0;

            /* Scale raw data: (Value * 3300mV) / 1023 / 1000 to get Volts */
            calculated_volts = ((float)raw_reading * REFERENCE_VOLTAGE) / ADC_RESOLUTION / 1000.0;

            /* Format the float into a string for the display */
            sprintf(display_string, "Voltage: %.2f V", calculated_volts);

            /* Refresh the voltage reading on the LCD */
            for (i = 0; i < ROW_SPACING; i++)
            {
                RENDER_StrLine(line, 3, i, display_string);
                DISP_WriteLine(4 + i, line);
            }

            /* Reset the flag to wait for the next trigger */
            ADC_COMPLETE_FLAG = 0;
        }
    }
}
