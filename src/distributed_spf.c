#include "distributed_spf.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define STANDARD_TIMEOUT 2
#define DEBUG 1
#define VERBOSE 0

int calls = 0;
int rank, n;
int dist = INT_MAX;
int parent = -1;
int timeOutSecs;
int n_neighbors;
struct Edge **edges;

// if the distance is smaller than the current distance, update the distance and the parent

void spf(MPI_Comm *graph_comm, bool beginner_node){
    MPI_Request **request = malloc(n_neighbors * sizeof(MPI_Request *));;

    // n -1 rounds
    int *recData = start_recieving(graph_comm, request);
    for(int i = 0; i < n-1; i++){
        //start recieving with MPI_Irecv
        

        //only send if we got a distance smaller than INT_MAX
        if(dist != INT_MAX){
            // each round, each node sends its distance to its neighbors
            send_dist_to_neighbors(graph_comm, dist);
        }

        //wait for timeout and check if something was recieved
        wait_for_timeout_and_process_result(graph_comm, request, recData);
    }
    free(request);
    free(recData);
}

int calc_weight(int node1, int node2){
    //TODO: implement ping
    return (int)((node1 + node2)/2)+1;
}

void init_neighbors(MPI_Comm *graph_comm){
    MPI_Graph_neighbors_count(*graph_comm, rank, &n_neighbors);

    // Get the neighbors
    int *neighbors = malloc(n_neighbors * sizeof(int));
    MPI_Graph_neighbors(*graph_comm, rank, n_neighbors, neighbors);
    edges = malloc(n_neighbors * sizeof(struct Edge *));
    for(int i = 0; i < n_neighbors; i++){
        edges[i] = malloc(sizeof(struct Edge));
        edges[i]->neighbor = neighbors[i];
        edges[i]->weight = calc_weight(rank, neighbors[i]);
    }
    free(neighbors);
}


int *start_recieving(MPI_Comm *graph_comm, MPI_Request **request){
    int *recData = malloc(n_neighbors * sizeof(int));
    for(int i = 0; i < n_neighbors; i++){
        request[i] = malloc(sizeof(MPI_Request));
        if(VERBOSE)
            printf("Rank: %d, Start recieving from: %d\n", rank, edges[i]->neighbor);
        MPI_Irecv(&recData[i], 1, MPI_INT, edges[i]->neighbor, 0, *graph_comm, request[i]);
    }
    return recData;
}

void send_dist_to_neighbors(MPI_Comm *graph_comm, int dist){
    for(int i = 0; i < n_neighbors; i++){
        if(edges[i]->neighbor == parent){
            continue;
        }
        if(VERBOSE)
            printf("Rank: %d, Sending distance: %d to neighbor: %d\n", rank, dist, edges[i]->neighbor);
        MPI_Send(&dist, 1, MPI_INT, edges[i]->neighbor, 0, *graph_comm);
        calls++;
    }
}

void wait_for_timeout_and_process_result(MPI_Comm *graph_comm, MPI_Request **request, int *recData){
    int *flag = malloc(n_neighbors * sizeof(int));
    int timeout = 0;
    //wait for timeout
    while(timeout < timeOutSecs){
        sleep(1);
        timeout++;
    }
    //if something was recieved, process it
    MPI_Status *status = malloc(n_neighbors * sizeof(MPI_Status));
    for(int i = 0; i < n_neighbors; i++){
        MPI_Test(request[i], &flag[i], &status[i]);
        if(flag[i]){
            //process result
            if((dist == INT_MAX) || (recData[i] < (dist + edges[i]->weight))){
                dist = recData[i] + edges[i]->weight;
                parent = edges[i]->neighbor;
                if(DEBUG)
                    printf("Rank: %d, New parent: %d, New distance to 0: %d\n", rank, parent ,dist);
            }
            MPI_Irecv(&recData[i], 1, MPI_INT, edges[i]->neighbor, 0, *graph_comm, request[i]);
        }
    }
    free(status);
}



/**
 * @brief The main function of the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return int The exit status of the program.
 */
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // n = number of nodes / number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    if (argc > 1)
    {
        timeOutSecs = atoi(argv[1]);
        if (rank == 0)
            printf("Rank: %d, Setting timeout to %d\n", rank, timeOutSecs);
    }
    else
    {
        timeOutSecs = STANDARD_TIMEOUT;
    }
    
    int id_beginner_node = 0;    
    int edgeCount = 0;
    int *index = NULL;
    int *edgesArray = NULL;
    if(rank == 0){

        edgeCount = generateRandomConnectedNetwork(n, (int)((n/3)+1), &index, &edgesArray);
        printGraph(n, index, edgesArray);

    }
    MPI_Bcast(&edgeCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank != 0){
        index = malloc(n * sizeof(int));
        edgesArray = malloc(edgeCount * sizeof(int));
    }
    MPI_Bcast(edgesArray, edgeCount, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(index, n, MPI_INT, 0, MPI_COMM_WORLD);



    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, n, index, edgesArray, 0, &graph_comm);
    free(index);
    free(edgesArray);
    if(rank == id_beginner_node){
        dist = 0;
    }

    init_neighbors(&graph_comm);
    spf(&graph_comm, (rank == id_beginner_node));
    sleep(1);
    //print stuff
    //for n -1 rounds
        // only recieve from parent
        // send print command to all nodes
        // if print command recieved: print node + distance
    
    int depth;
    if(rank == id_beginner_node){
        depth = 0;
    }
    else{
        printf("Rank: %d, Waiting for parent: %d\n", rank, parent);
        MPI_Recv(&depth, 1, MPI_INT, parent, 0, graph_comm, MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < depth; i++)
    {
        printf("    ");
    }
    //print node + distance
    //printf("Node: %d, Distance: %d\n", rank, dist);
    if (depth > 0)
    {
        //printf(isLast ? "└── " : "|── ");
        printf("└── ");
    }

    printf("Rank: %d, Distance: (%d)\n", rank, dist);
    
    //send print command
    depth++;
    for(int i = 0; i < n_neighbors; i++){
        MPI_Send(&depth, 1, MPI_INT, edges[i]->neighbor, 0, graph_comm);      
    }
    MPI_Comm_free(&graph_comm);
    free(edges);
    MPI_Finalize();

    return 0;
}