
// USER INCLUDES
#include "SI_EFM8UB2_Register_Enums.h"
#include "InitDevice.h"

//-----------------------------------------------------------------------------
// TIMER0_ISR
//-----------------------------------------------------------------------------
//
// TIMER0 ISR Content goes here. Remember to clear flag bits:
// TCON::TF0 (Timer 0 Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER0_ISR, TIMER0_IRQn)
{
    // איפוס דגל הפסיקה של טיימר 0
    TCON &= ~TCON_TF0__BMASK;  // או פשוט: TF0 = 0;

    // P2.6 שינוי מצב הפין
    P2 ^= 0x40;
}

SI_INTERRUPT (TIMER1_ISR, TIMER1_IRQn)
{
    // איפוס דגל הפסיקה של טיימר 1
    TCON &= ~TCON_TF1__BMASK;  // או פשוט: TF1 = 0;

    // P2.7 שינוי מצב הפין
    P2 ^= 0x80;
}
