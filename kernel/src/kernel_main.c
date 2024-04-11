#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "kernel_main.h"

int main(int argc, char* argv[]) {
    
    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    int socket_id = iniciar_servidor();
    log_info(logger,"Hola! Kernel");
    log_destroy(logger);
    
    return 0;
}
