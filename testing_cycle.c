#include <cs50.h>
#include <stdio.h>


#define MAX_CANDIDATES 100

int candidate_count;
bool locked[MAX_CANDIDATES][MAX_CANDIDATES];

bool cycle(int end, int start);

int main() {
    // Initialize the candidate_count and locked matrix with hardcoded data
    candidate_count = 3; // Change this value as needed
    bool adjacency_matrix[MAX_CANDIDATES][MAX_CANDIDATES] = {
        {false, true, false},
        {false, false, false},
        {true, true, false}
    };

    // Copy the adjacency matrix into the locked matrix
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            locked[i][j] = adjacency_matrix[i][j];
        }
    }

    // Test the cycle function for different start and end points
    int start, end;

    start = 0; // Change start and end values as needed
    end = 1;
    if (cycle(end, start)) {
        printf("There is a cycle from %d to %d.\n", start, end);
    } else {
        printf("There is no cycle from %d to %d.\n", start, end);
    }

    start = 1;
    end = 0;
    if (cycle(end, start)) {
        printf("There is a cycle from %d to %d.\n", start, end);
    } else {
        printf("There is no cycle from %d to %d.\n", start, end);
    }

    start = 1;
    end = 2;
    if (cycle(end, start)) {
        printf("There is a cycle from %d to %d.\n", start, end);
    } else {
        printf("There is no cycle from %d to %d.\n", start, end);
    }

    return 0;
}

bool cycle(int end, int start) {
    if (end == start) {
        return true;
    }

    for (int i = 0; i < candidate_count; i++) {
        if (locked[end][i]) {
            if (cycle(i, start)) {
                return true;
            }
        }
    }
    return false;
}
