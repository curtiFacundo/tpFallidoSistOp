#include <utils/hello.h>

void decir_hola(char* quien) {
    printf("Hola desde %s!!\n", quien);
}

t_log *iniciar_log(t_log *t_log, char *nombre_log, char *nombre_modulo){

    
    t_log = log_create(nombre_log,nombre_modulo,true,LOG_LEVEL_INFO);

    return t_log;
}
