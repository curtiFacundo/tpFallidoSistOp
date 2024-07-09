#include <stdlib.h>
#include <stdio.h>
#include "entradasalida_main.h"

int main(int argc, char* argv[]) {
           
	/*
	TO DO list:
	*/
	pthread_t tid_kernel;
	pthread_t tid_memoria;
	void *ret_value;
	
	argumentos_thread arg_kernel;
	argumentos_thread arg_memoria;
	
    logger = log_create("entradasalida.log", "entradasalida", 1, LOG_LEVEL_DEBUG);
    config_global = config_create("../utils/config/config_global.config");

	arg_memoria.puerto = config_get_string_value(config_global, "PUERTO_IO->MEMORIA");
	arg_kernel.puerto = config_get_string_value(config_global, "PUERTO_IO->KERNEL");
    
	//conexiones
	pthread_create(&tid_kernel, NULL, conexion_kernel, (void *)&arg_kernel);
	pthread_create(&tid_memoria, NULL, conexion_memoria, (void *)&arg_memoria);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_kernel,ret_value);
	pthread_join(tid_memoria,ret_value);
	//espero fin conexiones

    return 0;
}

void *conexion_kernel(void* arg_kernel) 
{
	argumentos_thread * args = arg_kernel;
	t_paquete *handshake_send;
	t_list *handshake_recv;
	char * handshake_texto = "handshake";
	
	int server = iniciar_servidor(args->puerto);
		log_info(logger, "Servidor listo para recibir al cliente KERNEL");
		int cliente = esperar_cliente(server);

	//HANDSHAKE
	handshake_send = crear_paquete(INSTRUCCIONES);
	agregar_a_paquete (handshake_send, handshake_texto , strlen(handshake_texto)+1);
	//HANDSHAKE_end


		while(true){
			int cod_op = recibir_operacion(cliente);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake_recv = recibir_paquete(cliente);
					log_info(logger, "me llego:\n");
					list_iterate(handshake_recv, (void*) iterator);
					enviar_paquete(handshake_send, cliente);
					break;
				case INSTRUCCIONES:
					handshake_recv = recibir_paquete(cliente);
					log_info(logger, "me llego:\n");
					list_iterate(handshake_recv, (void*) iterator);
					enviar_paquete(handshake_send, cliente);
					break;
				case -1:
					log_error(logger, "el cliente se desconecto. Terminando servidor");
					return (void *)EXIT_FAILURE;
					break;
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
		
	close(server);
	close(cliente);
}
void *conexion_memoria(void* arg_memoria) 
{
	argumentos_thread * args = arg_memoria;
	t_paquete *handshake_send;
	t_list *handshake_recv;
	char * handshake_texto = "handshake";
	
	int server = iniciar_servidor(args->puerto);
		log_info(logger, "Servidor listo para recibir al cliente MEMORIA");
		int cliente = esperar_cliente(server);

	//HANDSHAKE
	handshake_send = crear_paquete(HANDSHAKE);
	agregar_a_paquete (handshake_send, handshake_texto , strlen(handshake_texto)+1);
	//HANDSHAKE_end


		while(true){
			int cod_op = recibir_operacion(cliente);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake_recv = recibir_paquete(cliente);
					log_info(logger, "me llego:\n");
					list_iterate(handshake_recv, (void*) iterator);
					enviar_paquete(handshake_send, cliente);
					break;
				case -1:
					log_error(logger, "el cliente se desconecto. Terminando servidor");
					return (void*)EXIT_FAILURE;
					break;
				default:
					log_warning(logger,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}
		
	close(server);
	close(cliente);
}