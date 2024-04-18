#include <stdlib.h>
#include <stdio.h>
#include "kernel_main.h"

int main(int argc, char* argv[]) {
    
	/*
	TO DO list
	* Mover la secuencia de arranque del server de kernel a su propia funcion
	* Conexion con CPU: hacer que Kernel sea Server
	* iniciar_config(): Mover a utils.c
	* serServidorIO(): generalizar? hasta que tengamos hilos, atajar las operaciones una por una
	*/
	int conexionEntradaSalida;
	char* ip, ipEntradaSalida;
	char* puerto, puertoEntradaSalida;
	char* valor, valorEntradaSalida;
	char* mensaje;
	t_config* config;
	t_list *handshake;
	t_paquete* save_handshake;

    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    //int socket_id = iniciar_servidor();
    config = config_create("../utils/config/kernel.config");

	//Config para cliente Memoria
    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTOMEMORIA");
	valor = config_get_string_value(config, "CLAVE");
	

	int server_fd_memoria = iniciar_servidor(puerto);
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd_memoria = esperar_cliente(server_fd_memoria);
	int cod_op = recibir_operacion(server_fd_memoria);
	switch (cod_op)
	{
	case HANDSHAKE:
		handshake = recibir_paquete(server_fd_memoria);
		log_info(logger, "me llego:\n");
		list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
		break;
	case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
	
	
	// t_paquete *crear_paquete(HANDSHAKE);
	// agregar_a_paquete(valor)

	terminar_programa(server_fd_memoria, logger, config); //logger: redundante (global) pero esta definido asi en utils.h
    
	//Config para conexion con EntradaSalida
	ipEntradaSalida = config_get_string_value(config, "IPIO");
    puertoEntradaSalida = config_get_string_value(config, "PUERTOIO");
	valorEntradaSalida = config_get_string_value(config, "CLAVE");

	conexionEntradaSalida = crear_conexion(ip, puerto, logger);
	save_handshake = crear_paquete(HANDSHAKE);

	agregar_a_paquete (save_handshake, valor, strlen(valor)+1);
	enviar_paquete(save_handshake, conexionEntradaSalida);
	eliminar_paquete(save_handshake);
	
	
	
	
	return 0;
}
