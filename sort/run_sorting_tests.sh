#!/bin/bash

# Define the sorting programs
SORT_PROGRAMS=("sort1" "sort2" "sort3")

# Define the input files
INPUT_FILES=(
             #"sorted5000.txt" "random5000.txt" "reversed5000.txt"
             "sorted10000.txt" "random10000.txt" "reversed10000.txt"
             #"sorted50000.txt" "random50000.txt" "reversed50000.txt"
             )

# Define the number of runs
NUM_RUNS=50

# Create a CSV file to store the results
OUTPUT_FILE="sorting_results.csv"
echo "Program,InputFile,AvgRealTime" > "$OUTPUT_FILE"

# Loop through each sorting program
for program in "${SORT_PROGRAMS[@]}"; do
    for input_file in "${INPUT_FILES[@]}"; do
        total_real_time=0

        # Run the program multiple times and accumulate real times
        for ((i = 1; i <= NUM_RUNS; i++)); do
            # Measure the real time and extract it using 'awk'
            real_time=$( { time ./"$program" "$input_file" 2>&1; } 2>&1 | grep real | awk '{print $2}' )
            
            # Convert the time to seconds (remove 'm' and 's' characters)
            real_time_seconds=$(echo "$real_time" | sed 's/[ms]//g')

            # Add the real time to the total
            total_real_time=$(echo "$total_real_time + $real_time_seconds" | bc -l)
        done

        # Calculate the average real time
        average_real_time=$(echo "$total_real_time / $NUM_RUNS" | bc -l)

        # Print the results and append to the CSV file
        echo "Average real time for $program with $input_file: $average_real_time seconds"
        echo "$program,$input_file,$average_real_time" >> "$OUTPUT_FILE"
    done
done
