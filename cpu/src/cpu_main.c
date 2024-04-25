#include <stdlib.h>
#include <stdio.h>
#include "cpu_main.h"

int main(void) {
    
	/*
	TO DO list:
	
	*/

	logger = log_create("cpu.log", "cpu", 1, LOG_LEVEL_DEBUG); //movido al mail desde abrirServerCPU
	
	char* ip_memoria,*ip_Kernel;
	char* puerto_CPU_MEMORIA,*puerto_KERNEL_CPU;
	char* valor,*valor_KERNEL;
	char* mensaje;
	int conexion_KERNEL_CPU;
	t_config* config_global;
	t_list *handshake;
	t_paquete* send_handshake_Kernel;
	

    //int socket_id = iniciar_servidor();
    config_global = config_create("../utils/config/config_global.config");

	//Cliente memoria
    ip_memoria = config_get_string_value(config_global, "IP_MEMORIA");
    puerto_CPU_MEMORIA = config_get_string_value(config_global, "PUERTO_CPU->MEMORIA");
	valor = config_get_string_value(config_global, "CLAVE_CPU"); // este esta de mas no? no lo usa en ningun lado

	int server_fd_memoria = iniciar_servidor(puerto_CPU_MEMORIA);
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
	
	//Config para conexion con Kernel. KERNEL->CPU
	ip_Kernel = config_get_string_value(config_global, "IP_KERNEL");
    puerto_KERNEL_CPU = config_get_string_value(config_global, "PUERTO_KERNEL->CPU");
	valor_KERNEL = config_get_string_value(config_global, "CLAVE_CPU");

	conexion_KERNEL_CPU = crear_conexion(ip_Kernel, puerto_KERNEL_CPU, logger);
	send_handshake_Kernel = crear_paquete(HANDSHAKE);

	agregar_a_paquete(send_handshake_Kernel, valor_KERNEL, strlen(valor_KERNEL)+1);
	enviar_paquete(send_handshake_Kernel, conexion_KERNEL_CPU);
	eliminar_paquete(send_handshake_Kernel);
	liberar_conexion(conexion_KERNEL_CPU);
	
	// t_paquete *crear_paquete(HANDSHAKE);
	// agregar_a_paquete(valor)

	terminar_programa(server_fd_memoria, logger, config_global); //logger: redundante (global) pero esta definido asi en utils.h
    return 0;
}


