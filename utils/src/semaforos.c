#include <utils.h>

sem_t server_io_kernel;
sem_init(&server_io_kernel,0,0);
sem_t server_io_memoria;
sem_init(&server_io_memoria,0,0);
sem_t server_kernel_cpu;
sem_init(&server_kernel_cpu,0,0);
sem_t server_kernel_memoria;
sem_init(&server_kernel_memoria,0,0);
sem_t server_cpu_memoria;
sem_init(&server_cpu_memoria,0,0);
