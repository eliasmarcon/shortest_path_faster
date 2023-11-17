
#ifndef spf_h
#define spf_h
#include <mpi.h>
#include <stdbool.h>


void spf(MPI_Comm *graph_comm, bool beginner_node);
void init_neighbors(MPI_Comm *graph_comm);
int *start_recieving(MPI_Comm *graph_comm, MPI_Request **request);
void send_dist_to_neighbors(MPI_Comm *graph_comm, int dist);
void wait_for_timeout_and_process_result(MPI_Comm *graph_comm,  MPI_Request **request, int *recData);


#endif // spf_h