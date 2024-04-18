#include <stdlib.h>
#include <stdio.h>
#include "memoria_main.h"

int Saludar(void) {
    logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    return 0;
}

int main(void) {
	int conexionKernel;
	int conexionCpu;
	char* ipKernel;
	char* ipCPU;
	char* puertoKernel;
	char* puertoCPU;
	char* valor;
	t_config* config;
	t_paquete* paquete_handshake_kernel;
	t_paquete* paquete_handshake_cpu;
	    
	/*
	TO DO list:
	
	*/

	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	config = config_create("../utils/config/memoria.config");
	valor = config_get_string_value(config, "CLAVE");
	//conexión

	//-- COMIENZA CLIENTE PARA KERNEL
	paquete_handshake_kernel = crear_paquete(HANDSHAKE);
	agregar_a_paquete (paquete_handshake_kernel, valor, strlen(valor)+1);	
	ipKernel = config_get_string_value(config, "IPKERNEL");
    puertoKernel = config_get_string_value(config, "PUERTOKERNEL");

	conexionKernel = crear_conexion(ipKernel, puertoKernel, logger);

	enviar_paquete(paquete_handshake_kernel, conexionKernel);
	eliminar_paquete(paquete_handshake_kernel);
	liberar_conexion(conexionKernel);
	//-- FINALIZA CLIENTE PARA KERNEL

	//-- COMIENZA CLIENTE PARA CPU
	paquete_handshake_cpu = crear_paquete(HANDSHAKE);
	agregar_a_paquete (paquete_handshake_cpu, valor, strlen(valor)+1);
	ipCPU = config_get_string_value(config, "IPCPU");
    puertoCPU = config_get_string_value(config, "PUERTOCPU");

	conexionCpu = crear_conexion(ipCPU, puertoCPU, logger);

	enviar_paquete(paquete_handshake_kernel, conexionCpu);
	eliminar_paquete(paquete_handshake_kernel);
	//-- FINALIZA CLIENTE PARA CPU

}

// t_paquete *crear_paquete(HANDSHAKE);
// agregar_a_paquete(valor);
