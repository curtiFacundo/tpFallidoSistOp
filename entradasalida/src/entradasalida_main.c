#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "entradasalida_main.h"

int main(int argc, char* argv[]) {
    logger = log_create("entradasalida.log", "entradasalida", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! entradasalida");
    log_destroy(logger);
    return 0;
}