#include <stdlib.h>
#include <stdio.h>
#include <utils/utils.h>
#include "kernel_main.h"

int main(int argc, char* argv[]) {
    
	/*
	TO DO list:
	
	*/

    int conexion;
	char* ip;
	char* puerto;
	char* valor;
	t_config* config;

    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    int socket_id = iniciar_servidor();
    config = iniciar_config();
    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
    //serServidorIO();
    crearConexionCPU(logger,conexion,ip,puerto,valor);
    log_destroy(logger);
    return 0;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("../utils/config/kernel.config");

	return nuevo_config;
}

int crearConexionCPU(t_log* logger,int conexion, char* ip, char* puerto,char* valor) {
        conexion = crear_conexion(ip, puerto, logger);
        enviar_mensaje(valor, conexion);
        paquete(conexion);
    return 0;
}

int serServidorIO(void) {
	//logger = log_create("CPU.log", "CPU", 1, LOG_LEVEL_DEBUG); es necesario? aparte abre un log de CPU en el modulo kernel

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	log_destroy(logger);
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}