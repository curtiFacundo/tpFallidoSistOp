#include <utils.h>

//defines de operaciones
#define OPERACION1 0

t_log* logger;


void decir_hola(char* quien) {
    printf("Hola desde %s!!\n", quien);
}

//socket
    int iniciar_servidor(char *puerto)
    {
        int socket_servidor;

        struct addrinfo hints, *servinfo;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(NULL, puerto, &hints, &servinfo);

        socket_servidor = socket(
            servinfo->ai_family,
            servinfo->ai_socktype,
            servinfo->ai_protocol);

        bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
        listen(socket_servidor, SOMAXCONN);

        freeaddrinfo(servinfo);
        log_trace(logger, "Listo para escuchar a mi cliente");

        return socket_servidor;
    }

    int esperar_cliente(int socket_servidor)
    {
        int socket_cliente = accept(socket_servidor, NULL, NULL);
        log_info(logger, "Se conecto un cliente!");

        return socket_cliente;
    }

    int recibir_operacion(int socket_cliente)
    {
        int cod_op;
        if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) //function block hasta que llegue un paquete
            return cod_op;
        else
        {
            close(socket_cliente); //cierra la conexion si la conexion no existe
            return -1;
        }
    }

    void* recibir_buffer(int* size, int socket_cliente)
    {
        void * buffer;

        recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
        buffer = malloc(*size);
        recv(socket_cliente, buffer, *size, MSG_WAITALL);

        return buffer;
    }

    void recibir_mensaje(int socket_cliente)
    {
        int size;
        char* buffer = recibir_buffer(&size, socket_cliente);
        log_info(logger, "Me llego el mensaje %s", buffer);
        free(buffer);
    }

    t_list* recibir_paquete(int socket_cliente)
    {
        int size;
        int desplazamiento = 0;
        void * buffer;
        t_list* valores = list_create();
        int tamanio;

        buffer = recibir_buffer(&size, socket_cliente);
        while(desplazamiento < size)
        {
            memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
            desplazamiento+=sizeof(int);
            char* valor = malloc(tamanio);
            memcpy(valor, buffer+desplazamiento, tamanio);
            desplazamiento+=tamanio;
            list_add(valores, valor);
        }
        free(buffer);
        return valores;
    }

    void* serializar_paquete(t_paquete* paquete, int bytes)
    {
        void * magic = malloc(bytes);
        int desplazamiento = 0;

        memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
        desplazamiento+= sizeof(int);
        memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
        desplazamiento+= sizeof(int);
        memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
        desplazamiento+= paquete->buffer->size;

        return magic;
    }

    int crear_conexion(char *ip, char* puerto)
    {
        struct addrinfo hints;
        struct addrinfo *server_info;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        getaddrinfo(ip, puerto, &hints, &server_info);

        int socket_cliente = socket(
                            server_info->ai_family,
                            server_info->ai_socktype,
                            server_info->ai_protocol);

        if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)==-1){
            log_error(logger, "no se pudo establecer conexion al servidor");
        }else log_info(logger, "Conectado al servidor");

        freeaddrinfo(server_info);

        return socket_cliente;
    }

    void enviar_mensaje(char* mensaje, int socket_cliente)
    {
        t_paquete* paquete = malloc(sizeof(t_paquete));

        paquete->codigo_operacion = MENSAJE;config_get_string_value(config_global, "IP_CPU");
        paquete->buffer = malloc(sizeof(t_buffer));
        paquete->buffer->size = strlen(mensaje) + 1;
        paquete->buffer->stream = malloc(paquete->buffer->size);
        memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

        int bytes = paquete->buffer->size + 2*sizeof(int);

        void* a_enviar = serializar_paquete(paquete, bytes);

        send(socket_cliente, a_enviar, bytes, 0);

        free(a_enviar);
        eliminar_paquete(paquete);
    }

    void crear_buffer(t_paquete* paquete)
    {
        paquete->buffer = malloc(sizeof(t_buffer));
        paquete->buffer->size = 0;
        paquete->buffer->stream = NULL;
    }

    t_paquete* crear_paquete(protocolo_socket cod_op)
    {
        t_paquete* paquete = malloc(sizeof(t_paquete));
        paquete->codigo_operacion = cod_op;
        crear_buffer(paquete);
        return paquete;
    }

    void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
    {
        paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

        memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
        memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

        paquete->buffer->size += tamanio + sizeof(int);
    }

    void enviar_paquete(t_paquete* paquete, int socket_cliente)
    {
        int bytes = paquete->buffer->size + 2*sizeof(int);
        void* a_enviar = serializar_paquete(paquete, bytes);

        send(socket_cliente, a_enviar, bytes, 0);

        free(a_enviar);
    }

    void eliminar_paquete(t_paquete* paquete)
    {
        free(paquete->buffer->stream);
        free(paquete->buffer);
        free(paquete);
    }

    void liberar_conexion(int socket_cliente)
    {
        close(socket_cliente);
    }
