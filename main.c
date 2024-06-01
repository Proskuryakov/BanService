#include <stdio.h>
#include "controller/routes.h"

static int s_debug_level = MG_LL_INFO;
static const char *s_listening_address = "http://0.0.0.0:8000";

// Handle interrupts, like Ctrl-C
static int s_signo;
static void signal_handler(int signo) {
    s_signo = signo;
}

int main(void) {
    struct mg_mgr mgr;
    struct mg_connection *c;

    mg_mgr_init(&mgr);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    mg_log_set(s_debug_level);

    if ((c = mg_http_listen(&mgr, s_listening_address, route_handler, &mgr)) == NULL) {
        MG_ERROR(("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
                s_listening_address));
        exit(EXIT_FAILURE);
    }

    printf("Starting web server on port 8000\n");

    enable_controllers();

    while (s_signo == 0) mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
    MG_INFO(("Exiting on signal %d", s_signo));
    free_controllers();
    return 0;
}