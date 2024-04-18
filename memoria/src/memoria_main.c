#include <stdlib.h>
#include <stdio.h>
#include "memoria_main.h"

int Saludar(void) {
    logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Hola! CPU");
    return 0;
}

int main(void) {
	int conexion;
	char* ip;
	char* puerto;
	char* valor;
	t_config* config;
	t_paquete* save_handshake;
	    
	/*
	TO DO list:
	
	*/

	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	config = iniciar_config();
	//conexión

	ip = config_get_string_value(config, "IPKERNEL");
    puerto = config_get_string_value(config, "PUERTOKERNEL");
	valor = config_get_string_value(config, "CLAVE");

	conexion = crear_conexion(ip, puerto, logger);
	save_handshake = crear_paquete(HANDSHAKE);

	agregar_a_paquete (save_handshake, valor, strlen(valor)+1);
	enviar_paquete(save_handshake, conexion);
	eliminar_paquete(save_handshake);

	//


}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	nuevo_config = config_create("../utils/config/memoria.config");

	return nuevo_config;
}



// t_paquete *crear_paquete(HANDSHAKE);
// agregar_a_paquete(valor);

void iterator(char* value) {
	log_info(logger,"%s", value);
}