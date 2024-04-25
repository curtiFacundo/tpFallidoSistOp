#include <stdlib.h>
#include <stdio.h>
#include "memoria_main.h"

int Saludar(void) {
    logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    return 0;
}

int main(void) {
	int conexion_Kernel_memoria, conexion_CPU_memoria, conexion_IO_memoria ;
	char* ipKernel, *ipCPU, *ipIO;
	char* puerto_KERNEL_MEMORIA, *puerto_CPU_MEMORIA, *puerto_IO_MEMORIA;

	char* valor;
	t_config* config_global;
	t_paquete* paquete_handshake_kernel, *paquete_handshake_cpu, *paquete_handshake_io;
	    
	/*
	TO DO list:
	
	*/

	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	config_global = config_create("../utils/config/config_global.config");
	valor = config_get_string_value(config_global, "CLAVE_MEMORIA");
	//conexión

	//-- COMIENZA CLIENTE PARA KERNEL
	paquete_handshake_kernel = crear_paquete(HANDSHAKE);
	agregar_a_paquete (paquete_handshake_kernel, valor, strlen(valor)+1);	
	ipKernel = config_get_string_value(config_global, "IP_KERNEL");
    puerto_KERNEL_MEMORIA = config_get_string_value(config_global, "PUERTO_KERNEL->MEMORIA");

	conexion_Kernel_memoria = crear_conexion(ipKernel, puerto_KERNEL_MEMORIA, logger);

	enviar_paquete(paquete_handshake_kernel, conexion_Kernel_memoria);
	eliminar_paquete(paquete_handshake_kernel);
	liberar_conexion(conexion_Kernel_memoria);
	//-- FINALIZA CLIENTE PARA KERNEL

	//-- COMIENZA CLIENTE PARA CPU
	paquete_handshake_cpu = crear_paquete(HANDSHAKE);
	agregar_a_paquete (paquete_handshake_cpu, valor, strlen(valor)+1);
	ipCPU = config_get_string_value(config_global, "IP_CPU");
    puerto_CPU_MEMORIA = config_get_string_value(config_global, "PUERTO_CPU->MEMORIA");

	conexion_CPU_memoria = crear_conexion(ipCPU, puerto_CPU_MEMORIA, logger);

	enviar_paquete(paquete_handshake_cpu, conexion_CPU_memoria);
	eliminar_paquete(paquete_handshake_cpu);
	liberar_conexion(conexion_CPU_memoria);
	//-- FINALIZA CLIENTE PARA CPU

	//-- COMIENZA CLIENTE PARA IO
	paquete_handshake_io = crear_paquete(HANDSHAKE);
	agregar_a_paquete (paquete_handshake_io, valor, strlen(valor)+1);
	ipIO = config_get_string_value(config_global, "IP_IO");
    puerto_IO_MEMORIA = config_get_string_value(config_global, "PUERTO_IO->MEMORIA");
	conexion_IO_memoria = crear_conexion(ipIO, puerto_IO_MEMORIA, logger);

	enviar_paquete(paquete_handshake_io, conexion_IO_memoria);
	eliminar_paquete(paquete_handshake_io);
	liberar_conexion(conexion_IO_memoria);
	//-- FINALIZA CLIENTE PARA IO
}
