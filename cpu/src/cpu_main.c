#include <stdlib.h>
#include <stdio.h>
#include "cpu_main.h"

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

    // Crear un PCB de ejemplo para probar fetch y decode
    t_list* instrucciones = list_create();
    RegistroCPU registros = {0};
    pcb* proceso = crear_pcb(1, 10, registros, instrucciones);

	// Loop principal de ejecución del CPU
	while (proceso->estado != EXIT) {
		t_operaciones operacion;
		proceso->estado != NEW;
		
		Fetch(proceso, &operacion);//Obtiene la próxima instrucción utilizando el PC del PCB
		decode(&operacion);//Interpreta la instrucción obtenida
		Execute(proceso, &operacion, &(proceso->registros));//Ejecuta la instrucción decodificada
		// Actualiza el PC solo si no es una instrucción de salto
		if (operacion.tipo != JNZ || proceso->registros.registros[operacion.parametros[0]] == 0) {
			proceso->pc += 1;
		}
	}

    // Liberación de recursos y finalización
    eliminar_PCB(proceso);
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
	t_paquete *handshake_send;
	t_paquete *handshake_recv;
	char * handshake_texto = "handshake";
	
	int server = iniciar_servidor(puerto);
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



void Fetch(pcb* pcb, t_operaciones* operacion) {
    if (pcb->pc < list_size(pcb->instrucciones)) {
        *operacion = list_get(pcb->instrucciones, pcb->pc);
		pcb->estado = READY;
    } else {
        log_error(logger, "PC fuera de los límites de las instrucciones");
        pcb->estado = EXIT;
    }
}

void decode(t_operaciones* operacion) {
    switch (operacion->tipo) {
        case SET:
            log_info(logger, "SET");
            log_info(logger, "SET del valor del registro %d hacia el registro %d", operacion->parametros[0], operacion->parametros[1]);
            break;
        case SUM:
            log_info(logger, "SUM");
            log_info(logger, "Suma del valor del registro %d y el registro %d", operacion->parametros[1], operacion->parametros[0]);
            break;
        case SUB:
            log_info(logger, "SUB");
            log_info(logger, "Resta el valor del registro %d y el registro %d", operacion->parametros[1], operacion->parametros[0]);
            break;
        case JNZ:
            log_info(logger, "JNZ");
            log_info(logger, "Salto desde el registro %d si el registro %d no es 0", operacion->parametros[1], operacion->parametros[0]);
            break;
        default:
            log_error(logger, "Instrucción desconocida");
            break;
    }
}


void Execute(pcb* pcb, t_operaciones *operacion, RegistroCPU *registros) {
    // Dependiendo del tipo de instrucción, realizamos las operaciones correspondientes
	pcb->estado = EXEC;
    switch (operacion->tipo) {
        case SET:
            registros[operacion->parametros[0]] = operacion->parametros[1];
            break;
        case SUM:
            registros[operacion->parametros[0]] += registros[operacion->parametros[1]];
            break;
        case SUB:
            registros[operacion->parametros[0]] -= registros[operacion->parametros[1]];
            break;
        case JNZ:
            if (registros[operacion->parametros[0]] != 0)
                registros->PC = operacion->parametros[1];
            break;
        default:
            log_error(logger, "Instrucción desconocida");
            break;
		proceso->pc += 1;
    }
}