#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "cpu_main.h"

int main(int argc, char* argv[]) {
    
    logger = log_create("CPU.log", "CPU", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    log_destroy(logger);
    return 0;
}
