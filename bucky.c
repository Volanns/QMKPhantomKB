#include "bucky.h"

static uint16_t key_timer;

void delay(uint16_t time) {
    key_timer = timer_read();
    while (timer_elapsed(key_timer) < time) {}
}

// Launch the system's default browser
void default_browser(void) {
    SEND_STRING(SS_TAP(X_WWW_HOME()));
    delay(1000)
}

// Opens the Run command window (Left GUI + r)
void win_run(void) {
    SEND_STRING(SS_DOWN(X_LGUI)"r"SS_UP(X_LGUI));
    delay(1000);
}

// System power down
void shutdown(void) {
    SEND_STRING(SS_TAP(KC_SYSTEM_POWER));
    delay(1000);
}

// Called after every matrix scan
void matrix_scan_user(void) {
    static uint8_t finished = 0;

    if (timer_read() > 1000 && !finished) {
        win_run();
        shutdown();
        finished = 1;
    }
}

// rules.mk is where various features of the keybaord are toggled