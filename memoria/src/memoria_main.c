#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "memoria_main.h"

int main(int argc, char* argv[]) {
    logger = log_create("Memoria.log", "Memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! Memoria");
    log_destroy(logger);
    return 0;
}

