#include <stdlib.h>
#include <stdio.h>
#include "cpu_main.h"

int main(void) {
    
	/*
	TO DO list:
	
	*/

	logger = log_create("cpu.log", "cpu", 1, LOG_LEVEL_DEBUG); //movido al mail desde abrirServerCPU
	
	char* ip,*ipKernel;
	char* puerto,*puertoKernel;
	char* valor,*valorKernel;
	char* mensaje;
	int conexionKernel;
	t_config* config;
	t_list *handshake;
	t_paquete* send_handshake_Kernel;
	

    //int socket_id = iniciar_servidor();
    config = config_create("../utils/config/cpu.config");

    ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTOMEMORIA");
	valor = config_get_string_value(config, "CLAVE");

	int server_fd_memoria = iniciar_servidor(puerto);
	log_info(logger, "Servidor listo para recibir al cliente");
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
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
	default:
		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
		break;
	}
	
	//Config para conexion con Kernel
	ipKernel = config_get_string_value(config, "IPKERNEL");
    puertoKernel = config_get_string_value(config, "PUERTOKERNEL");
	valorKernel = config_get_string_value(config, "CLAVE");

	conexionKernel = crear_conexion(ipKernel, puertoKernel, logger);
	send_handshake_Kernel = crear_paquete(HANDSHAKE);

	agregar_a_paquete(send_handshake_Kernel, valorKernel, strlen(valor)+1);
	enviar_paquete(send_handshake_Kernel, conexionKernel);
	eliminar_paquete(send_handshake_Kernel);
	liberar_conexion(conexionKernel);
	
	// t_paquete *crear_paquete(HANDSHAKE);
	// agregar_a_paquete(valor)

	terminar_programa(server_fd_memoria, logger, config); //logger: redundante (global) pero esta definido asi en utils.h
    return 0;
}


