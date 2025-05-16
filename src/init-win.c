#include <windows.h>
#include "init.h"

/**
 * Handles Ctrl+C.
 */
BOOL WINAPI HandlerRoutine(DWORD ctrlType)
{
    if (ctrlType == CTRL_C_EVENT)
    {
        printf("\x1b[?25h\x1b[?1049l");
        fflush(stdout);
        exit(0);
    }
    return FALSE;
}

void initialize()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(console, &mode))
    {
        SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);
}