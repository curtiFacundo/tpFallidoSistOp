#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "memoria_main.h"

int main(int argc, char* argv[]) {
    t_log* logger = iniciar_log(logger, "Memoria.log", "Memoria");
	log_info(logger,"Hola! Memoria");
    log_destroy(logger);
    return 0;
}

