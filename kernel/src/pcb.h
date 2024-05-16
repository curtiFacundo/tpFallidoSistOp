typedef enum  
{
    NEW,
    READY,
    EXEC,
    BLOCK,
    EXIT
}t_estado;

typedef struct{
    int id;
    int* pc_id;
    int quantum;
    int estado;
}pcb;