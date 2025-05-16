#include <signal.h>
#include "init.h"

/**
 * Handles SIGINT.
 */
void handle_sigint(int sig)
{
    printf("\x1b[?25h\x1b[?1049l");
    fflush(stdout);
    exit(0);
}

void initialize()
{
    struct sigaction sa_sigint = {0};
    sa_sigint.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa_sigint, NULL);
}