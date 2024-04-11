#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "kernel_main.h"

int main(int argc, char* argv[]) {
    
    int socket_id = iniciar_servidor();
    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    log_info(logger,"Hola! Kernel");
    log_destroy(logger);
    
    return 0;
}
