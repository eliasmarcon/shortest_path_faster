# shortest_path_faster

Finds all the shortest paths from one node to all other nodes in a graph using the shortest-path-faster algorithm

## Flow Chart

```mermaid
  flowchart TD
  A((Start)) --> B{Custom timeout supplied\nvia command params?}
  B --> |Yes| C[Set custom timeout]
  B --> |No| D[Set default timeout]
  C --> E[Set beginner node]
  D --> E
  E --> F[Calculate branching factor]
  F --> G[Generate graph]
  G --> H[Broadcast graph to all nodes]
  H --> I[Generate Graph comm]
  I --> J[Distance of root node = 0]
  J --> K[Detect neighbors\n+\ncalculate edge weights]
  K --> L[Start receiving messages]
  subgraph SPFA
  L --> M[Send messages to neighbors if distance is smaller than infinity]
  M --> N[After timeout has passed: Check for received messages]
  N --> O{Received messages?}
  O --> |Yes| P{New distance smaller \nthan current distance?}
  O --> |No| R{Max loops reached?}
  P --> |Yes| Q[Update distance, restart recieving messages]
  P --> |No| R
  Q --> R
  R --> |no| M
  end
  R --> |yes| S[Print results]
  S --> T(((End)))
```

## Applied optimizations

- do not send messages when the distance is infinity

### Possible further optimizations

- only send messages when the distance is smaller than the current distance


## Authors

- Elias Marcon MSc. | [@eliasmarcon](https://github.com/eliasmarcon)
- Ing. Fabian Steiner BSc.| [@fasteiner](https://github.com/fasteiner/)
- Jan Langela Regincos BSc. | [@janpilu](https://github.com/janpilu)

## Folder Structure

- `./include`:

  - `distributed_spf.h`: defines the data structure for the `distributed_spf.c` file

  - `Graph.h`: defines the data structure for the `Graph.c` file

- `./src`:

  - `distributed_spf.c`: initializes a distributed graph and performs a parallel implementation of the Shortest-Path-Faster Algorithm (SPFA) using MPI, where each process represents a node in the graph, and communication occurs between neighboring nodes to calculate the shortest path from a designated root node to all other nodes, with additional features such as timeout handling and message counting

  - `Graph.c`: defines functions for generating a random connected network represented as an adjacency list in C, including functions for checking and inserting edges, and generating the network. Optimizations from us are printing the resulting graph, including the Graph.h file and smaller code optimizations

- `./out`: contains the executable file `mpi_spf` for the shortest-path-faster algorithm created through the build process

- `Makefile`: configuration file that controls the build process of the MPI program (contains rules and commands to compile the project and create the executable file `mpi_spf`)

- `start_shortest_path_faster.sh`: automates the execution of the `make all` command and MPI program (shortest-path-faster) with a user-specified number of processes and a time out (saves the results of the run in the output file `shortest_path_faster_result.txt` for monitoring and analysis)

- `shortest_path_faster_result.txt`: contains the results of all runs with the latest run on top

## How to build

### Prerequisites

- Ubuntu 20.04
- mpich

### Build

- the `make all` command is included in the `start_shortest_path_faster.sh` file for the local run

### Usage

To use the provided Bash script for running the Shortest-Path-Faster Algorithm (SPFA), follow these steps:

1. Ensure that MPI is installed on your system.

2. Open a terminal and navigate to the directory containing this bash script.

3. Run the bash script by providing the run_type (cluster, local), the number of MPI tasks and the time out as command-line arguments.

- Default Parameters:
  - run_type = local
  - number_of_tasks = 5
  - time_out = 2

```sh
./start_distributed_bfs.sh <run_type> <number_of_tasks> <time_out> (replace `<run_type>`, `<number_of_tasks>` and <time_out> with the actual values)

./start_distributed_bfs.sh local 8 1

./start_distributed_bfs.sh cluster 10 2

```

## Example Graph with 5 Nodes/Processes

```mermaid
graph LR
0----1
0----2
0----3 
1----2
1----4 
2----3 
3----4 
```
