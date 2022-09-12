// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Table of frequencies for hashing
int freqs[] = {24, 4, 8, 13, 36, 7, 6, 18, 22, 0, 2, 12, 8, 21, 23, 5, 0, 18, 19, 27, 9, 3, 6, 1, 6, 0};

const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Check
    node *tracker = table[hash(word)];

    // Make sure we don't segmentation fault ourselves
    if (tracker == NULL)
    {
        return false;
    }

    while (tracker->next != NULL)
    {
        if (strcasecmp(tracker->word, word) == 0)
        {
            return true;
        }

        tracker = tracker->next;
    }

    // Check once more because the last node has next = NULL
    if (strcasecmp(tracker->word, word) == 0)
    {
        return true;
    }


    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int total = freqs[toupper(word[0]) - 'A' + 1];

    int i = 1;
    while (word[i] != '\0' && i < 4)
    {
        total *= (toupper(word[i]) - 'A' + 1);
        i++;
    }


    return total % 10000;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open file
    FILE *dict_f = fopen(dictionary, "r");
    if (dict_f == NULL)
    {
        return false;
    }

    // Read one word at a time
    char c;
    char word[LENGTH + 1];
    int i = 0;
    int greatest = 0;

    while (fread(&c, sizeof(char), 1, dict_f))
    {
        if (isalpha(c) || c == '\'')
        {
            word[i] = tolower(c);
            i++;
        }
        else if (c == '\n')
        {
            // Copy word out of garbage chars
            if (i == 0)
            {
                continue;
            }

            char curr_word[i + 1];
            int j;
            for (j = 0; j < i; j++)
            {
                curr_word[j] = word[j];
            }
            curr_word[j] = '\0';

            i = 0;

            // File word in dictionary
            unsigned int curr_hash = hash(curr_word);
            //printf("%d\n", curr_hash);
            node *curr_node = malloc(sizeof(node));

            int k;
            for (k = 0; k < j; k++)
            {
                curr_node->word[k] = curr_word[k];
            }
            curr_node->word[k] = '\0';

            if (table[curr_hash] == NULL)
            {
                curr_node->next = NULL;
                table[curr_hash] = curr_node;
            }
            else
            {
                curr_node->next = table[curr_hash];
                table[curr_hash] = curr_node;
            }
        }
    }
    fclose(dict_f);


    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int total = 0;

    // Iterate through buckets
    for (int bucket = 0; bucket < N; bucket++)
    {
        int nodes = 0;
        if (table[bucket] == NULL)
        {
            continue;
        }

        node *tracker = table[bucket];
        while (tracker->next != NULL)
        {
            tracker = tracker->next;
            nodes++;
        }
        nodes++;

        total += nodes;
    }


    return total;
}

// Used recursively
void remove_node(node *n)
{
    if (n != NULL)
    {
        if (n->next != NULL)
        {
            remove_node(n->next);
        }

        // The core of the recursion
        free(n->word);
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through buckets
    for (int bucket = 0; bucket < N; bucket++)
    {
        remove_node(table[bucket]);
    }


    return true;
}
