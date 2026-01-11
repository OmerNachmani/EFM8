//-----------------------------------------------------------------------------
// LAB5 - FINAL
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "tick.h"
#include "render.h"
#include <stdio.h>
#include <string.h>

#define LCD_height 128
#define LCD_width  128

// ADC Control Bits
sbit FlagFlag = ADC0CN0^5; // ADINT (Conversion Done Flag)
sbit ADBUSY   = ADC0CN0^4; // ADBUSY (Start Conversion)

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
    // Explicitly disable the Watchdog Timer
    PCA0MD &= ~0x40;
}

//-----------------------------------------------------------------------------
// Init_ADC_Force()
//-----------------------------------------------------------------------------
void Init_ADC_Force(void)
{
    // 1. SELECT REFERENCE VOLTAGE (VDD)
    REF0CN = 0x08;

    // 2. SET ADC CLOCK
    // Your InitDevice used divider 7. We will use a safe divider of 4.
    ADC0CF = (0x04 << 3);

    // 3. REMOVED ADC0AC (Does not exist on EFM8UB2)

    // 4. ENABLE ADC POWER & SET MANUAL MODE
    // Your InitDevice set ADCM to Timer0 (0x02).
    // We MUST set ADCM to 0x00 for ADBUSY=1 to work.
    // 0x80 = Enable (Bit 7) | Manual Mode (Bits 2-0 are 000)
    ADC0CN0 = 0x80;
}

int main(void)
{
    // --- VARIABLES ---
    SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG);
    int i = 0;
    long timeout_counter = 0;
    float digitalValue;
    float Vin;
    char buffer[20];
    char direct[20];
    // -----------------

    enter_DefaultMode_from_RESET();

    // --- FORCE FIX ---
    Init_ADC_Force();

    // Disable ADC Interrupt (Use Polling)
    EIE1 &= ~0x02;

    // Enable Global Interrupts (for display)
    IE_EA = 1;

    DISP_Init();
    DISP_ClearAll();

    // Initial Debug Message
    for (i = 0; i < FONT_HEIGHT; i++)
    {
        RENDER_StrLine(line, 3, i, "Group 2");
        DISP_WriteLine(0 + i, line);
    }

    FlagFlag = 0;

    while(1)
    {
        // 1. Start Conversion
        // Now that ADCM is 0 (set by Init_ADC_Force), this will actually work.
        ADBUSY = 1;

        // 2. Wait for completion (Polling with timeout)
        timeout_counter = 0;
        while(FlagFlag == 0)
        {
            timeout_counter++;
            // Emergency break if hardware is stuck
            if(timeout_counter > 50000) break;
        }

        // 3. Handle Timeout
        if (timeout_counter > 50000)
        {
             for (i = 0; i < FONT_HEIGHT; i++)
            {
                RENDER_StrLine(line, 3, i, "ADC TIMEOUT");
                DISP_WriteLine(10 + i, line);
            }
            // Attempt restart
            ADC0CN0 = 0x80;
            FlagFlag = 0;
            continue;
        }

        // 4. Process Data
        digitalValue = (float)ADC0;
        Vin = (digitalValue / 1023.0) * 3.3;

        FlagFlag = 0;

        // --- DISPLAY LOGIC ---

        // Voltage
        for (i = 0; i < FONT_HEIGHT; i++)
        {
            sprintf(buffer, "%.3f(V)", Vin);
            RENDER_StrLine(line, 3, i, buffer);
            DISP_WriteLine(10 + i, line);
        }

        // Direction
        strcpy(direct, "No Press");
        if(Vin < 0.04)                     strcpy(direct, "Center");
        else if (Vin > 1.5 && Vin < 1.9)   strcpy(direct, "Down");
        else if (Vin > 1.9 && Vin < 2.08)  strcpy(direct, "Left");
        else if (Vin > 2.4 && Vin < 2.6)   strcpy(direct, "Right");
        else if (Vin > 2.73 && Vin < 2.93) strcpy(direct, "Up");

        for(i = 0; i < FONT_HEIGHT; i++)
        {
            RENDER_StrLine(line, 3, i, direct);
            DISP_WriteLine(20 + i, line);
        }
    }
}
