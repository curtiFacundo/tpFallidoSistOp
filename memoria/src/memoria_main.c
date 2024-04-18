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
	t_paquete* save_handshake;
	    
	/*
	TO DO list:
	
	*/

	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	config = config_create("../utils/config/memoria.config");
	//conexión

	valor = config_get_string_value(config, "CLAVE");
	save_handshake = crear_paquete(HANDSHAKE);
	agregar_a_paquete (save_handshake, valor, strlen(valor)+1);

	//-- COMIENZA CLIENTE PARA KERNEL
	ipKernel = config_get_string_value(config, "IPKERNEL");
    puertoKernel = config_get_string_value(config, "PUERTOKERNEL");

	conexionKernel = crear_conexion(ipKernel, puertoKernel, logger);

	enviar_paquete(save_handshake, conexionKernel);
	eliminar_paquete(save_handshake);
	liberar_conexion(conexionKernel);
	//-- FINALIZA CLIENTE PARA KERNEL

	//-- COMIENZA CLIENTE PARA CPU
	ipCPU = config_get_string_value(config, "IPCPU");
    puertoCPU = config_get_string_value(config, "PUERTOCPU");

	conexionCpu = crear_conexion(ipCPU, puertoCPU, logger);

	enviar_paquete(save_handshake, conexionCpu);
	eliminar_paquete(save_handshake);
	//-- FINALIZA CLIENTE PARA CPU

}

// t_paquete *crear_paquete(HANDSHAKE);
// agregar_a_paquete(valor);
