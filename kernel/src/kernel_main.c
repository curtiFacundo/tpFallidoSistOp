#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "kernel_main.h"

int socket_cliente_cpu;
int cliente_memoria; // cliente memoria, tendria que cambiar el nombre a un monton de variables

int main(int argc, char* argv[]) 
{    

	pthread_t tid_memoria;
	pthread_t tid_cpu;
	pthread_t tid_io;
	pthread_t tid_scheduler;

	argumentos_thread_scheduler arg_sch;

	void *ret_value;
	argumentos_thread arg_io;
	argumentos_thread arg_memoria;
	argumentos_thread arg_cpu;


	logger = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);
    config_global = config_create("../utils/config/config_global.config");
   	
	

	//planificador
	arg_sch.algoritmo = FIFO;
	pthread_create(&tid_scheduler,NULL,corto_plazo,(void*)&arg_sch);
	pthread_detach(tid_scheduler);


	//conexiones
	arg_memoria.puerto = config_get_string_value(config_global, "PUERTO_KERNEL->MEMORIA");
	arg_cpu.puerto = config_get_string_value(config_global, "PUERTO_KERNEL->CPU");
	arg_io.puerto = config_get_string_value(config_global, "PUERTO_IO->KERNEL");
	arg_io.ip = config_get_string_value(config_global, "IP_IO");

	//conexiones
	pthread_create(&tid_memoria, NULL, conexion_memoria, (void *)&arg_memoria);
	pthread_create(&tid_cpu, NULL, conexion_cpu, (void *)&arg_cpu);
	pthread_create(&tid_io, NULL, cliente_conexion_IO, (void *)&arg_io);
	//conexiones

	//espero fin conexiones
	pthread_join(tid_memoria, ret_value);
	pthread_join(tid_cpu, ret_value);
	pthread_join(tid_io, ret_value);
	//espero fin conexiones
	
	return 0;
}

void *conexion_cpu(void* arg_cpu)
{
	argumentos_thread * args = arg_cpu; 
	t_paquete *handshake_send;
	t_list *handshake_recv;
	char * handshake_texto = "handshake";
	
	int server = iniciar_servidor(args->puerto);
	log_info(logger, "Servidor listo para recibir al cliente CPU");
	
	socket_cliente_cpu = esperar_cliente(server);
	//HANDSHAKE
	handshake_send = crear_paquete(HANDSHAKE);
	agregar_a_paquete (handshake_send, handshake_texto , strlen(handshake_texto)+1);
	//HANDSHAKE_end


		while(true){
			int cod_op = recibir_operacion(socket_cliente_cpu);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake_recv = recibir_paquete(socket_cliente_cpu);
					log_info(logger, "me llego:\n");
					list_iterate(handshake_recv, (void*) iterator);
					enviar_paquete(handshake_send, socket_cliente_cpu);
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
	close(socket_cliente_cpu);
}
void *conexion_memoria(void* arg_memoria) 
{
	argumentos_thread * args = arg_memoria;
	t_paquete *handshake_send;
	t_list *handshake_recv;
	char * handshake_texto = "handshake";
	
	int server = iniciar_servidor(args->puerto);
	log_info(logger, "Servidor listo para recibir al cliente MEMORIA");
	cliente_memoria = esperar_cliente(server);  // declaraba cliente aca "int cliente" lo saque para que sea general arriba de todo

	//HANDSHAKE
	handshake_send = crear_paquete(HANDSHAKE);
	agregar_a_paquete (handshake_send, handshake_texto , strlen(handshake_texto)+1);
	//HANDSHAKE_end


		while(true){
			int cod_op = recibir_operacion(cliente_memoria);
			switch (cod_op)
			{
				case HANDSHAKE:
					handshake_recv = recibir_paquete(cliente_memoria);
					log_info(logger, "me llego:\n");
					list_iterate(handshake_recv, (void*) iterator);
					enviar_paquete(handshake_send, cliente_memoria);
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
	close(cliente_memoria);
}

void *cliente_conexion_IO(void * arg_io){

	argumentos_thread * args = arg_io;
	t_paquete* send_handshake_io;
	int conexion_IO_KERNEL;
	protocolo_socket op;
	int flag=1;
	char *valor_IO;
	valor_IO = config_get_string_value(config_global, "CLAVE_KERNEL");

	do
	{
		conexion_IO_KERNEL = crear_conexion(args->ip, args->puerto);
		sleep(1);

	}while(conexion_IO_KERNEL == -1);
	
	
	send_handshake_io = crear_paquete(INSTRUCCIONES);
	agregar_a_paquete (send_handshake_io,valor_IO, strlen(valor_IO)+1); 

	while(flag){
		enviar_paquete(send_handshake_io, conexion_IO_KERNEL);
		sleep(1);
		op = recibir_operacion(conexion_IO_KERNEL);
		switch (op)
		{
		case HANDSHAKE:
			log_info(logger, "recibi handshake de IO");
			break;
		case INSTRUCCIONES:
			log_info(logger, "Recibi el archivo de instruccciones de IO");
			break;
		case TERMINATE:
			flag = 0;
			break;

		default:
			break;
		}
	}

	eliminar_paquete(send_handshake_io);
	liberar_conexion(conexion_IO_KERNEL);
}
