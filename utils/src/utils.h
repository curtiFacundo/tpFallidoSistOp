#ifndef UTILS_H_
#define UTILS_H_

#include <linux/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <readline/readline.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>

/* Orden de conexiones:
Server -> Cliente:
* Kernel -> CPU
* Kernel -> Memoria
* CPU -> Memoria
* I/O -> Kernel
* I/O -> Memoria
*/

#define SEM_SERVER_KERNEL_MEMORIA "SEM_SERVER_KERNEL_MEMORIA"
#define SEM_SERVER_KERNEL_CPU "SEM_SERVER_KERNEL_CPU"
#define SEM_SERVER_CPU_MEMORIA "SEM_SERVER_CPU_MEMORIA"
#define SEM_SERVER_IO_MEMORIA "SEM_SERVER_IO_MEMORIA"
#define SEM_SERVER_IO_KERNEL "SEM_SERVER_IO_KERNEL"

typedef enum
{
    HANDSHAKE,
    MENSAJE,
    PAQUETE,
    TERMINATE

}protocolo_socket;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	protocolo_socket codigo_operacion;
	t_buffer* buffer;
} t_paquete;

extern t_log* logger;
extern t_config* config_global;

//socket
    void* recibir_buffer(int*, int);
    int iniciar_servidor(char *puerto);
    int esperar_cliente(int);
    t_list* recibir_paquete(int);
    void recibir_mensaje(int);
    void recibir_handshake(int);
    int recibir_operacion(int);

    int crear_conexion(char* ip, char* puerto);
    void enviar_mensaje(char* mensaje, int socket_cliente);
    t_paquete* crear_paquete(protocolo_socket cod_op); 
    void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
    void enviar_paquete(t_paquete* paquete, int socket_cliente);
    void liberar_conexion(int socket_cliente);
    void eliminar_paquete(t_paquete* paquete);
    void terminar_programa(int conexion, t_log* logger, t_config* config);
//socket
    void leer_consola(void);
    void iterator(char* value);
    
/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
* @return void
*/

//THREADS
typedef enum
{
    KERNEL_CPU,
    KERNEL_MEMORIA,
    CPU_MEMORIA,
    IO_KERNEL,
    IO_MEMORIA,

}tipo_conexion;
void *thread_crear_conexion_server(void *arg);
void *thread_crear_conexion_cliente(void *arg);
//THREADS

#endif