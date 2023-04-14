#include <stdio.h>
#include <mpi.h>

int my_id, nproc, resultlen;
char name[30];
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Get_processor_name(name, &resultlen);
    name[resultlen] = '\0';
    printf("Hola Mundo! Yo soy %d de %d ejecutando en %s\n", my_id, nproc, name);
    MPI_Finalize();
}





