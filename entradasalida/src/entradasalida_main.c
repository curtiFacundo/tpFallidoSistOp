#include <stdlib.h>
#include <stdio.h>
#include <utils/hello.h>
#include "entradasalida_main.h"

int main(int argc, char* argv[]) {
    t_log* logger = iniciar_log(logger, "entradasalida.log", "entradasalida");
	log_info(logger,"Hola! entradasalida");
    log_destroy(logger);
    return 0;
}