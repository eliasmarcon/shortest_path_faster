#!/bin/bash

# Check for the number of tasks and diameter arguments
if [ "$#" -gt 3 ]; then
    echo "Usage: $0 [<runtype>] [<number_of_tasks>]"
    exit 1
fi

# Set default values if arguments are not provided
run_type=${1:-"local"}
num_tasks=${2:-5}

# Output file to save results
output_file="./shortest_path_faster_result.txt"

#clear the output file
> $output_file

if [ "$run_type" == "cluster" ]; then
    
    # Running on the Slurm cluster
    echo "Running on the Slurm cluster..."
    echo "Running Shortest Path Faster (SPFA) with $num_tasks MPI tasks..."
    srun -n $num_tasks --mpi=pmi2 ~/dev/spf/mpi_spf $max_tree_depth >> $output_file

elif [ "$run_type" == "local" ]; then
    
    make all
    
    # Running locally
    echo "Running locally..."
    echo "Running Shortest Path Faster (SPFA) with $num_tasks MPI tasks..."
    mpirun -np $num_tasks ./out/mpi_spf $max_tree_depth >> $output_file

else
    echo "Invalid run type. Use 'local' or 'cluster'."
    exit 1
fi

# show the result of the run in the command line
cat $output_file

echo "Completed."