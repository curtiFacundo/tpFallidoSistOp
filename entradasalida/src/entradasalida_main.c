#include <stdlib.h>
#include <stdio.h>
#include "entradasalida_main.h"

t_config* config_global;

int main(int argc, char* argv[]) {
        
	/*
	TO DO list:
	*/
	pthread_t tid_kernel;
	pthread_t tid_memoria;
	char *ret_value;
 
    logger = log_create("entradasalida.log", "entradasalida", 1, LOG_LEVEL_DEBUG);
    config_global = config_create("../utils/config/config_global.config");
    
	//conexiones
	pthread_create(&tid_kernel, NULL, thread_crear_conexion_server, IO_KERNEL);
	pthread_create(&tid_memoria, NULL, thread_crear_conexion_server, IO_MEMORIA);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_kernel,&ret_value);
	log_info(logger, ret_value);
	pthread_join(tid_memoria,&ret_value);
	log_info(logger, ret_value);
	//espero fin conexiones

    return 0;
}

void conexion_kernel(char* puerto) 
{
	t_list *handshake;

	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente CPU");
		//sem_post(server_io_kernel);
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
		//sem_post(server_io_memoria); //condicion de carrera entre esperar_cliente de io y crear_conexion de memoria?
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