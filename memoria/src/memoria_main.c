#include <stdlib.h>
#include <stdio.h>
#include "memoria_main.h"

int Saludar(void) {
    logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    return 0;
}

int main(void) {
	int conexion;
	char* ip;
	char* puerto;
	char* valor;
	t_config* config;
	    
	/*
	TO DO list:
	
	*/

	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "listo para recibir");
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
	
	//conexión
	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	conexion = crear_conexion(ip, puerto, logger);
	enviar_mensaje(valor, conexion);
	paquete(conexion);
	//


}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("../utils/config/memoria.config");

	return nuevo_config;
}



// t_paquete *crear_paquete(HANDSHAKE);
// agregar_a_paquete(valor);

void iterator(char* value) {
	log_info(logger,"%s", value);
}