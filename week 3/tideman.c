#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
}
pair;

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
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    //print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int r = 0; r < candidate_count; r++)
    {
        for (int s = r + 1; s < candidate_count; s++)
        {
            preferences[ranks[r]][ranks[s]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Map strenghts
    int strengths[pair_count];
    int max_strength = 0;
    for (int p = 0; p < pair_count; p++)
    {
        strengths[p] = preferences[pairs[p].winner][pairs[p].loser] - preferences[pairs[p].loser][pairs[p].winner];
        max_strength = (strengths[p] > max_strength) ? strengths[p] : max_strength;
    }

    // Save the current order of pairs
    pair temp[pair_count];
    for (int p = 0; p < pair_count; p++)
    {
        temp[p] = pairs[p];
    }

    // Cycle backwards down the strengths
    int sorted = 0;
    for (int strength = max_strength; strength >= 0; strength--)
    {
        for (int p = 0; p < pair_count; p++)
        {
            if (strengths[p] == strength)
            {
                pairs[sorted] = temp[p];
                sorted++;
            }
        }
    }

    return;
}

// Return true for cycle
bool next_cand(int curr_cand, int start)
{
    for (int i = 0; i < MAX; i++)
    {
        if (locked[curr_cand][i])
        {
            if (i == start)
            {
                return true;
            }
            if (next_cand(i, start))
            {
                return true;
            }
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Prepare locked array
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            locked[i][j] = false;
        }
    }

    // Cycle through the pairs to (try to) lock in
    for (int p = 0; p < pair_count; p++)
    {
        locked[pairs[p].winner][pairs[p].loser] = true;

        // If we're now in a cycle, undo this move
        if (next_cand(pairs[p].loser, pairs[p].loser))
        {
            locked[pairs[p].winner][pairs[p].loser] = false;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    int candidate = 0;
    for (int count = 0; count < 100; count++)
    {
        for (int i = 0; i < MAX; i++)
        {
            if (locked[i][candidate])
            {
                candidate = i;
                break;
            }
        }
    }
    printf("%s\n", candidates[candidate]);


    return;
}