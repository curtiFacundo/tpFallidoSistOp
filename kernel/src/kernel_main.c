#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "kernel_main.h"

int main(int argc, char* argv[]) {
    
    t_log* logger = iniciar_log(logger, "Kernel.log", "Kernel");
    log_info(logger,"Hola! Kernel");
    log_destroy(logger);
    return 0;
}
