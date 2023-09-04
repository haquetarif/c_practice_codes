#!/bin/bash

# Define the sorting programs
SORT_PROGRAMS=("sort1" "sort2" "sort3")

# Define the number of runs
NUM_RUNS=100

# Loop through each sorting program
for program in "${SORT_PROGRAMS[@]}"; do
	    total_real_time=0

	        # Run the program multiple times and accumulate real times
		    for ((i = 1; i <= NUM_RUNS; i++)); do
			            # Measure the real time and extract it using 'awk'
				            real_time=$( { time ./"$program" sorted5000.txt 2>&1; } 2>&1 | grep real | awk '{print $2}' )
					            
					            # Convert the time to seconds (remove 'm' and 's' characters)
						            real_time_seconds=$(echo "$real_time" | sed 's/[ms]//g')

							            # Add the real time to the total
								            total_real_time=$(echo "$total_real_time + $real_time_seconds" | bc -l)
									        done

										    # Calculate the average real time
										        average_real_time=$(echo "$total_real_time / $NUM_RUNS" | bc -l)

											    # Print the results
											        echo "Average real time for $program: $average_real_time seconds"
											done

