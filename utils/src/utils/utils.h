#ifndef UTILS_H_
#define UTILS_H_

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

#define PUERTO "4444" //ojo -> Hardcodeado. 

/* Orden de conexiones:
Server -> Cliente:
* Kernel -> CPU
* Kernel -> Memoria
* CPU -> Memoria
* I/O -> Kernel
* I/O -> Memoria
*/

typedef enum
{
    HANDSHAKE,
    MENSAJE,
    PAQUETE

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

//socket
    void* recibir_buffer(int*, int);
    int iniciar_servidor(void);
    int esperar_cliente(int);
    t_list* recibir_paquete(int);
    void recibir_mensaje(int);
    void recibir_handshake(int);
    int recibir_operacion(int);

    int crear_conexion(char* ip, char* puerto, t_log * logger);
    void enviar_mensaje(char* mensaje, int socket_cliente);
    t_paquete* crear_paquete(protocolo_socket cod_op); 
    void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
    void enviar_paquete(t_paquete* paquete, int socket_cliente);
    void liberar_conexion(int socket_cliente);
    void eliminar_paquete(t_paquete* paquete);
//socket
    void leer_consola(void);
/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
* @return void
*/
void decir_hola(char* quien);

/*


*/

#endif