//socket

void leer_consola()
{
	char* leido;
	while(!string_is_empty(leido = readline("> "))){
		log_info(logger, leido);
	}
	free(leido);
}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete = crear_paquete(PAQUETE);
	while(!string_is_empty(leido = readline("> "))){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
	}
	enviar_paquete(paquete, conexion);
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	close(conexion);
	config_destroy(config);

}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

//THREADS
void *thread_crear_conexion_server(void *arg) {
  //return value
  char ret[1000];
  strcpy(ret, "Error de thread -> argumento invalido");
  switch((tipo_conexion) arg){
    case KERNEL_CPU:
        conexion_cpu(config_get_string_value(config_global, "PUERTO_KERNEL->CPU"));
        strcpy(ret, "Se finalizo el thread conexion Kernel_CPU");
        pthread_exit(ret);
        break;
    
    case KERNEL_MEMORIA:
        conexion_memoria(config_get_string_value(config_global, "PUERTO_KERNEL->MEMORIA"));
        strcpy(ret, "Se finalizo el thread conexion Kernel_MEMORIA");
        pthread_exit(ret);
        break;

    case CPU_MEMORIA:
        #ifdef CPU_MAIN_H_
        conexion_memoria(config_get_string_value(config_global, "PUERTO_CPU->MEMORIA"));
        strcpy(ret, "Se finalizo el thread conexion CPU_MEMORIA");
        #endif
        pthread_exit(ret);
        break;

    case IO_KERNEL:
        #ifdef IO_MAIN_H_
        conexion_kernel(config_get_string_value(config_global, "PUERTO_IO->KERNEL"));
        strcpy(ret, "Se finalizo el thread conexion IO->KERNEL");
        #endif
        pthread_exit(ret);
        break;

    case IO_MEMORIA:
        #ifdef IO_MAIN_H_
        conexion_memoria(config_get_string_value(config_global, "PUERTO_IO->MEMORIA"));
        strcpy(ret, "Se finalizo el thread conexion IO->MEMORIA");
        #endif
        pthread_exit(ret);
        break;

  }
  strcpy(ret, "Mensaje finalizacion placeholder");
  pthread_exit(ret);
}
void *thread_crear_conexion_cliente(void *arg) {
  //return value
  char ret[100];
  strcpy(ret, "Error de thread -> argumento invalido");
  switch((tipo_conexion) arg){
    case KERNEL_CPU:
        #ifdef CPU_MAIN_H_
        cliente_conexion_KERNEL(
                config_get_string_value(config_global, "PUERTO_KERNEL->CPU"),
                config_get_string_value(config_global, "IP_KERNEL"));

        strcpy(ret, "Se finalizo el thread conexion Kernel_CPU");
        #endif
        pthread_exit(ret);
        break;
    
    case KERNEL_MEMORIA:
        #ifdef MEMORIA_MAIN_H_
        cliente_conexion_KERNEL(    
            config_get_string_value(config_global, "PUERTO_KERNEL->MEMORIA"),
            config_get_string_value(config_global, "IP_KERNEL"));

        strcpy(ret, "Se finalizo el thread conexion KERNEL_MEMORIA");
        #endif
        pthread_exit(ret);
        break;

    case CPU_MEMORIA:
        #ifdef MEMORIA_MAIN_H_
        cliente_conexion_CPU(
            config_get_string_value(config_global, "PUERTO_CPU->MEMORIA"),
            config_get_string_value(config_global, "IP_CPU"));

        strcpy(ret, "Se finalizo el thread conexion CPU_MEMORIA");
        #endif
        pthread_exit(ret);
        break;

    case IO_KERNEL:
        
        cliente_conexion_IO(
        config_get_string_value(config_global, "PUERTO_IO->KERNEL"),
        config_get_string_value(config_global, "IP_IO"));

        strcpy(ret, "Se finalizo el thread conexion IO_KERNEL");

        pthread_exit(ret);
        break;

    case IO_MEMORIA:
        #ifdef MEMORIA_MAIN_H_
        cliente_conexion_IO(
            config_get_string_value(config_global, "PUERTO_IO->MEMORIA"),
            config_get_string_value(config_global, "IP_IO"));

        strcpy(ret, "Se finalizo el thread conexion IO_MEMORIA");
        #endif
        pthread_exit(ret);
        break;

  }
  pthread_exit(ret);
}
//THREADS