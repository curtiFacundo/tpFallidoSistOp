sem_a=0
sem_e=0
sem_f=0
sem_s=0


A(){
    wait(sem_a)
    print(A)
    wait(sem_a)
    print(A)
    signal(sem_f)
}
E(){
    wait(sem_e)
    print(E)
    signal(sem_s)
}
F(){
    wait(sem_f)
    print(F)
    signal(sem_e)
}
S(){
    wait(sem_s)
    print(S)
    signal(sem_a)
    signal(sem_a)
}


sem_a=0
sem_e=0
sem_f=0
sem_s=0
/*
    SAAFE SAAFE ...
*/