#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "main.h"

int main(int argc, char* argv[]) {
    t_log* logger;
    logger = log_create("logsKernel.log","logsKernel",true,LOG_LEVEL_INFO);
	log_info(logger,"Hola! Kernel");
    log_destroy(logger);
    return 0;
}
