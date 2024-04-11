#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "cpu_main.h"

int main(int argc, char* argv[]) {
    
    t_log* logger = iniciar_log(logger, "CPU.log", "CPU");
	log_info(logger,"Hola! CPU");
    log_destroy(logger);
    return 0;
}
