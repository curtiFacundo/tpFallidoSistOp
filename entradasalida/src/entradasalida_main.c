#include <stdlib.h>
#include <stdio.h>
#include "entradasalida_main.h"

int main(int argc, char* argv[]) {
        
	/*
	TO DO list:
	
	*/
    
	char* ip;
	char* puerto;
	char* valor;
	char* mensaje;
	t_config* config;
	t_list *handshake;

    logger = log_create("entradasalida.log", "entradasalida", 1, LOG_LEVEL_DEBUG);
    //int socket_id = iniciar_servidor();
    config = config_create("../utils/config/entradasalida.config");
    // SERVER KERNEL
    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTOKERNEL");
	valor = config_get_string_value(config, "CLAVE");
    
    int server_fd_kernel = iniciar_servidor(puerto);
	log_info(logger, "Servidor I/O listo para recibir al cliente Kernel");
	int cliente_fd_kernel = esperar_cliente(server_fd_kernel);
	int cod_op = recibir_operacion(cliente_fd_kernel);

	switch (cod_op)
	{
	case HANDSHAKE:
		handshake = recibir_paquete(cliente_fd_kernel);
		log_info(logger, "me llego:\n");
		list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
		break;
	case -1:
			log_error(logger, "el cliente Kernel se desconecto. Terminando servidor I/O");
			return EXIT_FAILURE;
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
	// FIN SERVER KERNEL

    // SERVER MEMORIA
    ip = config_get_string_value(config, "IPMEMORIA");
    puerto = config_get_string_value(config, "PUERTOMEMORIA");
	valor = config_get_string_value(config, "CLAVE");
    
    int server_fd_memoria = iniciar_servidor(puerto);
	log_info(logger, "Servidor I/O listo para recibir al cliente Memoria");
	int cliente_fd_memoria = esperar_cliente(server_fd_memoria);
	cod_op = recibir_operacion(cliente_fd_memoria);

	switch (cod_op)
	{
	case HANDSHAKE:
		handshake = recibir_paquete(cliente_fd_memoria);
		log_info(logger, "me llego:\n");
		list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
		break;
	case -1:
			log_error(logger, "el cliente Memoria se desconecto. Terminando servidor I/O");
			return EXIT_FAILURE;
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
	// FIN SERVER MEMORIA


	
	// t_paquete *crear_paquete(HANDSHAKE);
	// agregar_a_paquete(valor)

	terminar_programa(server_fd_kernel, logger, config); //logger: redundante (global) pero esta definido asi en utils.h
	close(cliente_fd_kernel);

	close(server_fd_memoria);
	close(cliente_fd_memoria);
    return 0;
}

