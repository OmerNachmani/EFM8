#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include "tick.h"
#include <stdint.h>

// ===== Joystick driver =====
// ברוב המעבדות זה אחד מהבאים. תשאיר רק את ה-include הזה:
#include "joystick.h"

// ------------------------------------------------------------
// CONFIG: choose joystick function name (רק אם עדיין יש WARNING/ERROR)
// ------------------------------------------------------------
// אם אתה מקבל WARNING של "missing function-prototype" או ERROR של undefined symbol,
// אז תבחר פה נכון לפי מה שיש אצלך ב-joystick.h
//
// אופציה A (נפוץ):
uint8_t JOYSTICK_getDirection(void);
// אופציה B (אם אצלך הפונקציה נקראת אחרת) — אז תחליף את השורה מעל לזה:
// uint8_t JOYSTICK_directionGet(void);
// ותעדכן גם למטה בשורה של JOY_READ_DIR()

// מאקרו לקריאה אחידה:
#define JOY_READ_DIR()   (JOYSTICK_getDirection())
// אם אתה עובר לאופציה B:
// #define JOY_READ_DIR()   (JOYSTICK_directionGet())

// ------------------------------------------------------------
// Direction mapping (מכסה שמות שונים של הדרייבר)
// ------------------------------------------------------------
#if defined(JOYSTICK_E)
  #define JOY_RIGHT   JOYSTICK_E
#elif defined(JOYSTICK_EAST)
  #define JOY_RIGHT   JOYSTICK_EAST
#elif defined(JOYSTICK_DIR_E)
  #define JOY_RIGHT   JOYSTICK_DIR_E
#elif defined(JOYSTICK_DIR_EAST)
  #define JOY_RIGHT   JOYSTICK_DIR_EAST
#else
  #define JOY_RIGHT   0xFF   // אם לא נמצא, לא יעבוד עד שתתקן לפי ה-header
#endif

#if defined(JOYSTICK_W)
  #define JOY_LEFT    JOYSTICK_W
#elif defined(JOYSTICK_WEST)
  #define JOY_LEFT    JOYSTICK_WEST
#elif defined(JOYSTICK_DIR_W)
  #define JOY_LEFT    JOYSTICK_DIR_W
#elif defined(JOYSTICK_DIR_WEST)
  #define JOY_LEFT    JOYSTICK_DIR_WEST
#else
  #define JOY_LEFT    0xFE
#endif

// ------------------------------------------------------------
// User settings
// ------------------------------------------------------------
#define DUTY_STEP        5     // כמה כל "קליק" ג'ויסטיק משנה מהירות (%)
#define DUTY_START       30    // מהירות התחלתית (%)
#define JOY_DELAY_MS     120   // כדי שלא יהיה מהיר מדי
#define BTN_DEBOUNCE_MS  250
#define DIR_SWITCH_PAUSE 50    // עצירה קטנה לפני החלפת כיוון

// ------------------------------------------------------------
// Buttons (לפי הלוח שלכם)
SI_SBIT(BTN0, SFR_P0, 2);
SI_SBIT(BTN1, SFR_P0, 3);

// Motor direction pins (לפי מצגת 8: P0.4,P0.5)
SI_SBIT(M1, SFR_P0, 4);   // -> L293D IN1 (Pin 2)
SI_SBIT(M2, SFR_P0, 5);   // -> L293D IN2 (Pin 7)

typedef enum { DIR_RIGHT = 0, DIR_LEFT = 1 } motor_dir_t;

// ------------------------------------------------------------
// Motor direction control
// ------------------------------------------------------------
static void Motor_SetDir(motor_dir_t dir)
{
    if (dir == DIR_RIGHT) { M1 = 1; M2 = 0; }
    else                  { M1 = 0; M2 = 1; }
}

// ------------------------------------------------------------
// PWM control (PCA module 0, 8-bit PWM)
// note: duty% -> PCA0CPH0 mapping
// ------------------------------------------------------------
static void PWM_SetDuty_0_100(uint8_t duty_percent)
{
    uint16_t cp;

    if (duty_percent > 100) duty_percent = 100;

    // 100% => 0, 0% => 255
    cp = 256 - ((uint16_t)duty_percent * 256) / 100;
    if (cp > 255) cp = 255;

    PCA0CPH0 = (uint8_t)cp;
}

void main(void)
{
    motor_dir_t dir;
    uint8_t duty;
    uint8_t jdir;

    enter_DefaultMode_from_RESET();
    IE_EA = 1;

    // P0.4,P0.5 output push-pull (כיוון)
    P0MDOUT |= (1<<4) | (1<<5);

    // PCA PWM on module 0:
    // ECOM=1 + PWM=1 -> 0x42
    PCA0CPM0 = 0x42;

    // Start PCA counter
    PCA0CN0_CR = 1;

    // init state
    dir = DIR_RIGHT;
    duty = DUTY_START;

    Motor_SetDir(dir);
    PWM_SetDuty_0_100(duty);

    while (1)
    {
        // ---------------- Buttons: direction ----------------
        if (BTN0 == 0) {
            PWM_SetDuty_0_100(0);
            Wait(DIR_SWITCH_PAUSE);

            dir = DIR_RIGHT;
            Motor_SetDir(dir);

            PWM_SetDuty_0_100(duty);
            Wait(BTN_DEBOUNCE_MS);
        }

        if (BTN1 == 0) {
            PWM_SetDuty_0_100(0);
            Wait(DIR_SWITCH_PAUSE);

            dir = DIR_LEFT;
            Motor_SetDir(dir);

            PWM_SetDuty_0_100(duty);
            Wait(BTN_DEBOUNCE_MS);
        }

        // ---------------- Joystick: speed ----------------
        jdir = JOY_READ_DIR();

        // אם JOY_RIGHT/JOY_LEFT לא זוהו (נשארו 0xFF/0xFE),
        // זה אומר שהשמות אצלך שונים -> צריך להסתכל ב-joystick.h ולהוסיף למיפוי למעלה.
        if (jdir == JOY_RIGHT) {
            if (duty + DUTY_STEP <= 100) duty += DUTY_STEP;
            PWM_SetDuty_0_100(duty);
            Wait(JOY_DELAY_MS);
        }
        else if (jdir == JOY_LEFT) {
            if (duty >= DUTY_STEP) duty -= DUTY_STEP;
            PWM_SetDuty_0_100(duty);
            Wait(JOY_DELAY_MS);
        }
    }
}
