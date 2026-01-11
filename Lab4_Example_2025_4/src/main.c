//-----------------------------------------------------------------------------
// LAB4 - LCD Text Display
// MCU: EFM8UB20F64G-B-QFP48
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Resources
//-----------------------------------------------------------------------------
// SYSCLK - 48MHz HFOSC / 2
// SPI0   - 1MHz
// Timer2 - 2MHz (SPI CS delay)
// Timer3 - 1KHz (1ms tick)
// P0.1   - CS (Active High)
// P0.6   - SCK
// P1.0   - MOSI
// P1.4   - Display enable
//-----------------------------------------------------------------------------

#include "bsp.h"
#include "InitDevice.h"
#include "disp.h"
#include "tick.h"
#include "render.h"

#define LCD_height 128

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
SI_SEGMENT_VARIABLE(line[DISP_BUF_SIZE], uint8_t, RENDER_LINE_SEG); // LCD Line buffer

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // Watchdog disabled via Configurator
}

//-----------------------------------------------------------------------------
// write_text_row()
// Description: Writes a string to a specific text row index.
// text_row: Row index (0-15)
// col: Starting pixel column
// text: String to display
//-----------------------------------------------------------------------------
void write_text_row(uint8_t text_row, uint8_t col, char* text)
{
  uint8_t y;
  uint8_t i;
  uint8_t base_row;

  // Convert text row index to absolute pixel row
  base_row = text_row * FONT_HEIGHT;

  for (y = 0; y < FONT_HEIGHT; y++)
  {
    // Clear buffer before rendering new slice
    for (i = 0; i < DISP_BUF_SIZE; i++)
    {
      line[i] = 0x00;
    }

    // Render the specific font slice (y) into the buffer
    RENDER_StrLine(line, col, y, text);

    // Write the buffer to the physical LCD line
    DISP_WriteLine(base_row + y, line);
  }
}

//-----------------------------------------------------------------------------
// main()
//-----------------------------------------------------------------------------
int main(void)
{
  // Hardware Initialization
  enter_DefaultMode_from_RESET();

  // Enable global interrupts
  IE_EA = 1;

  // Initialize LCD and clear screen
  DISP_Init();
  DISP_ClearAll();

  // --- Display Text ---

  // Row 0: Title
  write_text_row(0, 0, "Shenkar COLLEGE");

  // Row 10: Student Name
  write_text_row(9, 0, "Eden Eli");
  write_text_row(10, 0, "Omer Nachmani");
  write_text_row(11, 0, "Erez with E");


  while (1)
  {
     // Main loop
  }
}
