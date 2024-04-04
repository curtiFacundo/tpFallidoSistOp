#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "cpu_main.h"

int main(int argc, char* argv[]) {
    t_log* logger;
    logger = log_create("logsCPU.log","logsCPU",true,LOG_LEVEL_INFO);
	log_info(logger,"Hola! CPU");
    log_destroy(logger);
    return 0;
}
