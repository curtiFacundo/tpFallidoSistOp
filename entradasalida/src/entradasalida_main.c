#include <stdlib.h>
#include <stdio.h>
#include "entradasalida_main.h"

t_config* config_global;


int main(int argc, char* argv[]) {
        
	/*
	TO DO list:
	*/
    
	char* ip, *ip_memoria;
	char* puerto_IO_KERNEL, *puerto_IO_MEMORIA;
	char* valor;
	char* mensaje;


    logger = log_create("entradasalida.log", "entradasalida", 1, LOG_LEVEL_DEBUG);
    //int socket_id = iniciar_servidor();
    config_global = config_create("../utils/config/config_global.config");
    // SERVER KERNEL
    ip = config_get_string_value(config_global, "IP_KERNEL");
    puerto_IO_KERNEL = config_get_string_value(config_global, "PUERTO_IO->KERNEL");
	valor = config_get_string_value(config_global, "CLAVE_IO");
    
    int server_fd_kernel = iniciar_servidor(puerto_IO_KERNEL);
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
			log_error(logger, handshake"el cliente Kernel se desconecto. Terminando servidor I/O");
			return EXIT_FAILURE;
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
	// FIN SERVER KERNEL

    // SERVER MEMORIA
    ip_memoria = config_get_string_value(config_global, "IP_MEMORIA");
    puerto_IO_MEMORIA = config_get_string_value(config_global, "PUERTO_IO->MEMORIA");
    
    int server_fd_memoria = iniciar_servidor(puerto_IO_MEMORIA);
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

	terminar_programa(server_fd_kernel, logger, config_global); //logger: redundante (global) pero esta definido asi en utils.h
	close(cliente_fd_kernel);

	close(server_fd_memoria);
	close(cliente_fd_memoria);
    return 0;
}

void conexion_kernel(char* puerto) 
{
	t_list *handshake;

	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente CPU");
		int cliente = esperar_cliente(server);
		while(true){
			int cod_op = recibir_operacion(cliente);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake = recibir_paquete(cliente);
					log_info(logger, "me llego:\n");
					list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
					break;
				case -1:
					log_error(logger, "el cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
					break;
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
		
	close(server);
	close(cliente);
}
void conexion_memoria(char* puerto) 
{
	t_list *handshake;

	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente CPU");
		int cliente = esperar_cliente(server);
		while(true){
			int cod_op = recibir_operacion(cliente);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake = recibir_paquete(cliente);
					log_info(logger, "me llego:\n");
					list_iterate(handshake, (void*) iterator); //no se como funciona esto 💁🏼
					break;
				case -1:
					log_error(logger, "el cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
					break;
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
		
	close(server);
	close(cliente);
}