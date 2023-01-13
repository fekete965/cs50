#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool will_cycle(int origin, int target);

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
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
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
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        string candidate = candidates[i];

        if (strcmp(candidate, name) == 0)
        {
            ranks[rank] = i;
            return 1;
        }
    }

    return 0;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Loop through the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int candidateId1 = ranks[i];
            int candidateId2 = ranks[j];
            // Increase preferred candidate's vote
            preferences[candidateId1][candidateId2]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            int preference1 = preferences[pairs[i].winner][pairs[i].loser];
            int preference2 = preferences[pairs[j].winner][pairs[j].loser];

            if (preference1 < preference2)
            {
                pair temp;
                temp.loser = pairs[i].loser;
                temp.winner = pairs[i].winner;

                pairs[i].loser = pairs[j].loser;
                pairs[i].winner = pairs[j].winner;

                pairs[j].loser = temp.loser;
                pairs[j].winner = temp.winner;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        pair currentPair = pairs[i];
        locked[currentPair.winner][currentPair.loser] = !will_cycle(currentPair.winner, currentPair.loser);
    }

    return;
}

// Check if we will create a cycle during pair locking
bool will_cycle(int origin, int target)
{
    // Check if we get back to the origin eventually
    if (origin == target)
    {
        return true;
    }

    // Otherwise, find the target and check for more cycle
    for (int i = 0; i < pair_count; i++)
    {
        if (locked[target][i] && will_cycle(origin, i))
        {
            return true;
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Loop through the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Set up our initial flag to true
        int winnerFound = 1;

        // Start an inner loop to check whose locked
        for (int j = 0; j < candidate_count; j++)
        {
            // If anybody is locked break the loop and set winnerFound flag to false
            if (locked[j][i] == true)
            {
                winnerFound = 0;
                break;
            }
        }

        // If we found a winner, print the winner's name
        if (winnerFound)
        {
            printf("%s\n", candidates[i]);
            return;
        }
        else
        {
            // Otherwise, continue the loop 
            continue;
        }
    }

    return;
}
