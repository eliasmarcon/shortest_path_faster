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
temp_file="./temp_output.txt"

# Ensure file is created
touch "$output_file"

# Run the program and store the output in the temporary file
if [ "$run_type" == "cluster" ]; then
    # Running on the Slurm cluster
    echo "Running on the Slurm cluster..."
    echo "Running Shortest Path Faster (SPFA) with $num_tasks MPI tasks..."
    srun -n $num_tasks --mpi=pmi2 ./out/mpi_spf > $temp_file
elif [ "$run_type" == "local" ]; then
    make all
    # Running locally
    echo "Running locally..."
    echo "Running Shortest Path Faster (SPFA) with $num_tasks MPI tasks..."
    mpirun -np $num_tasks ./out/mpi_spf > $temp_file
else
    echo "Invalid run type. Use 'local' or 'cluster'."
    exit 1
fi

# Prepend the latest output to the existing content of the output file
cat "$temp_file" "$output_file" > "$output_file.tmp" && mv "$output_file.tmp" "$output_file"

# Show the result of the run in the command line
cat $temp_file

# Remove the temporary file
rm -f "$temp_file"

echo "Completed."
