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

    int conexion;
	char* ip;
	char* puerto;
	char* valor;
	char* mensaje;
	t_config* config;
	t_list *handshake;

    logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    //int socket_id = iniciar_servidor();
    config = iniciar_config();

    ip = config_get_string_value(config, "IPIO");
    puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	int server_fd_cpu = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd_cpu = esperar_cliente(server_fd_cpu);
	int cod_op = recibir_operacion(server_fd_cpu);
	switch (cod_op)
	{
	case HANDSHAKE:
		handshake = recibir_paquete(server_fd_cpu);
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

	terminar_programa(conexion, logger, config); //logger: redundante (global) pero esta definido asi en utils.h
    return 0;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("../utils/config/kernel.config");

	return nuevo_config;
}

int crearConexionCPU(int conexion, char* ip, char* puerto,char* valor) {
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