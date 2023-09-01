#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX]; /// Creates the preference matrix or Tally

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX]; /// locked after sorting and locking from the strongest to the decreasing order of strength
/// doesn't lock if it create rock-paper situation

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair; /// Will be used to store the winner or loser between each of the pair of candidates

// Array of candidates
string candidates[MAX];          /// Stores the names of the candidates, number of elements will be equal to 9 (MAX 9)
pair pairs[MAX * (MAX - 1) / 2]; /// Creates the winner or loser pairs explanation below:

/* ----------------------------------------
    If MAX is 9, then the expression MAX * (MAX - 1) / 2
    would represent the number of ways to choose 2 items from a set of 9 distinct items,
    without considering the order. Let's calculate it:

    MAX = 9

    MAX * (MAX - 1) / 2 = 9 * (9 - 1) / 2 = 9 * 8 / 2 = 36

    So, if MAX is 9, then the expression evaluates to 36.
    This means there are 36 different ways to choose 2 items from a set of 9.
    This value is also equal to the combination 9C2, which can be calculated using the formula:

    9C2 = 9! / (2! * (9 - 2)!) = (9 * 8) / (2 * 1) = 36

 ----------------------------------------*/

int pair_count;      /// Actual number of pairs
int candidate_count; /// Actual number of candidate

// Function prototypes
bool vote(int rank, string name, int ranks[]); /// takes an integer rank, string name, and array of integers ranks[]
void record_preferences(int ranks[]);          /// recods preferences in the ranks[] array
void add_pairs(void);                          /// no idea
void sort_pairs(void);                           /// sort the pairs
void lock_pairs(void);                         /// lock the pairs
void print_winner(void);                       /// print winners
bool cycle(int end, int start);                /// Checks cycle in lock
void print_preferences(void); // Function to print the preferences matrix
void print_locked(void);      // Function to print the locked matrix

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i  = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1]; /// Storing the names of the candidates
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();

    print_preferences();
    print_locked();

    printf("\n");

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i; /* ranks[0] = 3; if candidates[3] = "Bob";
            Then the voter ranks Bob in the first position */
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;

            /*
            Update preferences of j over i

            Adds it every time a voter expressed
            s/he prefers such
            */
        }
    }

    return;
}

void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // Add the pair to the pairs array
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                pair_count++;
            }
        }
    }

    return;
}

void sort_pairs(void)
{
    int victory_strength[pair_count];
    // Create a separate array for storing victory strengths

    for (int i = 0; i < pair_count; i++)
    {
        victory_strength[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    int swapped;

    for (int j = 0; j < pair_count - 1; j++)
    {
        swapped = 0; // setting 0 for each inner loop

        for (int i = 0; i < pair_count - 1; i++)
        {
            if (victory_strength[i] < victory_strength[i + 1])
            {
                // Swap victory_strength
                int temp_strength = victory_strength[i];
                victory_strength[i] = victory_strength[i + 1];
                victory_strength[i + 1] = temp_strength;

                // Swap pairs (keep the original swapping logic)
                pair parking = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = parking;

                swapped = 1;
            }
        }

        if (swapped == 0) // Break if no more swap
        {
            break;
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Checks for cycles
bool cycle(int end, int start)
{
    if (end == start)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i])
            if (cycle(i, start))
            {
                return true;
            }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int false_count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                false_count++;
                if (false_count == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}

// Function to print the preferences matrix
void print_preferences(void)
{
    printf("Preferences Matrix:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%2d ", preferences[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to print the locked matrix
void print_locked(void)
{
    printf("Locked Matrix:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%s ", locked[i][j] ? "True" : "False");
        }
        printf("\n");
    }
    printf("\n");
}