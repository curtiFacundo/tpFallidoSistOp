#include <stdlib.h>
#include <stdio.h>
#include "cpu_main.h"

t_config* config_global;

int main(void) {
    
	/*
	TO DO list:
	
	*/

	logger = log_create("cpu.log", "cpu", 1, LOG_LEVEL_DEBUG); //movido al mail desde abrirServerCPU
	
	pthread_t tid_memoria;
	pthread_t tid_kernel;
	char *ret_value;
	char *puerto_memoria;
	char *arg_kernel[2]; // [PUERTO | IP]
	

    //int socket_id = iniciar_servidor();
    config_global = config_create("../utils/config/config_global.config");

	puerto_memoria = config_get_string_value(config_global, "PUERTO_CPU->MEMORIA");
	arg_kernel[0] = config_get_string_value(config_global, "PUERTO_KERNEL->CPU");
	arg_kernel[1] = config_get_string_value(config_global, "IP_KERNEL");


	//conexiones
	pthread_create(&tid_memoria, NULL, conexion_memoria, puerto_memoria);
	pthread_create(&tid_kernel, NULL, cliente_conexion_KERNEL, arg_kernel);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_memoria, ret_value);
	pthread_join(tid_kernel, ret_value);
	//espero fin conexiones

	// terminar_programa(server_fd_memoria, logger, config_global); //logger: redundante (global) pero esta definido asi en utils.h
    return 0;
}
void *cliente_conexion_KERNEL(char * arg_kernel[]){
	t_paquete* send_handshake;
	int server;
	protocolo_socket op;
	int flag=1;
	char* valor_CPU;
	
	log_info(logger, "Conectando a kernel");
	log_info(logger, "IP:");
	log_info(logger, arg_kernel[1]);
	log_info(logger, "Puerto:");
	log_info(logger, arg_kernel[0]);

	valor_CPU = config_get_string_value(config_global, "CLAVE_CPU");
	server = crear_conexion(arg_kernel[1], arg_kernel[0]);
	send_handshake = crear_paquete(HANDSHAKE);
	agregar_a_paquete (send_handshake, valor_CPU , strlen(valor_CPU)+1); 

	while(flag){
		sleep(1);
		enviar_paquete(send_handshake, server);
		op = recibir_operacion(server);
		switch (op)
		{
		case HANDSHAKE:
			log_info(logger, "recibi handshake de KERNEL");
			break;
		
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake);
	liberar_conexion(server);
}
void *conexion_memoria(char* puerto) 
{
	t_list *handshake;
	int server = iniciar_servidor(puerto);
		log_info(logger, "Servidor listo para recibir al cliente Memoria");
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