#include <stdlib.h>
#include <stdio.h>
#include "kernel_main.h"

t_config* config;
t_list *handshake;
t_paquete* send_handshake_io;

int main(int argc, char* argv[]) 
{    
	/*
	TO DO list
	* Mover la secuencia de arranque del server de kernel a su propia funcion
	* Conexion con CPU: hacer que Kernel sea Server
	* iniciar_config(): Mover a utils.c
	* serServidorIO(): generalizar? hasta que tengamos hilos, atajar las operaciones una por una
	*/
	
	int conexionEntradaSalida;
	char* ip, *ipEntradaSalida;
	char* puerto, *puertoEntradaSalida;
	char* valor, *valorEntradaSalida;
	char* mensaje;


    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    //int socket_id = iniciar_servidor();
    config = config_create("../utils/config/kernel.config");

	//Config para cliente Memoria
	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTOMEMORIA");
	valor = config_get_string_value(config, "CLAVE");
	EscucharServidor(puerto, "memoria");
	
	//Config para cliente CPU
	ip = config_get_string_value(config, "IPCPU");
    puerto = config_get_string_value(config, "PUERTOCPU");
	EscucharServidor(puerto, "CPU");

	//terminar_programa(server_fd_memoria, logger, config); //logger: redundante (global) pero esta definido asi en utils.h
    
	//Config para conexion con EntradaSalida
	ipEntradaSalida = config_get_string_value(config, "IPIO");
    puertoEntradaSalida = config_get_string_value(config, "PUERTOIO");
	valorEntradaSalida = config_get_string_value(config, "CLAVE");

	conexionEntradaSalida = crear_conexion(ipEntradaSalida, puertoEntradaSalida, logger);
	send_handshake_io = crear_paquete(HANDSHAKE);

	agregar_a_paquete (send_handshake_io, valorEntradaSalida, strlen(valor)+1);
	enviar_paquete(send_handshake_io, conexionEntradaSalida);
	eliminar_paquete(send_handshake_io);
	liberar_conexion(conexionEntradaSalida);
	
	return 0;
}

void EscucharServidor(char* puerto, char* CLIENTE) 
{
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente");
		log_info(logger, CLIENTE);
		int cliente = esperar_cliente(server);
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
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	close(server);
}