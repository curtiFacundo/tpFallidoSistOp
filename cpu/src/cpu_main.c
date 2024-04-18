#include <stdlib.h>
#include <stdio.h>
#include "cpu_main.h"

int main(void) {
    
	/*
	TO DO list:
	
	*/

	logger = log_create("cpu.log", "cpu", 1, LOG_LEVEL_DEBUG); //movido al mail desde abrirServerCPU
	
	char* ip;
	char* puerto;
	char* valor;
	char* mensaje;
	t_config* config;
	t_list *handshake;

    //int socket_id = iniciar_servidor();
    config = config_create("../utils/config/cpu.config");

    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTOMEMORIA");
	valor = config_get_string_value(config, "CLAVE");

	int server_fd_memoria = iniciar_servidor(puerto);
	log_info(logger, "Servidor CPU listo para recibir al cliente MEMORIA");
	int cliente_fd_memoria = esperar_cliente(server_fd_memoria);
	int cod_op = recibir_operacion(cliente_fd_memoria);
	switch (cod_op)
	{
	case HANDSHAKE:
		handshake = recibir_paquete(cliente_fd_memoria);
		log_info(logger, "me llego:\n");
		list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
		break;
	case -1:
			log_error(logger, "el cliente MEMORIA se desconecto. Terminando servidor CPU");
			return EXIT_FAILURE;
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
	
	
	// t_paquete *crear_paquete(HANDSHAKE);
	// agregar_a_paquete(valor)

	terminar_programa(server_fd_memoria, logger, config); //logger: redundante (global) pero esta definido asi en utils.h
    return 0;
}


