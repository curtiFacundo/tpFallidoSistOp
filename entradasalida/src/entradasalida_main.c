#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "entradasalida_main.h"

int main(int argc, char* argv[]) {
        
	/*
	TO DO list:
	
	*/

	char* ip;
	char* puerto;
	char* valor;
	t_config* config;

    logger = log_create("entradasalida.log", "entradasalida", 1, LOG_LEVEL_DEBUG);
    int socket_id = iniciar_servidor();
    config = iniciar_config();
    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
    int conexion = crear_conexion(ip, puerto, logger);
    crearConexionKernel(logger,conexion,ip,puerto,valor);
    log_destroy(logger);
    return 0;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("../utils/config/entradasalida.config");

	return nuevo_config;
}

int crearConexionKernel(t_log* logger,int conexion, char* ip, char* puerto,char* valor) {
        
        enviar_mensaje(valor, conexion);
        paquete(conexion);
    return 0;
}